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
local Input1_right= C.Input_get_trig2(C.Input_get_input1())
local Input2_right= C.Input_get_trig2(C.Input_get_input2())

local function set_on_press_callback(sprite, func, input)
  if input == nil then
    sprite:on_press( Input1_left, func )
    sprite:on_press( Input2_left, func )
  else
    sprite:on_press( input, func )
  end
end

local function set_on_press_callback_r(sprite, func, input)
  if input == nil then
    sprite:on_press( Input1_right, func )
    sprite:on_press( Input2_right, func )
  else
    sprite:on_press( input, func )
  end
end

local function set_on_down_callback(sprite, func, input)
  if input == nil then
    sprite:on_down( Input1_left, func )
    sprite:on_down( Input2_left, func )
  else
    sprite:on_down( input, func )
  end
end

local function set_on_leave_focus_callback(sprite, func, input)
  if input == nil then
    sprite:on_leave_focus( Input1, func )
    sprite:on_leave_focus( Input2, func )
  else
    sprite:on_leave_focus( input, func )
  end
end

local function set_on_enter_focus_callback(sprite, func, input)
  if input == nil then
    sprite:on_enter_focus( Input1, func )
    sprite:on_enter_focus( Input2, func )
  else
    sprite:on_enter_focus( input, func )
  end
end

local function set_focus_leave_pic(obj, focus_pic, leave_pic, input)
  local focus_f = function(self) self:set_texture(focus_pic) end
  local leave_f = function(self) self:set_texture(leave_pic) end

  if input == nil then
    obj:on_enter_focus(Input1, focus_f)
    obj:on_leave_focus(Input1, leave_f)
    obj:on_enter_focus(Input2, focus_f)
    obj:on_leave_focus(Input2, leave_f)
  else
    obj:on_enter_focus(input, focus_f)
    obj:on_leave_focus(input, leave_f)
  end
end

local function set_focus_leave_color(obj, focus_color, leave_color, input)
  local focus_f = function(self) ffi.cast("pSpriteText*", self):set_color(focus_color.r, focus_color.g, focus_color.b) end
  local leave_f = function(self) ffi.cast("pSpriteText*", self):set_color(leave_color.r, leave_color.g, leave_color.b) end

  if input == nil then
    obj:on_enter_focus(Input1, focus_f)
    obj:on_leave_focus(Input1, leave_f)
    obj:on_enter_focus(Input2, focus_f)
    obj:on_leave_focus(Input2, leave_f)
  else
    obj:on_enter_focus(input, focus_f)
    obj:on_leave_focus(input, leave_f)
  end
end


----------------------------------------------------------------------------
-- Metatable supplementals
----------------------------------------------------------------------------

view.Mt_Sprite_Ex.on_press = function(self, func, input)
  set_on_press_callback(self._cdata, func, input)
end

view.Mt_SpriteText_Ex.on_press = function(self, func, input)
  set_on_press_callback(self._cdata, func, input)
  local leave_color = {r = self.r or 255, g = self.g or 255, b = self.b or 255}
  
  if input==Input1_left or input==Input1_right then
    set_focus_leave_color(self._cdata, self.focus_color or {r=0, g=255, b=255}, leave_color, Input1)
  elseif input==Input2_left or input==Input2_right then
    set_focus_leave_color(self._cdata, self.focus_color or {r=0, g=255, b=255}, leave_color, Input2)
  else
    set_focus_leave_color(self._cdata, self.focus_color or {r=0, g=255, b=255}, leave_color)
  end
  
  --set_focus_leave_color(self._cdata, self.focus_color or {r=0, g=255, b=255}, leave_color, input)
end

view.Mt_Sprite_Ex.on_press_r = function(self, func, input)
  set_on_press_callback_r(self._cdata, func, input)
end

view.Mt_Sprite_Ex.on_down = function(self, func, input)
  set_on_down_callback(self._cdata, func, input)
end

view.Mt_SpriteText_Ex.on_down = function(self, func, input)
  set_on_down_callback(self._cdata, func, input)
  local leave_color = {r = self.r or 255, g = self.g or 255, b = self.b or 255}
  
  if input==Input1_left or input==Input1_right then
    set_focus_leave_color(self._cdata, self.focus_color or {r=0, g=255, b=255}, leave_color, Input1)
  elseif input==Input2_left or input==Input2_right then
    set_focus_leave_color(self._cdata, self.focus_color or {r=0, g=255, b=255}, leave_color, Input2)
  else
    set_focus_leave_color(self._cdata, self.focus_color or {r=0, g=255, b=255}, leave_color)
  end
  
  --set_focus_leave_color(self._cdata, self.focus_color or {r=0, g=255, b=255}, leave_color, input)
end

view.Mt_Sprite_Ex.on_leave_focus = function(self, func, input)
  set_on_leave_focus_callback(self._cdata, func, input)
end

view.Mt_Sprite_Ex.on_enter_focus = function(self, func, input)
  set_on_enter_focus_callback(self._cdata, func, input)
end

view.Mt_SpriteText_Ex.on_leave_focus = function(self, func, input)
  set_on_leave_focus_callback(self._cdata, func, input)
end

view.Mt_SpriteText_Ex.on_enter_focus = function(self, func, input)
  set_on_enter_focus_callback(self._cdata, func, input)
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
  object._cdata = view.new_sprite_text( object.title or 'new', object.parent, object.font or "kimberley", object.size or 24,
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
  object._cdata = view.new_sprite('blocker', object.parent, screen_w, screen_h, true)
  object:set_pos(screen_w/2, screen_h/2)
  object:set_color(0, 0, 0)
  object:set_alpha(0)
  
  object.panel  = new_image{parent=object._cdata, path='nothing',
                            w=object.w or 600, h=object.h or 300, center=true}
  object.panel:set_color(0, 0, 0)
  object.panel:set_alpha(192)
  
  object.text   = new_text{ parent=object._cdata, title=object.title, size=object.size or 30,
                            r=object.r, g=object.g, b=object.b, center=true }
  object.ok     = new_text{ parent=object._cdata, title='OK', size=object.size or 30,
                            r=object.r, g=object.g, b=object.b, center=true }
  object.cancel = new_text{ parent=object._cdata, title='Cancel', size=object.size or 30,
                            r=object.r, g=object.g, b=object.b, center=true }
  object.text:set_pos(0, -45)
  object.text:set_depth(-50)
  object.ok:set_scale(1.5)
  object.ok:set_pos(-75, 50)
  object.ok:set_depth(-50)
  object.cancel:set_scale(1.5)
  object.cancel:set_pos(75, 50)
  object.cancel:set_depth(-50)
  
  object.ok:    on_press( function(self)
                            if object.cb and object.cb[1] then object.cb[1](self) end
                            object:set_visible(false)
                          end )
  object.cancel:on_press( function(self)
                            if object.cb and object.cb[2] then object.cb[2](self) end
                            object:set_visible(false)
                          end )
  
  if object.cb and table.getn(object.cb)==1 then
    object.ok:set_pos(0, 50)
    object.cancel:set_visible(false)
  end
  
  -- functions
  object.set_title        = function(self, title)
                              object.text:change_text(title)
                            end
  object.on_press_ok      = function(self, func)
                              local cb = function(self) func(self) object:set_visible(false) end
                              object.ok:on_press(cb)
                            end
  object.on_press_cancel  = function(self, func)
                              local cb = function(self) func(self) object:set_visible(false) end
                              object.cancel:on_press(cb)
                            end
  
  -- init setting
  object.text:set_scale(object.scale or 1)
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
  object:set_pos(object.x or screen_w/2, object.y or screen_h/2)
  
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
                          self:set_list( file.load_data(filename) or object.list )
                        end
  object.save_list    = function(self, filename)
                          file.save_data(filename, object.list)
                        end
  object.on_press_back= function(self, func)
                          object.back:on_press(func)
                        end
  object.remove_cb    = function(self)
                          object.score:remove()
                          object.back:remove()
                        end
  
  -- init setting
  object:set_depth(object.depth or -10)
  object:set_visible(object.visible==nil or object.visible)
  
  return object
end

----------------------------------------------------------------------------
-- Ratio
----------------------------------------------------------------------------
local function new_ratio(object)
  if object.parent == nil then error('parent is nil') end

  -- create
  object.pressed = object.pressed or false
  local width = 256
  local height= 24
  setmetatable(object, Sprite_Based_Mt)
  object._cdata = view.new_sprite('', object.parent, width, height, object.center or false)
  
  object.box    = new_image{parent=object._cdata, path='cubes/cube1.bak', w=20, h=20, x=0, y=2}
  
  object.text   = new_text{parent=object._cdata, title=object.title or 'ratio', x=40, y=-4}
  
  -- functions
  object.on_press = function(self, func)
                      local press = function(self)
                                      local path = object.pressed and 'cubes/cube1.bak' or 'cubes/cube-b-1'
                                      object.box:set_texture(path)
                                      object.pressed = (object.pressed==false and true) or false
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
-- SelectBox
----------------------------------------------------------------------------
local function new_selectbox(object)
  if object.parent == nil then error('parent is nil') end
  
  -- create
  object.list   = object.list or {'select1', 'select2', 'select3'}
  object.index  = object.index or 1
  local width   = 256
  local height  = 24
  setmetatable(object, Sprite_Based_Mt)
  object._cdata = view.new_sprite('', object.parent, width, height, object.center or false)
  
  object.left   = new_image{parent=object._cdata, path='cubes/cube1.bak', w=20, h=20, x=0, y=2}
  object.right  = new_image{parent=object._cdata, path='cubes/cube1.bak', w=20, h=20, x=width-20, y=2}
  object.text   = new_text{parent=object._cdata, title=object.list[object.index], x=128, y=12, center=true}
  
  -- functions
  object.add_index  = function(self, v)
                        object.index = object.index + v
                        if object.index>table.getn(object.list) then object.index=1 end
                        if object.index<1 then object.index = table.getn(object.list) end
                        object.text:change_text(object.list[object.index])
                      end
  
  -- init setting
  set_focus_leave_pic(object.left._cdata,   'cubes/cube-b-1', 'cubes/cube1.bak')
  set_focus_leave_pic(object.right._cdata,  'cubes/cube-b-1', 'cubes/cube1.bak')
  local press_left  = function(self) object:add_index(-1) end
  local press_right = function(self) object:add_index(1) end
  object.left:on_press(press_left)
  object.right:on_press(press_right)
  
  object:set_pos(object.x or 0, object.y or 0)
  object:set_depth(object.depth or -10)
  object:set_visible(object.visible or true)
  
  return object
end

----------------------------------------------------------------------------
-- ScrollBar
----------------------------------------------------------------------------
local function new_scrollbar(object)
  if object.parent == nil then error('parent is nil') end
  
  -- create
  object.index  = object.index or 0
  object.range  = object.range or 100
  setmetatable(object, Sprite_Based_Mt)
  object._cdata = view.new_sprite('', object.parent, 256, 32, false)
  
  object.line   = new_image{parent=object._cdata, path='cubes/cube1',     w=100, h= 8, x=0, y=12}
  object.button = new_image{parent=object._cdata, path='cubes/cube1.bak', w= 20, h=20, x=0, y= 6, depth=-100}
  
  object.text   = new_text{parent=object._cdata, title='0', x=160, y=16, center=true}
  
  -- functions
  local function update_button_position(input)
    local pos_x = C.Input_get_cursor_x(input) - object.parent:get_screen_pos_x() - (object.x or 0) - (object.button:get_size_x()/2)
    local pos_y = object.button:get_pos_y()
    local bg_left = object.line:get_pos_x()
    local bg_right= bg_left + object.line:get_size_x() - object.button:get_size_x()
    if pos_x < bg_left  then pos_x = bg_left  end
    if pos_x > bg_right then pos_x = bg_right end
    object.button:set_pos(pos_x, pos_y)
    --
    object.index = math.floor( (pos_x-bg_left)*object.range / (object.line:get_size_x()-object.button:get_size_x()) )
    object.text:change_text(tostring(object.index))
  end
  object.set_index= function(self, index)
                      if index==nil then index=0 end
                      if index<0 then index=0 end
                      if index>object.range then index=object.range end
                      local bg_left = object.line:get_pos_x()
                      local bg_right= bg_left + object.line:get_size_x() - object.button:get_size_x()
                      local pos_x = ( index / object.range ) * (bg_right - bg_left) + bg_left
                      local pos_y = object.button:get_pos_y()
                      object.button:set_pos(pos_x, pos_y)
                      --
                      object.text:change_text(tostring(object.index))
                      object.index = index
                    end
  object.on_down  = function(self, func)
                      local down_input1 = function(self)
                                            update_button_position(Input1)
                                            if func then func(self) end
                                          end
                      local down_input2 = function(self)
                                            update_button_position(Input2)
                                            if func then func(self) end
                                          end
                      object.button:on_down( down_input1, Input1_left )
                      object.button:on_down( down_input2, Input2_left )
                      object.line:on_down( down_input1, Input1_left )
                      object.line:on_down( down_input2, Input2_left )
                    end
  object.remove_cb= function(self)
                      object.text:remove()
                      object.line:remove()
                      object.button:remove()
                    end
  
  -- init setting
  object.on_down(nil)
  set_focus_leave_pic(object.button._cdata, 'cubes/cube-b-1', 'cubes/cube1.bak')
  object:set_pos(object.x or 0, object.y or 0)
  object:set_depth(object.depth or -10)
  object:set_visible(object.visible or true)
  object:set_index(object.index or 0)
  
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
new_ratio       = new_ratio,
new_selectbox   = new_selectbox,
new_scrollbar   = new_scrollbar
}
