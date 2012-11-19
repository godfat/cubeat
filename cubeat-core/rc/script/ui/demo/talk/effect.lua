local ffi     = require 'ffi'
local C       = ffi.C
local view    = require 'rc/script/ui/view'
local config  = require 'rc/script/ui/demo/talk/config'
local ui      = require 'rc/script/ui/ui'


local actor_  = {}
local word_   = {}
local special_= {}

local image_tb  = {}
local text_tb   = {}


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

----

special_.cube = function(object, cb, special)
  local ch    = object.ch
  local act_x = config.act_x[ch]
  local act_y = config.act_y[ch]
  local dis   = config.act_s_dis
  local dur   = config.act_s_time

  local cube = ui.new_image{ parent=object.actor._cdata, path=special.path or config.cube_path,
                             x=special.x1 or config.cube_x1, y=special.y1 or config.cube_y1,
                             w=special.w or config.cube_w, h=special.h or config.cube_h }
  
  local function hit_end(self)
    cube:set_visible(false)
    cb()
  end
  local function hit_shake(self)
    shake(2, object.actor, act_x, act_y, dis, dur)
    cube:tween( "Linear", "Pos2D",
                ffi.new("v2", special.x2 or config.cube_x2, special.y2 or config.cube_y2),
                ffi.new("v2", special.x3 or config.cube_x3, special.y3 or config.cube_y3),
                special.dur or config.cube_time, 0, hit_end )
  end
  cube:tween( "Linear", "Pos2D",
              ffi.new("v2", special.x1 or config.cube_x1, special.y1 or config.cube_y1),
              ffi.new("v2", special.x2 or config.cube_x2, special.y2 or config.cube_y2),
              special.dur or config.cube_time, 0, hit_shake)
end

special_.add_image = function(object, cb, special)
  if special.name then
    image_tb[special.name]  = ui.new_image{ parent=object.actor._cdata, path=special.path,
                                            x=special.x or 0, y=special.y or 0,
                                            w=special.w or 64, h=special.h or 64 }
  end
  
  cb()
end

special_.hide_image = function(object, cb, special)
  if special.name then
    image_tb[special.name]:set_visible(false)
  else
    for k,v in pairs(image_tb) do
      v:set_visible(false)
    end
  end
  
  cb()
end

special_.add_text = function(object, cb, special)
  if special.name then
    text_tb[special.name] = ui.new_text{parent=object.actor._cdata, title=special.name,
                                        x=special.x or 0, y=special.y or 0,
                                        r=special.r or 0, g=special.g or 0, b=special.b or 0}
  end
  
  cb()
end

special_.hide_text = function(object, cb, special)
  if special.name then
    text_tb[special.name]:set_visible(false)
  else
    for k,v in pairs(text_tb) do
      v:set_visible(false)
    end
  end
  
  cb()
end


--
local function actor_effect(effect_a, actor, content, panel, ch, cb)
  if effect_a==nil then return end
  local object = {actor=actor, content=content, panel=panel, ch=ch}
  actor_[effect_a](object, cb)
end
local function word_effect(effect_w, actor, content, panel, ch, cb)
  if effect_w==nil then return end
  local object = {actor=actor, content=content, panel=panel, ch=ch}
  word_[effect_w](object, cb)
end
local function special_effect(special_id, actor, content, panel, ch, cb, special)
  if special_id==nil then return end
  local object = {actor=actor, content=content, panel=panel, ch=ch}
  special_[special_id](object, cb, special)
end


return {
  actor_effect  = actor_effect,
  word_effect   = word_effect,
  special_effect= special_effect
}