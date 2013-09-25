
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

local ATTACK_PWR     = 1
local DELAY          = 0  --ms -- currently not very useful. it should be useful. 
 
--these are intended for C to call from.
local interval_ = 450
local missrate_ = 0

function THINK_INTERVAL() return interval_ end --ms
function MISSRATE()       return missrate_ end --percentage. 0 ~ 100

function set_interval(n) interval_ = n end
function set_missrate(n) missrate_ = n end

function ai_entry(self)

  local t = os.clock()

  self = ffi.cast("AIPlayer*", self)

  --since we only have two map, one for each side, so let the first in ally-list be one's self.
  local my_map    = self:get_ally_map(0)
  local enemy_map = self:get_enemy_map(0)
  local cmdbuf    = ffi.new("LuaAICommand", {0, 0, 0, C.AI_NONE}) -- reuse this
  local capacity  = my_map:width() * (my_map:height() - 1)
  local ground_cube_num = my_map:grounded_cube_count()
  
  local emergency_level = 1
  local attack_threshold = 1

  local keycube = my_map:get_firepoint_cube(attack_threshold, ATTACK_PWR, emergency_level)

  local t2 = os.clock() - t

  if keycube:exist() and
     enemy_map:garbage_left() < ATTACK_PWR * 2 -- so opponent doesn't feel like they are being overpowered too much.
  then
    --io.write( string.format("keycube at: %d, %d\n", keycube:x(), keycube:y()) )
    setcmd(cmdbuf, C.AI_SHOOT, 0, keycube:x(), keycube:y())
    self:push_command(cmdbuf)
    if keycube:is_broken() then
      self:push_command(cmdbuf)
    end
  else
    --io.write "No keycube for now.\n"
    
    local highcol_threshold = 8
    local highcols, hsize = my_map:get_highcols( highcol_threshold )
    
    if hsize > 0 then
      shuffle(highcols, hsize)
      local rnd_x, rnd_height = highcols[random(hsize)], random( highcol_threshold/2 )
      setcmd(cmdbuf, C.AI_SHOOT, 0, rnd_x, rnd_height)
      self:push_command(cmdbuf)
      -- if my_map:get_grounded_cube(rnd_x, rnd_height):is_broken() then
        -- self:push_command(cmdbuf)
      -- end
    end

    -- if self:cmdqueue_size() < 1 then
      -- if ground_cube_num >= capacity * 0.8 and not my_map:still_chaining() then
        -- local x, y = pick_a_coord_from(my_map)
        -- setcmd(cmdbuf, C.AI_SHOOT, 0, x, y)
        -- self:push_command(cmdbuf)
      -- -- else
        -- -- if self:get_heat() < 0.7 then
          -- -- setcmd(cmdbuf, C.AI_HASTE, 0, 0, 0)
          -- -- self:push_command(cmdbuf)
        -- -- end
      -- end
    -- end
  end

  --io.write(string.format("Hard AI current mem: %.2f(K), up-to-keycube time: %.3f, total time: %.3f\n", collectgarbage("count"), t2, os.clock() - t))
  collectgarbage("collect")

end
