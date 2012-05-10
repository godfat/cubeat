local ffi       = require 'ffi'
local C         = ffi.C
local view      = require 'rc/script/ui/view'
local puzzle    = require 'rc/script/ui/puzzle/view'

local function load_setting(ui_setting, setting)
  for k,v in pairs(setting) do
    local no_key = (ui_setting[k] == nil)
    if no_key == false then
      ui_setting[k] = v
    end
  end
end

----------------------------------------------------------------------------
-- Image
----------------------------------------------------------------------------
local function new_ui_image(parent, setting)
  local image = {}
  
  -- load setting
  image.setting = { path='title', x=0, y=0, w=128, h= 128,
                    depth=-10, alpha=255, visible=true, center=false }
  load_setting(image.setting, setting)
  
  -- create
  image.pic = view.new_sprite(image.setting.path, parent, image.setting.w, image.setting.h, image.setting.center)
  
  -- functions
  image.set_texture = function(self, path)
                        image.pic:set_texture(path)
                      end
  image.set_pos     = function(self, x, y)
                        image.pic:set_pos(x, y)
                      end
  image.set_size    = function(self, w, h)
                        image.pic:set_size(w, h)
                      end
  image.set_depth   = function(self, depth)
                        image.pic:set_depth(depth)
                      end
  image.set_alpha   = function(self, alpha)
                        image.pic:set_alpha(alpha)
                      end
  image.set_visible = function(self, visible)
                        image.pic:set_visible(visible)
                      end
  image.set_center_aligned  = function(self, center)
                                image.pic:set_center_aligned(center)
                              end
  image.set_fade    = function(self, alpha)
                        image:set_visible(true)
                        if alpha == 0 then
                          local cb = function(self) image:set_visible(false) end
                          image.pic:on_tween_line_alpha(alpha, 500, 0, cb, 0)
                        else
                          local cb = function(self) end
                          image.pic:on_tween_line_alpha(alpha, 500, 0, cb, 0)
                        end
                      end

  -- init setting
  image:set_pos(image.setting.x, image.setting.y)
  image:set_depth(image.setting.depth)
  image:set_alpha(image.setting.alpha)
  image:set_visible(image.setting.visible)
  
  return image
end

----------------------------------------------------------------------------
-- Text
----------------------------------------------------------------------------
local function new_ui_text(parent, setting)
  local Text = {}

  -- load setting
  Text.setting  = { title='new', x=0, y=0, r=255, g=255, b=255,
                    depth=-10, alpha=255, visible=true, center=false }
  load_setting(Text.setting, setting)
  
  -- create
  Text.title = view.new_sprite_text(Text.setting.title, parent, "Star Jedi", 24, Text.setting.center,
                                    Text.setting.r, Text.setting.g, Text.setting.b)
  
  -- functions
  Text.set_pos    = function(self, x, y)
                      Text.title:set_pos(x, y)
                    end
  Text.set_depth  = function(self, depth)
                      Text.title:set_depth(depth)
                    end
  Text.set_alpha  = function(self, alpha)
                      Text.title:set_alpha(alpha)
                    end
  Text.set_visible= function(self, visible)
                      Text.title:set_visible(visible)
                    end
  Text.set_center_aligned = function(self, center)
                              Text.title:set_center_aligned(center)
                            end
  Text.set_fade   = function(self, alpha)
                      Text:set_visible(true)
                      if alpha == 0 then
                        local cb = function(self) Text:set_visible(false) end
                        Text.title:on_tween_line_alpha(alpha, 500, 0, cb, 0)
                      else
                        local cb = function(self) end
                        Text.title:on_tween_line_alpha(alpha, 500, 0, cb, 0)
                      end
                    end
  
  -- init setting
  Text:set_pos(Text.setting.x, Text.setting.y)
  Text:set_depth(Text.setting.depth)
  Text:set_alpha(Text.setting.alpha)
  Text:set_visible(Text.setting.visible)
  
  return Text
end

----------------------------------------------------------------------------
-- Main functions
----------------------------------------------------------------------------
return{
view            = view,
puzzle          = puzzle,
new_ui_image    = new_ui_image,
new_ui_text     = new_ui_text
}