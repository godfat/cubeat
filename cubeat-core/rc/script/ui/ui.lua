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

local input_lock_ = false

local function set_input_lock(t)
  input_lock_ = t
end

local Input1      = C.Input_get_input1()
local Input2      = C.Input_get_input2()
local Input1_left = C.Input_get_trig1(C.Input_get_input1())
local Input2_left = C.Input_get_trig1(C.Input_get_input2())
local Input1_right= C.Input_get_trig2(C.Input_get_input1())
local Input2_right= C.Input_get_trig2(C.Input_get_input2())

local function set_on_press_callback(sprite, func, input)
  if input == nil then
    --sprite:on_press( Input1_left, func )
    --sprite:on_press( Input2_left, func )
    sprite:on_press( Input1_left, function() if input_lock_ == true then return end func() end )
    sprite:on_press( Input2_left, function() if input_lock_ == true then return end func() end )
  else
    --sprite:on_press( input, func )
    sprite:on_press( input, function() if input_lock_ == true then return end func() end )
  end
end

local function set_on_press_callback_r(sprite, func, input)
  if input == nil then
    --sprite:on_press( Input1_right, func )
    --sprite:on_press( Input2_right, func )
    sprite:on_press( Input1_right, function() if input_lock_ == true then return end func() end )
    sprite:on_press( Input2_right, function() if input_lock_ == true then return end func() end )
  else
    --sprite:on_press( input, func )
    sprite:on_press( input, function() if input_lock_ == true then return end func() end )
  end
end

local function set_on_up_callback(sprite, func, input)
  if input == nil then
    --sprite:on_up( Input1_left, func )
    --sprite:on_up( Input2_left, func )
    sprite:on_up( Input1_left, function() if input_lock_ == true then return end func() end )
    sprite:on_up( Input2_left, function() if input_lock_ == true then return end func() end )
  else
    --sprite:on_up( input, func )
    sprite:on_up( input, function() if input_lock_ == true then return end func() end )
  end
end

local function set_on_down_callback(sprite, func, input)
  if input == nil then
    --sprite:on_down( Input1_left, func )
    --sprite:on_down( Input2_left, func )
    sprite:on_down( Input1_left, function() if input_lock_ == true then return end func() end )
    sprite:on_down( Input2_left, function() if input_lock_ == true then return end func() end )
  else
    --sprite:on_down( input, func )
    sprite:on_down( input, function() if input_lock_ == true then return end func() end )
  end
end

local function set_on_leave_focus_callback(sprite, func, input)
  if input == nil then
    --sprite:on_leave_focus( Input1, func )
    --sprite:on_leave_focus( Input2, func )
    sprite:on_leave_focus( Input1, function() if input_lock_ == true then return end func() end )
    sprite:on_leave_focus( Input2, function() if input_lock_ == true then return end func() end )
  else
    --sprite:on_leave_focus( input, func )
    sprite:on_leave_focus( input, function() if input_lock_ == true then return end func() end )
  end
end

local function set_on_enter_focus_callback(sprite, func, input)
  if input == nil then
    --sprite:on_enter_focus( Input1, func )
    --sprite:on_enter_focus( Input2, func )
    sprite:on_enter_focus( Input1, function() if input_lock_ == true then return end func() end )
    sprite:on_enter_focus( Input2, function() if input_lock_ == true then return end func() end )
  else
    --sprite:on_enter_focus( input, func )
    sprite:on_enter_focus( input, function() if input_lock_ == true then return end func() end )
  end
end

local function set_focus_leave_pic(obj, focus_pic, leave_pic, input)
  --local focus_f = function(self) self:set_texture(focus_pic) end
  --local leave_f = function(self) self:set_texture(leave_pic) end
  local focus_f = function(self) if input_lock_ == true then return end self:set_texture(focus_pic) end
  local leave_f = function(self) if input_lock_ == true then return end self:set_texture(leave_pic) end

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
  --local focus_f = function(self) ffi.cast("pSpriteText*", self):set_color(focus_color.r, focus_color.g, focus_color.b) end
  --local leave_f = function(self) ffi.cast("pSpriteText*", self):set_color(leave_color.r, leave_color.g, leave_color.b) end
  local focus_f = function(self) if input_lock_ == true then return end ffi.cast("pSpriteText*", self):set_color(focus_color.r, focus_color.g, focus_color.b) end
  local leave_f = function(self) if input_lock_ == true then return end ffi.cast("pSpriteText*", self):set_color(leave_color.r, leave_color.g, leave_color.b) end

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

view.Mt_Sprite_Ex.on_up = function(self, func, input)
  set_on_up_callback(self._cdata, func, input)
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
-- 9-Slice Image
----------------------------------------------------------------------------
local function new_image9s(object) 
  if object.parent == nil then error('parent is nil') end
  if object.w == nil then error('image-9-slice width required') end
  if object.h == nil then error('image-9-slice height required') end
  
  setmetatable(object, Sprite_Based_Mt)
  object._cdata = view.new_sprite('blocker', object.parent, object.w, object.h, object.center or false) -- center doesn't work for now
  object:set_color(0, 0, 0)
  object:set_alpha(0)
  
  object.image9s = {}
  
  object.image9s[1] = new_image{ parent = object._cdata, path = object.path..'_1', 
                                 w=object.w1, h=object.h1, x=0, y=0, depth = 0 }
  object.image9s[2] = new_image{ parent = object._cdata, path = object.path..'_2',
                                 w = object.w-object.w1-object.w2, 
                                 h = object.h1, x = object.w1, y = 0, depth = 0 }
  object.image9s[3] = new_image{ parent = object._cdata, path = object.path..'_3',
                                 w = object.w2, h = object.h1, 
                                 x = object.w-object.w2, y = 0, depth = 0 } 
  object.image9s[4] = new_image{ parent = object._cdata, path = object.path..'_4',
                                 w = object.w1, h = object.h-object.h1-object.h2, 
                                 x = 0, y = object.h1, depth = 0 }      
  object.image9s[5] = new_image{ parent = object._cdata, path = object.path..'_5',
                                 w = object.w-object.w1-object.w2, 
                                 h = object.h-object.h1-object.h2, 
                                 x = object.w1, y = object.h1, depth = 0 } 
  object.image9s[6] = new_image{ parent = object._cdata, path = object.path..'_6',
                                 w = object.w2, h = object.h-object.h1-object.h2, 
                                 x = object.w-object.w2, y = object.h1, depth = 0 } 
  object.image9s[7] = new_image{ parent = object._cdata, path = object.path..'_7',
                                 w = object.w1, h = object.h2, 
                                 x = 0, y = object.h-object.h2, depth = 0 } 
  object.image9s[8] = new_image{ parent = object._cdata, path = object.path..'_8',
                                 w = object.w-object.w1-object.w2, h = object.h2, 
                                 x = object.w1, y = object.h-object.h2, depth = 0 } 
  object.image9s[9] = new_image{ parent = object._cdata, path = object.path..'_9',
                                 w = object.w2, h = object.h2, 
                                 x = object.w-object.w2, y = object.h-object.h2, depth = 0 }    
                                 
  if object.center then
    for i = 1, 9 do 
      local new_x = object.image9s[i].x - object.w/2
      local new_y = object.image9s[i].y - object.h/2
      object.image9s[i]:set_pos(new_x, new_y)
    end
  end
  -- init setting
  object:set_pos(object.x or 0, object.y or 0)
  object:set_depth(object.depth or -10)
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
  
  object.bg     = new_image9s{ parent=object._cdata, path='textarea2', x=0, y=0, 
                               w=object.w or 600, h=object.h or 300, w1=34, w2=32, h1=38, h2=35, center=true }
  
  object.text   = new_text{ parent=object._cdata, title=object.title, size=object.size or 30,
                            r=object.r, g=object.g, b=object.b, font=object.font, center=true }
  object.ok     = new_text{ parent=object._cdata, title='OK', size=object.size or 30,
                            r=object.r, g=object.g, b=object.b, center=true }
  object.cancel = new_text{ parent=object._cdata, title='Cancel', size=object.size or 30,
                            r=object.r, g=object.g, b=object.b, center=true }
  object.text:set_pos(0, -45)
  object.text:set_depth(-50)
  object.ok:set_scale(1.5)
  object.ok:set_pos(-75, 80)
  object.ok:set_depth(-50)
  object.cancel:set_scale(1.5)
  object.cancel:set_pos(75, 80)
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
    object.ok:set_pos(0, 80)
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
  object.remove_cb        = function(self)
                              object.text:remove()
                              object.ok:remove()
                              object.cancel:remove()
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
  local value = 0
  local screen_w  = C.Get_SCREEN_W()
  local screen_h  = C.Get_SCREEN_H()
  setmetatable(object, Sprite_Based_Mt)
  --object._cdata = view.new_sprite('area_rect', object.parent, width, height, true)
  object._cdata = view.new_sprite('blocker', object.parent, screen_w, screen_h, true)
  object:set_pos(object.x or screen_w/2, object.y or screen_h/2)
  object:set_color(0, 0, 0)
  object:set_alpha(0)
  
  object.bg = new_image9s{ parent=object._cdata, path='textarea2', x=0, y=0, 
                           w=width, h=height, w1=34, w2=32, h1=38, h2=35, center=true }
  
  local score_pos_y = 60 - (height/2)
  object.score      = new_text{parent=object._cdata, title='score', x=0, y=score_pos_y, center=true, size=40}
  
  local back_pos_y  = (height/2) - 60
  object.back       = new_text{parent=object._cdata, title='back', x=0, y=back_pos_y, center=true, size=40, depth=-60}
  
  -- functions
  object.clear_list   = function(self)
                          for i=1,value do
                            if object.text_score[i] then
                              object.text_score[i]:set_visible(false)
                              object.text_score[i]:remove()
                            end
                            if object.text_name[i]  then
                              object.text_name[i]:set_visible(false)
                              object.text_name[i]:remove()
                            end
                          end
                          object.text_name  = {}
                          object.text_score = {}
                          object.list = {}
                          value = 0
                        end
  object.set_list     = function(self, list)
                          object:clear_list()
                          object.list = list
                          local pos_y = 120 - (height/2)
                          for k,v in pairs(object.list) do
                            if value+1 <= 10 then -- only show 1st to 10th now
                              value = value + 1
                              object.text_score[value] = new_text{parent=object._cdata, title=tostring(k), x=-100, y=pos_y, depth=-60, center=true}
                              object.text_name[value]  = new_text{parent=object._cdata, title=tostring(v), x= 100, y=pos_y, depth=-60, center=true}
                              pos_y=pos_y+30
                            end
                          end
                        end
  --[[
  object.load_list    = function(self, filename)
                          self:set_list( file.load_data(filename) or object.list )
                        end
  object.save_list    = function(self, filename)
                          file.save_data(filename, object.list)
                        end
  --]]
  object.set_title    = function(self, t)
                          object.score:change_text(tostring(t))
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
  local height= 44
  setmetatable(object, Sprite_Based_Mt)
  object._cdata = view.new_sprite('', object.parent, width, height, object.center or false)
  
  object.box    = new_image{parent=object._cdata, path='cubes/cube1.bak', w=40, h=40, x=380, y=2}
  
  object.text   = new_text{parent=object._cdata, title=object.title or 'ratio', x=0, y=0, size=32}
  
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
  object._cdata = view.new_sprite('', object.parent, 280, 44, false)
  
  object.line   = new_image{parent=object._cdata, path='cubes/cube1',     w=200, h=20, x=0, y=12}
  object.button = new_image{parent=object._cdata, path='cubes/cube1.bak', w= 40, h=40, x=0, y= 2, depth=-100}
  
  object.text   = new_text{parent=object._cdata, title='0', x=240, y=20, size=32, center=true}
  
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
new_image9s     = new_image9s,
new_text        = new_text,
new_askbox      = new_askbox,
new_list        = new_list,
new_ratio       = new_ratio,
new_selectbox   = new_selectbox,
new_scrollbar   = new_scrollbar,
set_input_lock  = set_input_lock
}
