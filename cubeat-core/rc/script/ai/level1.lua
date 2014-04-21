
local ffi      = require 'ffi'
local helper   = require 'rc/script/helper'
local basepath = helper.basepath
local random   = helper.random
local shuffle  = helper.C_random_shuffle
local C        = ffi.C

-- we don't actually assign this to anything b/c it only setups cdefs and metatype
require 'rc/script/ai/ai'
local should_use_ability = require 'rc/script/ai/ability'
local event = require 'rc/script/event/event'

local function setcmd(buf, type, delay, x, y)
  buf.x, buf.y, buf.delay, buf.type = x, y, delay, type
end

local function pick_a_coord_from(map)
  local x, y
  repeat
    x = random(map:width())
    y = random(map:height()/2) -- consider lower half only
  until map:get_grounded_cube(x, y):exist()
  return x, y
end

local function pick_a_higher_half_coord_from(map)
  local x, y
  local bail_out_count = 0
  repeat
    x = random(map:width())
    y = random(map:height()/2) + math.floor(map:height()/2) -- consider higher half only
    bail_out_count = bail_out_count + 1
  until map:get_grounded_cube(x, y):exist() or bail_out_count > 10
  return x, y
end

local function attack_power_of(firemap, x, y)
  return firemap[x*firemap:height() + y]
end

local ATTACK_PWR     = 1
local DELAY          = 0  --ms -- currently not very useful. it should be useful. 
 
--these are intended for C to call from.
function THINK_INTERVAL() return 550 end --ms
function MISSRATE()       return 0  end --percentage. 0 ~ 100

function normal_attack_consideration(keycube, power, my_map, enemy_map) 
  return keycube:exist() 
end

function throttled_attack_consideration(keycube, power, my_map, enemy_map)
  return keycube:exist() and random(100) > 75 and  
         enemy_map:garbage_left() + power <= 6 -- so opponent doesn't feel like they are being overpowered too much.
end

function ai_entry(self)

  local t = os.clock()

  self = ffi.cast("AIPlayer*", self)

  --since we only have two map, one for each side, so let the first in ally-list be one's self.
  local my_map    = self:get_ally_map(0)
  local enemy_map = self:get_enemy_map(0)
  local cmdbuf    = ffi.new("LuaAICommand", {0, 0, 0, C.AI_NONE}) -- reuse this
  local ab_kind   = self:ability_kind()
  local ab_left   = self:ability_left()
  local capacity  = my_map:width() * (my_map:height() - 1)
  local ground_cube_num = my_map:grounded_cube_count()
  
  local emergency_level = 0

  local attack_threshold = 1
  
  if my_map:warning_level() > 0 or
     ground_cube_num + my_map:garbage_left() >= capacity
  then
    emergency_level = 42 -- dummy for easy level computer
  end

  local keycube, power = my_map:get_firepoint_cube(attack_threshold, ATTACK_PWR, emergency_level)

  -- Fire Map test 
  local firemap = my_map:get_firemap()
  
  for y = my_map:height() - 2, 0, -1 do
    for x = 0, my_map:width() - 1 do
      io.write(string.format("%2d", firemap:power_at(x, y)))
    end
    io.write("\n")
  end
  io.write("\n")
  
  if ( enemy_map:grounded_cube_count() < capacity * 0.5 and normal_attack_consideration(keycube, power, my_map, enemy_map) )
     or
     ( throttled_attack_consideration(keycube, power, my_map, enemy_map) ) 
  then
    --io.write( string.format("keycube at: %d, %d\n", keycube:x(), keycube:y()) )
    setcmd(cmdbuf, C.AI_SHOOT, 0, keycube:x(), keycube:y())
    self:push_command(cmdbuf)
    if keycube:is_broken() then
      self:push_command(cmdbuf)
    end
  else
    -- io.write "No keycube for now.\n"
    
    local highcol_threshold = 7
    local highcols, hsize = my_map:get_highcols( highcol_threshold )
    local brokens,  bsize = my_map:get_brokens()

    -- we got some problem at this procedure:
    -- Usually we want to clear high columns asap, but when your map is already full, this becomes inappropriate.
    -- because we want to clear as many broken cubes as possible to get more chances at chaining (thus locking the countdown)
    -- but this no-brainer process kills AI, since even if there are still plenty of normal cube to shoot from,
    -- it will still take its time to clear broken cubes. 
    -- HOWEVER, if you always just clear high columns, without considering shooting at broken cubes, 
    -- your map will become full of broken cubes very soon, and that's almost a certain death. 
    -- so the middle way to take here is, when the number of broken cubes is not too many, 
    -- then by all means take out high columns by shooting at the lower half of the column,
    -- but when the broken cubes taking a portion of your map, you should consider shooting at them first.    
    if hsize > 0 and --[[ground_cube_num <= capacity * 0.9 and ]]bsize < ground_cube_num * 0.1 then
      shuffle(highcols, hsize)
      local rnd_x, rnd_height = highcols[random(hsize)], random( highcol_threshold/2 ) + highcol_threshold/2
      setcmd(cmdbuf, C.AI_SHOOT, 0, rnd_x, rnd_height)
      self:push_command(cmdbuf)
      -- if my_map:get_grounded_cube(rnd_x, rnd_height):is_broken() then
        -- self:push_command(cmdbuf)
      -- end
    end

    if bsize > 0 and self:cmdqueue_size() < 1 then
      shuffle(brokens, bsize)
      local rnd = random(bsize)
      setcmd(cmdbuf, C.AI_SHOOT, 0, brokens[rnd]:x(), brokens[rnd]:y())
      self:push_command(cmdbuf)
    end
    -- don't do garbages for now.
    
    if self:cmdqueue_size() < 1 then
      if ground_cube_num >= capacity * 0.5 --[[and not my_map:still_chaining()]] then
        local x, y = pick_a_higher_half_coord_from(my_map)
        if ( not keycube:exist() or (x ~= keycube:x() or y ~= keycube:y()) ) and 
           my_map:get_grounded_cube(x, y):exist() and
           random(100) > 50 
        then
          setcmd(cmdbuf, C.AI_SHOOT, 0, x, y)
          self:push_command(cmdbuf)
        end
      else
        if self:get_heat() < 0.7 and ground_cube_num <= capacity * 0.6 and random(100) > 40 then
          setcmd(cmdbuf, C.AI_HASTE, 0, 0, 0)
          self:push_command(cmdbuf)
        end
      end
    end
  end

  collectgarbage("collect")
end
