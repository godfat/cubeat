local ffi     = require 'ffi'
local C       = ffi.C
local view    = require 'rc/script/ui/view'
local ui      = require 'rc/script/ui/ui'
local config  = require 'rc/script/ui/demo/talk/config'
local flag    = require 'rc/script/ui/demo/talk/flag'


local actor_ = {}
local word_  = {}


local function shake(loop, sprite, x, y, dis, dur, cb)
  local tween     = {}
  local tweenback = {}
  for i=loop,1,-1 do
    if i==loop then
      tweenback[i]= function(self) self:tween("Linear", "Pos2D", ffi.new("value2", x+dis, y), ffi.new("value2", x-dis, y), dur, 0,
                      function(self) self:tween("Linear", "Pos2D", ffi.new("value2", x-dis, y), ffi.new("value2", x, y), dur/2, 0, cb) end)
                    end
    else
      tweenback[i]= function(self) self:tween("Linear", "Pos2D", ffi.new("value2", x+dis, y), ffi.new("value2", x-dis, y), dur, 0, tween[i+1]) end
    end
    tween[i]    = function(self) self:tween("Linear", "Pos2D", ffi.new("value2", x-dis, y), ffi.new("value2", x+dis, y), dur, 0, tweenback[i]) end
  end
  sprite:tween("Linear", "Pos2D", ffi.new("value2", x-dis, y), ffi.new("value2", x+dis, y), dur, 0, tweenback[1])
end

----

actor_.show = function(t)
  t.menu[t.actor]:set_visible(true)
  flag.actor_is_ready()
end

actor_.hide = function(t)
  t.menu[t.actor]:set_visible(false)
  flag.actor_is_ready()
end

actor_.slide_in = function(t)
  local act_x = config.act_x[t.ch]
  local act_y = config.act_y[t.ch]
  local out_x = config.out_x[t.ch]
  local out_y = config.out_y[t.ch]
  t.menu[t.actor]:set_visible(true)
  t.menu[t.actor]:tween("Linear", "Pos2D", ffi.new("value2", out_x, out_y), ffi.new("value2", act_x, act_y), config.slide_in_time, 0, flag.actor_cb())
end

actor_.slide_out = function(t)
  local act_x = config.act_x[t.ch]
  local act_y = config.act_y[t.ch]
  local out_x = config.out_x[t.ch]
  local out_y = config.out_y[t.ch]
  t.menu[t.actor]:set_visible(true)
  t.menu[t.actor]:tween("Linear", "Pos2D", ffi.new("value2", act_x, act_y), ffi.new("value2", out_x, out_y), config.slide_out_time, 0, flag.actor_cb())
end

actor_.fade_in = function(t)
  t.menu[t.actor]:set_visible(true)
  t.menu[t.actor]:tween("Linear", "Alpha", 0, 255, config.fade_in_time, 0, flag.actor_cb())
end

actor_.fade_out = function(t)
  t.menu[t.actor]:set_visible(true)
  t.menu[t.actor]:tween("Linear", "Alpha", 255, 0, config.fade_out_time, 0, flag.actor_cb())
end

actor_.shake = function(t)
  local act_x = config.act_x[t.ch]
  local act_y = config.act_y[t.ch]
  local loop  = config.act_s_loop
  local dis   = config.act_s_dis
  local dur   = config.act_s_time
  t.menu[t.actor]:set_visible(true)
  shake(loop, t.menu[t.actor], act_x, act_y, dis, dur, flag.actor_cb())
end

----

word_.size_L = function(t)
  t.menu[t.content]:set_scale( config.word_size_L )
  flag.word_is_ready()
end

word_.size_M = function(t)
  t.menu[t.content]:set_scale( config.word_size_M )
  flag.word_is_ready()
end

word_.size_S = function(t)
  t.menu[t.content]:set_scale( config.word_size_S )
  flag.word_is_ready()
end

word_.shake = function(t)
  local con_x = config.con_x[t.ch]
  local con_y = config.con_y[t.ch]
  local loop  = config.word_s_loop
  local dis   = config.word_s_dis
  local dur   = config.word_s_time
  t.menu[t.content]:set_scale( config.word_size_L )
  shake(loop, t.menu[t.content], con_x, con_y, dis, dur, flag.word_cb())
end


return {
  actor = actor_,
  word  = word_
}