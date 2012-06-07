local ffi       = require 'ffi'
local C         = ffi.C
local view      = require 'rc/script/ui/view'
local file      = require 'rc/script/ui/file'

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

  -- create
  setmetatable(object, Sprite_Based_Mt)
  object._cdata = view.new_sprite(object.path or 'title', object.parent, object.w or 128, object.h or 128, object.center or false)
                   
  -- init setting
  object:set_pos(object.x or 0, object.y or 0)
  object:set_depth(object.depth or -10)
  object:set_alpha(object.alpha or 255)
  object:set_visible(object.visible==nil or object.visible)

  return object
end

----------------------------------------------------------------------------
-- Text
----------------------------------------------------------------------------
local function new_text(object)
  if object.parent == nil then error('parent is nil') end
  
  -- create
  setmetatable(object, SpriteText_Based_Mt)
  object._cdata = view.new_sprite_text( object.title or 'new', object.parent, "kimberley", object.size or 24,
                                        object.center or false, object.r or 255, object.g or 255, object.b or 255)

  -- init setting
  object:set_pos(object.x or 0, object.y or 0)
  object:set_depth(object.depth or -10)
  object:set_alpha(object.alpha or 255)
  object:set_visible(object.visible==nil or object.visible)

  return object
end

----------------------------------------------------------------------------
-- AskBox
----------------------------------------------------------------------------
local function new_askbox(object)
  if object.parent == nil then error('parent is nil') end

  -- create
  local screen_w  = C.Get_SCREEN_W()
  local screen_h  = C.Get_SCREEN_H()  
  setmetatable(object, Sprite_Based_Mt)
  object._cdata = view.new_sprite('cubes/cube-b-1', object.parent, screen_w, screen_h, true)
  object:set_pos(screen_w/2, screen_h/2)
  object:set_color(0, 0, 0)
  object:set_alpha(100)
  
  object.panel  = new_image{parent=object._cdata, path='area_rect',
                            w=object.w or 300, h=object.h or 200, center=true}
  
  object.text   = new_text{ parent=object.panel._cdata, title=object.title, size=object.size,
                            r=object.r, g=object.g, b=object.b, center=true }
  object.ok     = new_text{ parent=object.panel._cdata, title='ok', size=object.size,
                            r=object.r, g=object.g, b=object.b, center=true }
  object.cancel = new_text{ parent=object.panel._cdata, title='cancel', size=object.size,
                            r=object.r, g=object.g, b=object.b, center=true }
  object.text:set_pos(0, -30)
  object.text:set_depth(-10)
  object.ok:set_pos(-60, 30)
  object.ok:set_depth(-10)
  object.cancel:set_pos(60, 30)
  object.cancel:set_depth(-10)
  
  -- functions
  object.set_title        = function(self, title)
                              object.text:change_text(title)
                            end
  object.on_press_ok      = function(self, func)
                              object.ok:on_press(func)
                            end
  object.on_press_cancel  = function(self, func)
                              object.cancel:on_press(func)
                            end
  
  --init setting
  object:set_depth(object.depth or -10)
  object:set_visible(object.visible==nil or object.visible)
  
  return object
end

----------------------------------------------------------------------------
-- List
----------------------------------------------------------------------------
local function new_list(object)
  if object.parent == nil then error('parent is nil') end
  
  -- create
  object.text_name  = {}
  object.text_score = {}
  object.list = {}
  local width = 400
  local height= 600
  local screen_w  = C.Get_SCREEN_W()
  local screen_h  = C.Get_SCREEN_H()  
  setmetatable(object, Sprite_Based_Mt)
  object._cdata = view.new_sprite('area_rect', object.parent, width, height, true)
  object:set_pos(screen_w/2, screen_h/2)
  
  local score_pos_y = 60 - (height/2)
  object.score      = new_text{parent=object._cdata, title='score', x=0, y=score_pos_y, center=true, size=40}
  
  local back_pos_y  = (height/2) - 60
  object.back       = new_text{parent=object._cdata, title='back', x=0, y=back_pos_y, center=true, size=40, depth=-10}
  
  -- functions
  object.clear_list   = function(self)
                          object.text_name  = {}
                          object.text_score = {}
                          object.list = {}
                          collectgarbage('collect')
                        end
  object.set_list     = function(self, list)
                          object:clear_list()
                          object.list = list
                          local pos_y = 120 - (height/2)
                          local i = 1
                          for k,v in pairs(object.list) do
                            object.text_score[i] = new_text{parent=object._cdata, title=k,           x=-100, y=pos_y, center=true}
                            object.text_name[i]  = new_text{parent=object._cdata, title=tostring(v), x= 100, y=pos_y, center=true}
                            pos_y=pos_y+30
                            i=i+1
                          end
                        end
  object.load_list    = function(self, filename)
                          self:set_list( file.load_data(object.list, filename) )
                        end
  object.save_list    = function(self, filename)
                          file.save_data( object.list, filename )
                        end
  object.on_press_back= function(self, func)
                          object.back:on_press(func)
                        end
  
  -- init setting
  object:set_depth(object.depth or -10)
  object:set_visible(object.visible==nil or object.visible)
  
  return object
end

----------------------------------------------------------------------------
-- Checkbox
----------------------------------------------------------------------------
local function new_checkbox(object)
  if object.parent == nil then error('parent is nil') end

  -- create
  object.pressed = object.pressed or false
  local width = 256
  local height= 24
  setmetatable(object, Sprite_Based_Mt)
  object._cdata = view.new_sprite('', object.parent, width, height, object.center or false)
  
  object.box    = new_image{parent=object._cdata, path='cubes/cube1.bak', w=20, h=20, x=0, y=2}
  object.text   = new_text{parent=object._cdata, title=object.title or 'checkbox', x=40, y=-4}
  
  -- functions
  object.on_press = function(self, func)
                      local press = function(self)
                                      local path = object.pressed and 'cubes/cube1.bak' or 'cubes/cube-b-1'
                                      object.box:set_texture(path)
                                      if object.pressed then object.pressed = false
                                      else object.pressed = true end
                                      func(self)
                                    end
                      object.box:on_press(press)
                      object.text:on_press(press)
                    end
  
  -- init setting
  object:set_pos(object.x or 0, object.y or 0)
  object:set_depth(object.depth or -10)
  object:set_visible(object.visible or true)
  
  return object
end



----------------------------------------------------------------------------
-- Main functions
----------------------------------------------------------------------------
return{
view            = view,
new_image       = new_image,
new_text        = new_text,
new_askbox      = new_askbox,
new_list        = new_list,
new_checkbox    = new_checkbox
}
