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
  image.set_alpha   = function(self, a)
                        image.pic:set_alpha(a)
                      end
  image.set_visible = function(self, visible)
                        image.pic:set_visible(visible)
                      end
  image.set_center_aligned  = function(self, align)
                                image.pic:set_center_aligned(align)
                              end

  -- init setting
  image:set_pos(image.setting.x, image.setting.y)
  image:set_depth(image.setting.depth)
  image:set_alpha(image.setting.alpha)
  image:set_visible(image.setting.visible)
  
  return image
end

----------------------------------------------------------------------------
-- Main functions
----------------------------------------------------------------------------
return{
view            = view,
puzzle          = puzzle,
new_ui_image    = new_ui_image
}