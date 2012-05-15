local ffi       = require 'ffi'
local C         = ffi.C
local view      = require 'rc/script/ui/view'

local function load_setting(ui_setting, setting)
  for k,v in pairs(setting) do
    local no_key = (ui_setting[k] == nil)
    if no_key == false then
      ui_setting[k] = v
    end
  end
end

local Input1      = C.Input_get_input1()
local Input2      = C.Input_get_input2()
local Input1_left = C.Input_get_trig1(C.Input_get_input1())
local Input2_left = C.Input_get_trig1(C.Input_get_input2())

local function set_on_press_callback(sprite, func)
  sprite:on_press( Input1_left, func )
  sprite:on_press( Input2_left, func )
end

local function set_focus_leave_color(sprite, focus_color, leave_color)
  local sprite_focus =  function(self) sprite:set_color(focus_color.r, focus_color.g, focus_color.b) end
  local sprite_leave =  function(self) sprite:set_color(leave_color.r, leave_color.g, leave_color.b) end
  sprite:on_enter_focus(Input1, sprite_focus)
  sprite:on_leave_focus(Input1, sprite_leave)
  sprite:on_enter_focus(Input2, sprite_focus)
  sprite:on_leave_focus(Input2, sprite_leave)
end

----------------------------------------------------------------------------
-- Image
----------------------------------------------------------------------------
local function new_image(parent, setting)
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
local function new_text(parent, setting)
  local text = {}

  -- load setting
  text.setting  = { title='new', x=0, y=0, r=255, g=255, b=255, size=24,
                    depth=-10, alpha=255, visible=true, center=false }
  load_setting(text.setting, setting)
  
  -- create
  text.title = view.new_sprite_text(text.setting.title, parent, "Star Jedi", text.setting.size,
                                    text.setting.center, text.setting.r, text.setting.g, text.setting.b)
  
  -- functions
  text.set_pos    = function(self, x, y)
                      text.title:set_pos(x, y)
                    end
  text.set_depth  = function(self, depth)
                      text.title:set_depth(depth)
                    end
  text.set_alpha  = function(self, alpha)
                      text.title:set_alpha(alpha)
                    end
  text.set_visible= function(self, visible)
                      text.title:set_visible(visible)
                    end
  text.set_center_aligned = function(self, center)
                              text.title:set_center_aligned(center)
                            end
  text.set_fade   = function(self, alpha)
                      text:set_visible(true)
                      if alpha == 0 then
                        local cb = function(self) text:set_visible(false) end
                        text.title:on_tween_line_alpha(alpha, 500, 0, cb, 0)
                      else
                        local cb = function(self) end
                        text.title:on_tween_line_alpha(alpha, 500, 0, cb, 0)
                      end
                    end
  
  -- init setting
  text:set_pos(text.setting.x, text.setting.y)
  text:set_depth(text.setting.depth)
  text:set_alpha(text.setting.alpha)
  text:set_visible(text.setting.visible)
  
  return text
end

----------------------------------------------------------------------------
-- Button
----------------------------------------------------------------------------
local function new_button(parent, setting)
  local button = {}
  
  -- load setting
  button.setting  = { title='button', x=0, y=0, r=255, g=255, b=255, size=24,
                      depth=-10, alpha=255, visible=true, center=false, focus_color={r=0,g=255,b=255} }
  load_setting(button.setting, setting)
  
  -- create
  button.title = view.new_sprite_text(button.setting.title, parent, "Star Jedi", button.setting.size,
                                      button.setting.center, button.setting.r, button.setting.g, button.setting.b)
  
  -- functions
  button.set_pos    = function(self, x, y)
                        button.title:set_pos(x, y)
                      end
  button.set_depth  = function(self, depth)
                        button.title:set_depth(depth)
                      end
  button.set_alpha  = function(self, alpha)
                        button.title:set_alpha(alpha)
                      end
  button.set_visible= function(self, visible)
                        button.title:set_visible(visible)
                      end
  button.set_center_aligned = function(self, center)
                                button.title:set_center_aligned(center)
                              end
  button.set_fade   = function(self, alpha)
                        button:set_visible(true)
                        if alpha == 0 then
                          local cb = function(self) button:set_visible(false) end
                          button.title:on_tween_line_alpha(alpha, 500, 0, cb, 0)
                        else
                          local cb = function(self) end
                          button.title:on_tween_line_alpha(alpha, 500, 0, cb, 0)
                        end
                      end
  button.on_press   = function(self, func)
                        set_on_press_callback(button.title, func)
                      end

  -- init setting
  local leave_color = {r=button.setting.r, g=button.setting.g, b=button.setting.b}
  set_focus_leave_color(button.title, button.setting.focus_color, leave_color)
  button:set_pos(button.setting.x, button.setting.y)
  button:set_depth(button.setting.depth)
  button:set_alpha(button.setting.alpha)
  button:set_visible(button.setting.visible)
  
  return button
end

----------------------------------------------------------------------------
-- Ratio
----------------------------------------------------------------------------
local function new_ui_ratio(parent, setting)
  local ratio = {}
  
  -- load setting
  
  -- create
  ratio.icon  = new_button(parent, setting)
  ratio.title = new_text(parent, setting)
  
  -- functions
  
  -- init setting
  
  return ratio
end

----------------------------------------------------------------------------
-- Main functions
----------------------------------------------------------------------------
return{
new_image    = new_image,
new_text     = new_text,
new_button   = new_button,
new_ui_ratio    = new_ui_ratio
}