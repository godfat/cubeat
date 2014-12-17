
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

local function pick_a_higher_half_coord_from(map, avoid_list)
  local x, y
  local bail_out_count = 0
  repeat
    x = random(map:width())
    y = random(map:height()/2) + math.floor(map:height()/2) -- consider higher half only
    reroll = false
    if avoid_list then
      for i = 1, #avoid_list do 
        if x == avoid_list[i][1] and y == avoid_list[i][2] then
          reroll = true
          break
        end
      end
    end
    bail_out_count = bail_out_count + 1
  until ( map:get_grounded_cube(x, y):exist() and reroll == false ) or bail_out_count > 6
  return x, y, bail_out_count
end
 
--these are intended for C to call from.
function THINK_INTERVAL() return 800 end --ms
function MISSRATE()       return 0  end --percentage. 0 ~ 100

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
  local base_atk  = my_map:get_base_attack()

  -- Fire Map test 
  local firemap = my_map:get_firemap()
  local excess_power_list = {}
  local ok_power_list = {}
  local emergency_list = {}
  
  local excessive_threshold = 6
  if enemy_map:grounded_cube_count() < capacity * 0.2 then 
    excessive_threshold = 20
  end 
  
  for y = my_map:height() - 2, 0, -1 do
    for x = 0, my_map:width() - 1 do
      local pwr = firemap:power_at(x, y)
      -- io.write(string.format("%2d", pwr))
      if pwr > excessive_threshold and pwr > base_atk + 1 then 
        table.insert(excess_power_list, {x, y}) 
      elseif pwr >= 3 and pwr <= excessive_threshold and pwr > base_atk + 1 then 
        table.insert(ok_power_list, {x, y})
      elseif pwr >  0 and pwr <  3 then 
        table.insert(emergency_list, {x, y})
      end
    end
    -- io.write("\n")
  end
  io.write("\n")
  
  io.write(string.format(" - Current AI base_attack: %d\n", base_atk))
  if #excess_power_list > 0 then io.write(string.format("excess_list head: (%d, %d), pwr: %d\n", excess_power_list[1][1], excess_power_list[1][2], firemap:power_at(excess_power_list[1][1], excess_power_list[1][2]) )) end
  if #ok_power_list     > 0 then io.write(string.format("ok_list     head: (%d, %d), pwr: %d\n", ok_power_list[1][1], ok_power_list[1][2], firemap:power_at(ok_power_list[1][1], ok_power_list[1][2]))) end
  if #emergency_list    > 0 then io.write(string.format("emergency   head: (%d, %d), pwr: %d\n", emergency_list[1][1], emergency_list[1][2], firemap:power_at(emergency_list[1][1], emergency_list[1][2]))) end
  
  -- if my_map:still_chaining() then 
    -- io.write(" My Map is still chaining. Try not to interrupt it.\n")
    -- return 
  -- end
  
  if enemy_map:grounded_cube_count() < capacity * 0.2 and 
     #ok_power_list > 0 and event.get_time_of('game') > 20000
  then
    io.write(string.format(" - entering keycube (ok_power: preemptive attack) block\n"))
    setcmd(cmdbuf, C.AI_SHOOT, 0, ok_power_list[1][1], ok_power_list[1][2])
    self:push_command(cmdbuf)
    
  elseif enemy_map:grounded_cube_count() < capacity * 0.6 and 
     enemy_map:warning_level() < 1 and #ok_power_list > 0 and random(100) > 66 and
     event.get_time_of('game') > 10000
  then
    --io.write( string.format("keycube at: %d, %d\n", keycube:x(), keycube:y()) )
    --setcmd(cmdbuf, C.AI_SHOOT, 0, keycube:x(), keycube:y())
    io.write(string.format(" - entering keycube (ok_power) block\n"))
    setcmd(cmdbuf, C.AI_SHOOT, 0, ok_power_list[1][1], ok_power_list[1][2])
    self:push_command(cmdbuf)
  
  elseif my_map:warning_level() > 0 and #emergency_list > 0 and random(100) > 66
  then
    io.write(string.format(" - entering emergency block\n"))
    setcmd(cmdbuf, C.AI_SHOOT, 0, emergency_list[1][1], emergency_list[1][2])
    self:push_command(cmdbuf)
  else
    -- io.write "No keycube for now.\n"
    
    local highcol_threshold = 7
    if enemy_map:grounded_cube_count() < capacity * 0.25 then
      highcol_threshold = 8
    end
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
    
    if hsize > 0 and bsize < ground_cube_num * 0.1 and random(100) > 25 then
      shuffle(highcols, hsize)
      local rnd_x, rnd_height
      local reroll
      local bail_out_count = 0
      repeat 
        rnd_x, rnd_height = highcols[random(hsize)], random( highcol_threshold/2 ) + highcol_threshold/2
        reroll = false
        for i = 1, #excess_power_list do
          if rnd_x == excess_power_list[i][1] and rnd_height == excess_power_list[i][2] then
            reroll = true
            break
          end
        end
        bail_out_count = bail_out_count + 1
      until reroll == false or bail_out_count > 5
      
      if bail_out_count <= 5 then
        io.write(string.format(" - entering highcol block, shooting(%d, %d) \n", rnd_x, rnd_height))

        setcmd(cmdbuf, C.AI_SHOOT, 0, rnd_x, rnd_height)
        self:push_command(cmdbuf)
      end
      -- if my_map:get_grounded_cube(rnd_x, rnd_height):is_broken() then
        -- self:push_command(cmdbuf)
      -- end
    end

    if bsize > 0 and self:cmdqueue_size() < 1 and random(100) > 50 then
      shuffle(brokens, bsize)
      local rnd
      local reroll
      local bail_out_count = 0
      repeat
        rnd = random(bsize)
        reroll = false
        for i = 1, #excess_power_list do 
          if brokens[rnd]:x() == excess_power_list[i][1] and brokens[rnd]:y() == excess_power_list[i][2] then
            reroll = true
            break
          end
        end
        bail_out_count = bail_out_count + 1
      until reroll == false or bail_out_count > 5
      if bail_out_count <= 5 then
        io.write(string.format(" - entering broken block, shooting(%d, %d) \n", brokens[rnd]:x(), brokens[rnd]:y()))
        
        setcmd(cmdbuf, C.AI_SHOOT, 0, brokens[rnd]:x(), brokens[rnd]:y())
        self:push_command(cmdbuf)
      end
    end
    -- don't do garbages for now.
    
    if self:cmdqueue_size() < 1 then
      
      local anti_emptiness_factor = 0.3
      if enemy_map:grounded_cube_count() < capacity * 0.4 then anti_emptiness_factor = 0.6 
      elseif enemy_map:grounded_cube_count() < capacity * 0.6 then anti_emptiness_factor = 0.5 end
      
      if ground_cube_num >= capacity * anti_emptiness_factor and 
         random(100) > (1 - anti_emptiness_factor) * 100 
      then
        local x, y, bail_out_count = pick_a_higher_half_coord_from(my_map)
        if my_map:get_grounded_cube(x, y):exist() and bail_out_count <= 6
        then
          io.write(string.format(" - entering random block, shooting(%d, %d) \n", x, y))
        
          setcmd(cmdbuf, C.AI_SHOOT, 0, x, y)
          self:push_command(cmdbuf)
        end
      else
        
        if self:get_heat() < 0.7 and my_map:warning_level() < 1 and 
           ground_cube_num <= capacity * anti_emptiness_factor and random(100) > (1 - anti_emptiness_factor) * 100 
        then
          io.write(string.format(" - entering haste block\n"))
          setcmd(cmdbuf, C.AI_HASTE, 400, 0, 0)
          self:push_command(cmdbuf)
        end
      end
    end
  end
  
  if self:cmdqueue_size() < 1 then
    io.write(" - IDLE\n")
  end

  collectgarbage("collect")
end
