local ffi     = require 'ffi'
local C       = ffi.C
local view    = require 'rc/script/ui/view'
local ui      = require 'rc/script/ui/ui'
local config  = require 'rc/script/ui/demo/talk/config'


local actor = {}
local word  = {}


local function shake(loop, sprite, x, y, move, dur)
  local tween     = {}
  local tweenback = {}
  for i=loop,1,-1 do
    if i==loop then
      tweenback[i]= function(self) self:tween("Linear", "Pos2D", ffi.new("value2", x+move, y), ffi.new("value2", x-move, y), dur, 0,
                      function(self) self:tween("Linear", "Pos2D", ffi.new("value2", x-move, y), ffi.new("value2", x, y), dur/2, 0) end)
                    end
    else
      tweenback[i]= function(self) self:tween("Linear", "Pos2D", ffi.new("value2", x+move, y), ffi.new("value2", x-move, y), dur, 0, tween[i+1]) end
    end
    tween[i]    = function(self) self:tween("Linear", "Pos2D", ffi.new("value2", x-move, y), ffi.new("value2", x+move, y), dur, 0, tweenback[i]) end
  end
  sprite:tween("Linear", "Pos2D", ffi.new("value2", x-move, y), ffi.new("value2", x+move, y), dur, 0, tweenback[1])
end

----

actor.show = function(t)
  t.menu[t.actor]:set_visible(true)
end

actor.hide = function(t)
  t.menu[t.actor]:set_visible(false)
end

actor.slide_in = function(t)
  local act_x = config.act_x[t.ch]
  local act_y = config.act_y[t.ch]
  local out_x = config.out_x[t.ch]
  local out_y = config.out_y[t.ch]
  t.menu[t.actor]:set_visible(true)
  t.menu[t.actor]:tween("Linear", "Pos2D", ffi.new("value2", out_x, out_y), ffi.new("value2", act_x, act_y), 200)
end

actor.slide_out = function(t)
  local act_x = config.act_x[t.ch]
  local act_y = config.act_y[t.ch]
  local out_x = config.out_x[t.ch]
  local out_y = config.out_y[t.ch]
  t.menu[t.actor]:set_visible(true)
  t.menu[t.actor]:tween("Linear", "Pos2D", ffi.new("value2", act_x, act_y), ffi.new("value2", out_x, out_y), 200)
end

actor.fade_in = function(t)
  t.menu[t.actor]:set_visible(true)
  t.menu[t.actor]:tween("Linear", "Alpha", 0, 255, 1000)
end

actor.fade_out = function(t)
  t.menu[t.actor]:set_visible(true)
  t.menu[t.actor]:tween("Linear", "Alpha", 255, 0, 1000)
end

actor.shake = function(t)
  local act_x = config.act_x[t.ch]
  local act_y = config.act_y[t.ch]
  t.menu[t.actor]:set_visible(true)
  shake(6, t.menu[t.actor], act_x, act_y, 5, 50)
end

----

word.size_L = function(t)
  t.menu[t.content]:set_scale( 1.7 )
end

word.size_M = function(t)
  t.menu[t.content]:set_scale( 1 )
end

word.size_S = function(t)
  t.menu[t.content]:set_scale( 0.7 )
end

word.shake = function(t)
  local con_x = config.con_x[t.ch]
  local con_y = config.con_y[t.ch]
  t.menu[t.content]:set_scale( 1.7 )
  shake(3, t.menu[t.content], con_x, con_y, 3, 100)
end


return {
  actor = actor,
  word  = word
}