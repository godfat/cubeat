
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

local select_char_bg_,
      select_char_title_,
      select_char_img_,
      select_char_ok_
local select_char_btn = {}

function init_select_char_panel(self)
  select_char_bg_ = view.new_sprite("area_rect", scene_, 600, 720, false)
  select_char_bg_:set_pos( 300, 0)
  select_char_bg_:set_depth(1000)
  
  select_char_title_ = view.new_sprite_text_from_sprite("Please select character",
                                                        select_char_bg_,
                                                        "Star Jedi",
                                                        24,
                                                        true,
                                                        255, 255, 255)
  select_char_title_:set_pos(300, 60)
  select_char_title_:set_depth(1000)
  
  select_char_img_ = view.new_sprite_from_sprite("char1/full", select_char_bg_, 200, 600, false)
  select_char_img_:set_pos(40, 100)
  select_char_img_:set_depth(1000)
  
  select_char_ok_ = view.new_sprite_text_from_sprite("ok", select_char_bg_, "Star Jedi", 24, false, 255, 255, 255)
  select_char_ok_:set_pos(340, 400)
  select_char_ok_:on_enter_focus( C.Input_get_input1(), function(self, x, y) select_char_ok_:set_red(100) end )
  select_char_ok_:on_leave_focus( C.Input_get_input1(), function(self, x, y) select_char_ok_:set_red(255) end )
  select_char_ok_:set_depth(-100)
  
  for i=1,5 do
    select_char_btn[i] = view.new_sprite_text_from_sprite("charactor"..i, select_char_bg_, "Star Jedi", 24, false, 255, 255, 255)
    select_char_btn[i]:set_pos(340, 100+i*40)
    select_char_btn[i]:set_depth(-100)
  end
  
  local function sel_char1(self) select_char_img_:set_texture("char1/full") print("push") end
  local function sel_char2(self) select_char_img_:set_texture("char2/full") end
  local function sel_char3(self) select_char_img_:set_texture("char3/full") end
  local function sel_char4(self) select_char_img_:set_texture("char4/full") end
  local function sel_char5(self) select_char_img_:set_texture("char5/full") end
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
  
  select_char_bg_:set_visible(false)
end

local start_game_title_

function init_game_title(self)
  start_game_title_ = view.new_sprite_text("Select Charactor", scene_, "Star Jedi", 36, true, 255, 255, 255)
  start_game_title_:set_pos(688, 480)
  
  local title_focus_in  = function(self, x, y) start_game_title_:set_red(100) end
  local title_focus_out = function(self, x, y) start_game_title_:set_red(255) end
  local title_press     = function(self)
                            select_char_bg_:set_visible(true)
                            start_game_title_:set_visible(false)
                          end
  start_game_title_:on_enter_focus( C.Input_get_input1(), title_focus_in )
  start_game_title_:on_leave_focus( C.Input_get_input1(), title_focus_out)
  start_game_title_:on_press( C.Input_get_trig1(C.Input_get_input1()), title_press )
end

local test1,
      test2
      
function init_test_sprite(self)
  test1 = view.new_sprite("cubes/cube1", scene_, 128, 128, true)
  test1:set_pos(688, 384)
  test2 = view.new_sprite_from_sprite("cubes/cube-b-1", test1, 64, 64, true)
  test2:set_depth(-1)
end

function cycle(self)
  self = ffi.cast("TestUI*", self);
end

function destroy(self)
  self = ffi.cast("TestUI*", self);
end
