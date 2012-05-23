local ffi       = require 'ffi'
local C         = ffi.C
local view      = require 'rc/script/ui/view'

local function check_parent(object)
  if object.parent == nil then
    print('error - parent is nil')
  end
  return object.parent
end

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
--local function new_image(parent, setting)
local function new_image(object)
  local image = {}
  
  local check = check_parent(object)
  if check == nil then return end
  --[[
  -- load setting
  image.setting = { path='title', x=0, y=0, w=128, h= 128,
                    depth=-10, alpha=255, visible=true, center=false }
  load_setting(image.setting, setting)
  --]]
  -- create
  --image.pic = view.new_sprite(image.setting.path, parent, image.setting.w, image.setting.h, image.setting.center)
  object.pic = view.new_sprite(object.path or 'title', object.parent, object.w or 0, object.h or 0, object.center or false)

  -- functions
  object.set_texture = function(self, path)
                        object.pic:set_texture(path)
                      end
  object.set_pos     = function(self, x, y)
                        object.pic:set_pos(x, y)
                      end
  object.set_size    = function(self, w, h)
                        object.pic:set_size(w, h)
                      end
  object.set_depth   = function(self, depth)
                        object.pic:set_depth(depth)
                      end
  object.set_alpha   = function(self, alpha)
                        object.pic:set_alpha(alpha)
                      end
  object.set_visible = function(self, visible)
                        object.pic:set_visible(visible)
                      end
  object.set_center_aligned  = function(self, center)
                                object.pic:set_center_aligned(center)
                              end
  object.set_fade    = function(self, alpha)
                        object:set_visible(true)
                        if alpha == 0 then
                          local cb = function(self) object:set_visible(false) end
                          object.pic:on_tween_line_alpha(alpha, 500, 0, cb, 0)
                        else
                          local cb = function(self) end
                          object.pic:on_tween_line_alpha(alpha, 500, 0, cb, 0)
                        end
                      end
  object.on_press   = function(self, func)
                       set_on_press_callback(object.pic, func)
                     end

  -- init setting
  object:set_pos(object.x or 0, object.y or 0)
  object:set_depth(object.depth or -10)
  object:set_alpha(object.alpha or 255)
  object:set_visible(object.visible or true)

  return object
end

----------------------------------------------------------------------------
-- Text
----------------------------------------------------------------------------
--local function new_text(parent, setting)
local function new_text(object)
  local text = {}
  
  local check = check_parent(object)
  if check == nil then return end

  --[[
  -- load setting
  text.setting  = { title='new', x=0, y=0, r=255, g=255, b=255, size=24,
                    depth=-10, alpha=255, visible=true, center=false }
  load_setting(text.setting, setting)
  --]]
  
  -- create
  --text.title = view.new_sprite_text(text.setting.title, parent, "kimberley", text.setting.size,
                                    --text.setting.center, text.setting.r, text.setting.g, text.setting.b)
  object.title = view.new_sprite_text(object.title or 'new', object.parent, "kimberley", object.size or 24,
                                    object.center or false, object.r or 255, object.g or 255, object.b or 255)

  -- functions
  object.set_pos    = function(self, x, y)
                      object.title:set_pos(x, y)
                    end
  object.set_depth  = function(self, depth)
                      object.title:set_depth(depth)
                    end
  object.set_alpha  = function(self, alpha)
                      object.title:set_alpha(alpha)
                    end
  object.set_visible= function(self, visible)
                      object.title:set_visible(visible)
                    end
  object.set_center_aligned = function(self, center)
                              object.title:set_center_aligned(center)
                            end
  object.set_fade   = function(self, alpha)
                      object:set_visible(true)
                      if alpha == 0 then
                        local cb = function(self) object:set_visible(false) end
                        object.title:on_tween_line_alpha(alpha, 500, 0, cb, 0)
                      else
                        local cb = function(self) end
                        object.title:on_tween_line_alpha(alpha, 500, 0, cb, 0)
                      end
                    end

  -- init setting
  object:set_pos(object.x or 0, object.y or 0)
  object:set_depth(object.depth or -10)
  object:set_alpha(object.alpha or 255)
  object:set_visible(object.visible or true)

  return object
end

----------------------------------------------------------------------------
-- Button
----------------------------------------------------------------------------
--local function new_button(parent, setting)
local function new_button(object)
  local button = {}
  
  local check = check_parent(object)
  if check == nil then return end
  
  --[[
  -- load setting
  button.setting  = { title='button', x=0, y=0, r=255, g=255, b=255, size=24,
                      depth=-10, alpha=255, visible=true, center=false, focus_color={r=0,g=255,b=255} }
  load_setting(button.setting, setting)
  --]]
  -- create
  --button.title = view.new_sprite_text(button.setting.title, parent, "kimberley", button.setting.size,
                                      --button.setting.center, button.setting.r, button.setting.g, button.setting.b)
  object.title = view.new_sprite_text(object.title or 'button', object.parent, "kimberley", object.size or 24,
                                      object.center or false, object.r or 255, object.g or 255, object.b or 255)

  -- functions
  object.set_pos    = function(self, x, y)
                        object.title:set_pos(x, y)
                      end
  object.set_depth  = function(self, depth)
                        object.title:set_depth(depth)
                      end
  object.set_alpha  = function(self, alpha)
                        object.title:set_alpha(alpha)
                      end
  object.set_visible= function(self, visible)
                        object.title:set_visible(visible)
                      end
  object.set_center_aligned = function(self, center)
                                object.title:set_center_aligned(center)
                              end
  object.set_fade   = function(self, alpha)
                        object:set_visible(true)
                        if alpha == 0 then
                          local cb = function(self) object:set_visible(false) end
                          object.title:on_tween_line_alpha(alpha, 500, 0, cb, 0)
                        else
                          local cb = function(self) end
                          object.title:on_tween_line_alpha(alpha, 500, 0, cb, 0)
                        end
                      end
  object.on_press   = function(self, func)
                        set_on_press_callback(object.title, func)
                      end

  -- init setting
  local leave_color = {r=object.r or 255, g=object.g or 255, b=object.b or 255}
  set_focus_leave_color(object.title or 'button', object.focus_color or {r=0,g=255,b=255}, leave_color)
  object:set_pos(object.x or 0, object.y or 0)
  object:set_depth(object.depth or -10)
  object:set_alpha(object.alpha or 255)
  object:set_visible(object.visible or true)

  return object
end

----------------------------------------------------------------------------
-- AskBox
----------------------------------------------------------------------------
local function new_askbox(parent, setting)
  local askbox = {}
  
  -- load setting
  askbox.setting  = { title='new', w=300, h=200, r=255, g=255, b=255, size=24, depth=-100,
                      focus_color={r=0,g=255,b=255} }
  load_setting(askbox.setting, setting)
  
  -- create
  local screen_w  = C.Get_SCREEN_W()
  local screen_h  = C.Get_SCREEN_H()
  askbox.filter   = view.new_sprite('cubes/cube-b-1', parent, screen_w, screen_h, true)
  askbox.filter:set_pos(screen_w/2, screen_h/2)
  askbox.filter:set_color(0, 0, 0)
  askbox.filter:set_alpha(100)
  
  askbox.panel  = view.new_sprite('area_rect', askbox.filter, askbox.setting.w, askbox.setting.h, true)
  
  askbox.title  = view.new_sprite_text( askbox.setting.title, askbox.panel, "Star Jedi",
                                        askbox.setting.size, true,
                                        askbox.setting.r, askbox.setting.g, askbox.setting.b )
  askbox.ok     = view.new_sprite_text( 'ok', askbox.panel, "Star Jedi", askbox.setting.size, true,
                                        askbox.setting.r, askbox.setting.g, askbox.setting.b )
  askbox.cancel = view.new_sprite_text( 'cancel', askbox.panel, "Star Jedi", askbox.setting.size, true,
                                        askbox.setting.r, askbox.setting.g, askbox.setting.b )
  askbox.title:set_pos(0, -30)
  askbox.title:set_depth(-10)
  askbox.ok:set_pos(-60, 30)
  askbox.ok:set_depth(-10)
  askbox.cancel:set_pos(60, 30)
  askbox.cancel:set_depth(-10)
  
  -- functions
  askbox.set_title        = function(self, title)
                              askbox.title:change_text(title)
                            end
  askbox.set_size         = function(self, w, h)
                              askbox.panel:set_size(w, h)
                            end
  askbox.set_depth        = function(self, depth)
                              askbox.filter:set_depth(depth)
                            end
  askbox.set_visible      = function(self, visible)
                              askbox.filter:set_visible(visible)
                              askbox.panel:set_visible(visible)
                              askbox.title:set_visible(visible)
                              askbox.ok:set_visible(visible)
                              askbox.cancel:set_visible(visible)
                            end
  askbox.on_press_ok      = function(self, func)
                              set_on_press_callback(askbox.ok, func)
                            end
  askbox.on_press_cancel  = function(self, func)
                              set_on_press_callback(askbox.cancel, func)
                            end
  
  -- init setting
  local leave_color = {r=askbox.setting.r, g=askbox.setting.g, b=askbox.setting.b}
  set_focus_leave_color(askbox.ok, askbox.setting.focus_color, leave_color)
  set_focus_leave_color(askbox.cancel, askbox.setting.focus_color, leave_color)
  askbox:set_depth(askbox.setting.depth)
  
  return askbox
end

----------------------------------------------------------------------------
-- Main functions
----------------------------------------------------------------------------
return{
view            = view,
puzzle          = puzzle,
new_image       = new_image,
new_text        = new_text,
new_button      = new_button,
new_askbox      = new_askbox
}
