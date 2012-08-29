local ffi     = require 'ffi'
local C       = ffi.C
local view    = require 'rc/script/ui/view'
local config  = require 'rc/script/ui/demo/talk/config'


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

actor_.show = function(object, cb)
  object.actor:set_visible(true)
  cb()
end

actor_.hide = function(object, cb)
  object.actor:set_visible(false)
  cb()
end

actor_.slide_in = function(object, cb)
  local ch    = object.ch
  local act_x = config.act_x[ch]
  local act_y = config.act_y[ch]
  local out_x = config.out_x[ch]
  local out_y = config.out_y[ch]
  object.actor:set_visible(true)
  object.actor:tween( "Linear", "Pos2D", ffi.new("value2", out_x, out_y), ffi.new("value2", act_x, act_y),
                      config.slide_in_time, 0, cb )
end

actor_.slide_out = function(object, cb)
  local ch    = object.ch
  local act_x = config.act_x[ch]
  local act_y = config.act_y[ch]
  local out_x = config.out_x[ch]
  local out_y = config.out_y[ch]
  object.actor:set_visible(true)
  object.actor:tween( "Linear", "Pos2D", ffi.new("value2", act_x, act_y), ffi.new("value2", out_x, out_y),
                      config.slide_out_time, 0, cb )
end

actor_.fade_in = function(object, cb)
  object.actor:set_visible(true)
  object.actor:tween("Linear", "Alpha", 0, 255, config.fade_in_time, 0, cb)
end

actor_.fade_out = function(object, cb)
  object.actor:set_visible(true)
  object.actor:tween("Linear", "Alpha", 255, 0, config.fade_out_time, 0, cb)
end

actor_.shake = function(object, cb)
  local ch    = object.ch
  local act_x = config.act_x[ch]
  local act_y = config.act_y[ch]
  local loop  = config.act_s_loop
  local dis   = config.act_s_dis
  local dur   = config.act_s_time
  object.actor:set_visible(true)
  shake(loop, object.actor, act_x, act_y, dis, dur, cb)
end

----

word_.size_L = function(object, cb)
  object.content:set_scale( config.word_size_L )
  cb()
end

word_.size_M = function(object, cb)
  object.content:set_scale( config.word_size_M )
  cb()
end

word_.size_S = function(object, cb)
  object.content:set_scale( config.word_size_S )
  cb()
end

word_.shake = function(object, cb)
  local con_x = object.panel:get_pos_x()+config.con_offset_x
  local con_y = object.panel:get_pos_y()+config.con_offset_y
  local loop  = config.word_s_loop
  local dis   = config.word_s_dis
  local dur   = config.word_s_time
  object.content:set_scale( config.word_size_L )
  shake(loop, object.content, con_x, con_y, dis, dur, cb)
end



--
local function actor_effect(effect, actor, content, panel, ch, cb)
  local object = {actor=actor, content=content, panel=panel, ch=ch}
  actor_[effect](object, cb)
end
local function word_effect(effect, actor, content, panel, ch, cb)
  local object = {actor=actor, content=content, panel=panel, ch=ch}
  word_[effect](object, cb)
end


return {
  actor_effect  = actor_effect,
  word_effect   = word_effect
}