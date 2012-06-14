
local ffi      = require 'ffi'
local helper   = require 'rc/script/helper'
local basepath = helper.basepath
local random   = helper.random
local shuffle  = helper.C_random_shuffle
local C        = ffi.C

-- we don't actually assign this to anything b/c it only setups cdefs and metatype
require 'rc/script/ai/ai' 

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

local ATTACK_PWR     = 99
local DELAY          = 0  --ms -- currently not very useful. it should be useful. 
 
--these are intended for C to call from.
function THINK_INTERVAL() return 300 end --ms
function MISSRATE()       return 4   end --percentage. 0 ~ 100

function ai_entry(self)

  local t = os.clock()

  self = ffi.cast("AIPlayer*", self)
  
  --since we only have two map, one for each side, so let the first in ally-list be one's self.
  local my_map =    self:get_ally_map(0)
  local enemy_map = self:get_enemy_map(0)
  local cmdbuf    = ffi.new("LuaAICommand", {0, 0, 0, C.PSC_AI_NONE}) -- reuse this
  local emergency_level = 0
  
  local attack_threshold = 5
  if ATTACK_PWR < 9 then      attack_threshold = 1
  elseif ATTACK_PWR < 20 then attack_threshold = 3
  end
  
  if my_map:warning_level() > 25 or 
     my_map:grounded_cube_count() + my_map:garbage_left() > my_map:width() * (my_map:height() - 1) 
  then 
    emergency_level = 1
  end
  
  if self:get_heat() > 0.7 then return end -- fix for insane computer, too fast.
  
  local keycube = my_map:get_firepoint_cube(attack_threshold, ATTACK_PWR, emergency_level)
  
  local t2 = os.clock() - t
  
  if keycube:exist() and 
     enemy_map:garbage_left() < ATTACK_PWR * 2 -- so opponent doesn't feel like they are being overpowered too much.
  then 
    io.write( string.format("keycube at: %d, %d\n", keycube:x(), keycube:y()) )
    setcmd(cmdbuf, C.PSC_AI_SHOOT, 0, keycube:x(), keycube:y())
    self:push_command(cmdbuf)
    if keycube:is_broken() then
      self:push_command(cmdbuf)
    elseif keycube:is_garbage() then
      self:push_command(cmdbuf)
      self:push_command(cmdbuf)
    end
  else
    io.write "No keycube for now.\n"
    local highcol_threshold = 9
    local highcols, hsize = my_map:get_highcols( highcol_threshold )
    local brokens,  bsize = my_map:get_brokens()
    
    if hsize > 0 and my_map:grounded_cube_count() <= 54 then
      shuffle(highcols, hsize)
      local rnd_x, rnd_height = highcols[random(hsize)], random( highcol_threshold/2 )
      setcmd(cmdbuf, C.PSC_AI_SHOOT, 0, rnd_x, rnd_height)
      self:push_command(cmdbuf)
      if my_map:get_grounded_cube(rnd_x, rnd_height):is_broken() then
        self:push_command(cmdbuf)
      end
    end    
    
    if bsize > 0 and self:cmdqueue_size() < 1 then 
      shuffle(brokens, bsize)
      local rnd = random(bsize)
      setcmd(cmdbuf, C.PSC_AI_SHOOT, 0, brokens[rnd]:x(), brokens[rnd]:y())
      self:push_command(cmdbuf)
    end
    -- don't do garbages for now.
    
    if self:cmdqueue_size() < 1 then
      if my_map:grounded_cube_count() >= 48 and not my_map:still_chaining() then
        local x, y = pick_a_coord_from(my_map)
        setcmd(cmdbuf, C.PSC_AI_SHOOT, 0, x, y)
        self:push_command(cmdbuf) 
      else
        local interrupt_cube = enemy_map:get_firepoint_cube(3, 99, 0)
        if interrupt_cube:exist() then 
          local dir = { {-1, 1}, {0, 2}, {1, 1} } -- don't do bottom
          local chance = random(3)+1
          local x = dir[chance][1] + interrupt_cube:x()
          local y = dir[chance][2] + interrupt_cube:y()
          if x >= 0 and x < enemy_map:width() and y >= 0 and y < enemy_map:height()-1 then
            local c = enemy_map:get_cube(x, y)
            if c:exist() and not c:is_broken() then
              io.write(("AI: I think %d, %d is important, so I jama %d, %d\n"):format(interrupt_cube:x(), interrupt_cube:y(), x, y))
              setcmd(cmdbuf, C.PSC_AI_SHOOT_OTHER, 0, x, y)
              self:push_command(cmdbuf) 
            end
          end
        elseif self:get_heat() < 0.7 then
          setcmd(cmdbuf, C.PSC_AI_HASTE, 0, 0, 0) 
          self:push_command(cmdbuf) 
        end
      end
    end
  end
  
  io.write(string.format("Isne AI current mem: %.2f(K), up-to-keycube time: %.3f, total time: %.3f\n", collectgarbage("count"), t2, os.clock() - t))
  collectgarbage("collect")
  
end
