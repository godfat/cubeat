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
  local leave_color = {r = self.setting.r, g = self.setting.g, b = self.setting.b}
  set_focus_leave_color(self, self.setting.focus_color, leave_color)
end

local Sprite_Based_Mt     = {__index = view.Mt_Sprite_Ex}
local SpriteText_Based_Mt = {__index = view.Mt_SpriteText_Ex}

----------------------------------------------------------------------------
-- Image
----------------------------------------------------------------------------
local function new_image(parent, setting)
  local image = setmetatable({}, Sprite_Based_Mt)
  
  -- load setting
  image.setting = { path='title', x=0, y=0, w=128, h= 128,
                    depth=-10, alpha=255, visible=true, center=false }
  load_setting(image.setting, setting)

  -- create
  image._cdata = view.new_sprite(image.setting.path, parent, image.setting.w, image.setting.h, image.setting.center)
                   
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
  local text = setmetatable({}, SpriteText_Based_Mt)
  
  -- load setting
  text.setting  = { title='new', x=0, y=0, r=255, g=255, b=255, size=24,
                    depth=-10, alpha=255, visible=true, center=false, 
                    focus_color = {r=0,g=255,b=255} }
  load_setting(text.setting, setting)

  -- create
  text._cdata = view.new_sprite_text(text.setting.title, parent, "kimberley", text.setting.size,
                                     text.setting.center, text.setting.r, text.setting.g, text.setting.b)

  -- init setting
  text:set_pos(text.setting.x, text.setting.y)
  text:set_depth(text.setting.depth)
  text:set_alpha(text.setting.alpha)
  text:set_visible(text.setting.visible)

  return text
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
