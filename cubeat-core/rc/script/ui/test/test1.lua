
local ffi      = require 'ffi'
local C        = ffi.C

local view     = require 'rc/script/ui/test/view'

-------------------- scripts above this line should be separated -----------------
----------------------------------------------------------------------------------

local scene_, sp

function init(self)
  self    = ffi.cast("TestUI*", self);
  scene_  = self:get_ui_scene()
  sp      = view.new_sprite("area_rect", scene_, 64, 64, true)
  sp:set_texture("cubes/cube-b-1")
  sp:set_pos(640, 360)
  sp:set_scale(1.5, 1.5, 1)
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
end

local select_char_bg,
      select_char_title,
      select_char_img
local select_char_btn = {}

function init_select_char_panel(self)
  select_char_bg = view.new_sprite("area_rect", scene_, 512, 680, false)
  select_char_bg:set_pos( 0, 0)
  
  select_char_title = view.new_sprite_text( "Please select character",
                                            scene_,
                                            "Star Jedi",
                                            24,
                                            true,
                                            255, 255, 255 )
  select_char_title:set_pos(256, 60)
  
  select_char_img = view.new_sprite("char1/full", scene_, 200, 600, false)
  select_char_img:set_pos(40, 60)
  
  for i=1,5 do
    select_char_btn[i] = view.new_sprite_text("charactor"..i, scene_, "Star Jedi", 24, false, 255, 255, 255)
    select_char_btn[i]:set_pos(260, 100+i*40)
  end
  
  local function sel_char1(self) select_char_img:set_texture("char1/full") print("push") end
  local function sel_char2(self) select_char_img:set_texture("char2/full") end
  local function sel_char3(self) select_char_img:set_texture("char3/full") end
  local function sel_char4(self) select_char_img:set_texture("char4/full") end
  local function sel_char5(self) select_char_img:set_texture("char5/full") end
  select_char_btn[1]:on_press( C.Input_get_trig1(C.Input_get_input1()), sel_char1 )
  select_char_btn[2]:on_press( C.Input_get_trig1(C.Input_get_input1()), sel_char2 )
  select_char_btn[3]:on_press( C.Input_get_trig1(C.Input_get_input1()), sel_char3 )
  select_char_btn[4]:on_press( C.Input_get_trig1(C.Input_get_input1()), sel_char4 )
  select_char_btn[5]:on_press( C.Input_get_trig1(C.Input_get_input1()), sel_char5 )
  
  local btn_focus_in  = {}
  local btn_focus_out = {}
  for i=1,5 do
    btn_focus_in[i]   = function(self, x, y) select_char_btn[i]:set_red(100) end
    btn_focus_out[i]  = function(self, x, y) select_char_btn[i]:set_red(255) end
    select_char_btn[i]:on_enter_focus( C.Input_get_input1(), btn_focus_in[i] )
    select_char_btn[i]:on_leave_focus( C.Input_get_input1(), btn_focus_out[i] )
  end
  
  select_char_bg:set_visible(false)
  select_char_img:set_visible(false)
  select_char_title:set_visible(false)
  for i=1,5 do select_char_btn[i]:set_visible(false) end
end

function set_select_char_panel_visible(self, visible)
  select_char_img:set_visible(visible)
  select_char_title:set_visible(visible)
  for i=1,5 do select_char_btn[i]:set_visible(visible) end
end

local test_panel,
      test_title

function init_test_panel(self)
  test_panel = view.new_sprite("square", scene_, 256, 256, false)
  test_panel:set_pos(600, 320)
  
  test_title = view.new_sprite_text_from_sprite("hello", test_panel, "Star Jedi", 24, true, 255, 255, 255)
  test_title:set_pos(128, 128)
end

function set_test_panel_visible(self, visible)
  test_panel:set_visible(visible)
end

function cycle(self)
  self = ffi.cast("TestUI*", self);
end

function destroy(self)
  self = ffi.cast("TestUI*", self);
end
