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

local function set_focus_leave_color(obj, focus_color, leave_color)
  local focus_f = function(self) obj:set_color(focus_color.r, focus_color.g, focus_color.b) end
  local leave_f = function(self) obj:set_color(leave_color.r, leave_color.g, leave_color.b) end
  obj:on_enter_focus(Input1, focus_f)
  obj:on_leave_focus(Input1, leave_f)
  obj:on_enter_focus(Input2, focus_f)
  obj:on_leave_focus(Input2, leave_f)
end

----------------------------------------------------------------------------
-- Metatable supplementals
----------------------------------------------------------------------------

view.Mt_Sprite_Ex.on_press = function(self, func)
  set_on_press_callback(self._cdata, func)
end

view.Mt_SpriteText_Ex.on_press = function(self, func)
  set_on_press_callback(self._cdata, func)
  local leave_color = {r = self.r or 255, g = self.g or 255, b = self.b or 255}
  set_focus_leave_color(self, self.focus_color or {r=0, g=255, b=255}, leave_color)
end

local Sprite_Based_Mt     = {__index = view.Mt_Sprite_Ex}
local SpriteText_Based_Mt = {__index = view.Mt_SpriteText_Ex}

----------------------------------------------------------------------------
-- Image
----------------------------------------------------------------------------
local function new_image(object)
  if object.parent == nil then error('parent is nil') end

  setmetatable(object, Sprite_Based_Mt)

  object._cdata = view.new_sprite(object.path or 'title', object.parent, object.w or 128, object.h or 128, object.center or false)
                   
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
local function new_text(object)
  if object.parent == nil then error('parent is nil') end
  
  setmetatable(object, SpriteText_Based_Mt)

  object._cdata = view.new_sprite_text( object.title or 'new', object.parent, "kimberley", object.size or 24,
                                        object.center or false, object.r or 255, object.g or 255, object.b or 255)

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
local function new_button(parent, setting)
  local button = {}
  
  -- load setting
  button.setting  = { title='button', x=0, y=0, r=255, g=255, b=255, size=24,
                      depth=-10, alpha=255, visible=true, center=false, focus_color={r=0,g=255,b=255} }
  load_setting(button.setting, setting)
  
  -- create
  button.title = view.new_sprite_text(button.setting.title, parent, "kimberley", button.setting.size,
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
  set_focus_leave_color(button.setting.title, button.setting.focus_color, leave_color)
  button:set_pos(button.setting.x, button.setting.y)
  button:set_depth(button.setting.depth)
  button:set_alpha(button.setting.alpha)
  button:set_visible(button.setting.visible)

  return button
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
