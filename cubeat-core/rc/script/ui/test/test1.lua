
local ffi      = require 'ffi'
local C        = ffi.C

local view     = require 'rc/script/ui/test/view'

-------------------- scripts above this line should be separated -----------------
----------------------------------------------------------------------------------

local scene_, sp

local char_
local btn_char_ = {}
local text_

function init(self)
  self    = ffi.cast("TestUI*", self);
  scene_  = self:get_ui_scene()
  sp      = view.new_sprite("area_rect", scene_, 64, 64, true)
  sp:set_texture("cubes/cube-b-1")
  sp:set_pos(640, 360)
  sp:set_scale(1.5, 1.5, 1)
  --sp:set_red(0)
  --sp:set_green(0)
  --sp:set_blue(0)
  --sp:set_alpha(100)
  --sp:set_redE(100)
  --sp:set_greenE(100)
  --sp:set_blueE(100)
  sp:set_visible(true)
  sp:set_center_aligned(false)
  sp:move_to(100, 100)
  sp:move_tween(0,0,5000)
  sp:texture_flipH()
  sp:texture_flipV()
  sp:set_size(32, 32)
  
  sp:on_release(C.Input_get_trig1(C.Input_get_input1()), function(self)
    print 'hi' 
    self:on_release(C.Input_get_trig1(C.Input_get_input1()), function(self)
      print 'hello'
    end)
  end)
  
  char_ = view.new_sprite("char1/full", scene_, 200, 600, false)
  char_:move_to(200, 100)

  text_ = view.new_sprite_text("Hello World", scene_, "Star Jedi", 24, false, 255, 255, 255)
  text_:set_pos(600, 250)
  text_:change_text("Please select character")
  print(text_:get_text())
  print(text_:get_font_size())
  text_:set_alpha(100)
  text_:on_press(C.Input_get_trig1(C.Input_get_input1()), function() text_:set_visible(false) end)
  text_:on_release(C.Input_get_trig1(C.Input_get_input1()), function() text_:set_visible(true) end)
  
  btn_char_[1] = view.new_sprite("cubes/cube1", scene_, 64, 64, false)
  btn_char_[2] = view.new_sprite("cubes/cube-r-1", scene_, 64, 64, false)
  btn_char_[3] = view.new_sprite("cubes/cube-g-1", scene_, 64, 64, false)
  btn_char_[4] = view.new_sprite("cubes/cube-b-1", scene_, 64, 64, false)
  btn_char_[5] = view.new_sprite("cubes/cube-y-1", scene_, 64, 64, false)
  btn_char_[1]:move_to(600, 300)
  btn_char_[2]:move_to(680, 300)
  btn_char_[3]:move_to(760, 300)
  btn_char_[4]:move_to(840, 300)
  btn_char_[5]:move_to(920, 300)
  
  local function sel_char1(self) char_:set_texture("char1/full") end
  local function sel_char2(self) char_:set_texture("char2/full") end
  local function sel_char3(self) char_:set_texture("char3/full") end
  local function sel_char4(self) char_:set_texture("char4/full") end
  local function sel_char5(self) char_:set_texture("char5/full") end
  btn_char_[1]:on_press( C.Input_get_trig1(C.Input_get_input1()), sel_char1 )
  btn_char_[2]:on_press( C.Input_get_trig1(C.Input_get_input1()), sel_char2 )
  btn_char_[3]:on_press( C.Input_get_trig1(C.Input_get_input1()), sel_char3 )
  btn_char_[4]:on_press( C.Input_get_trig1(C.Input_get_input1()), sel_char4 )
  btn_char_[5]:on_press( C.Input_get_trig1(C.Input_get_input1()), sel_char5 )
  local function focus_in(self, x, y)
    self:set_red(0)
    self:set_alpha(100)
  end
  local function focus_out(self, x, y)
    self:set_red(255)
    self:set_alpha(255)
  end
  for i=1,5 do
    btn_char_[i]:on_enter_focus( C.Input_get_input1(), focus_in )
    btn_char_[i]:on_leave_focus( C.Input_get_input1(), focus_out )
  end
end

function cycle(self)
  self = ffi.cast("TestUI*", self);
end

function destroy(self)
  self = ffi.cast("TestUI*", self);
end
