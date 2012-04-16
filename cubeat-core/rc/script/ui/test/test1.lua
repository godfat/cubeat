
local ffi      = require 'ffi'
local C        = ffi.C

local view     = require 'rc/script/ui/test/view'
local msgpack  = require 'rc/script/ui/test/luajit-msgpack-pure'

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

  select_char_title_ = view.new_sprite_text("Please select character",
                                            select_char_bg_,
                                            "Star Jedi",
                                            24,
                                            true,
                                            255, 255, 255)
  select_char_title_:set_pos(300, 60)
  select_char_title_:set_depth(1000)

  select_char_img_ = view.new_sprite("char1/full", select_char_bg_, 200, 600, false)
  select_char_img_:set_pos(40, 100)
  select_char_img_:set_depth(1000)

  select_char_ok_ = view.new_sprite_text("ok", select_char_bg_, "Star Jedi", 24, false, 255, 255, 255)
  select_char_ok_:set_pos(340, 400)
  select_char_ok_:on_enter_focus( C.Input_get_input1(), function(self, x, y) select_char_ok_:set_red(100) end )
  select_char_ok_:on_leave_focus( C.Input_get_input1(), function(self, x, y) select_char_ok_:set_red(255) end )
  select_char_ok_:set_depth(-100)

  for i=1,5 do
    select_char_btn[i] = view.new_sprite_text("charactor"..i, select_char_bg_, "Star Jedi", 24, false, 255, 255, 255)
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

local panel1
local button1 = {}
local button2 = {}
local button3 = {}
local button4 = {}
local panel2
local back_btn = {}
local ratio1 = {}
local ratio2 = {}
local selectbox1 = {}
local selectbox_title = {"SELECT1", "SELECT2", "SELECT3"}
local scrollbar1 = {}
local title_image
local option_data = {
                      ratio1 = false,
                      ratio2 = false,
                      scrollbar1_index = 255,
                      selectbox_index = 1
                    }

function init_test_menu(self)
  option_data = view.load_option(option_data)
  view.save_option(option_data)
  --=======================INIT PANEL1=======================--
  panel1 = view.new_sprite("area_rect", scene_, 256, 256, true)
  panel1:set_pos(688, 384)
  local button1_set = {x=-100, y=-100, title="button1"}
  button1 = view.new_ui_button(panel1, button1_set)
  local button2_set = {x=-100, y=-50, title="button2"}
  button2 = view.new_ui_button(panel1, button2_set)
  local button3_set = {x=-100, y=0, title="button3"}
  button3 = view.new_ui_button(panel1, button3_set)
  local button4_set = {x=-100, y=50, title="button4"}
  button4 = view.new_ui_button(panel1, button4_set)
  --button1:set_pos(-100, -100)
  --button2:set_pos(-100, -50)
  --button3:set_pos(-100, 0)
  --button4:set_pos(-100, 50)

  --=======================INIT PANEL2=======================--
  panel2  = view.new_sprite("area_rect", scene_, 512, 512, true)
  panel2:set_pos(688, 384)
  panel2:set_alpha(0)
  panel2:set_visible(false)
  --
  local back_btn_set = {x=-200, y=150, alpha=0, visible=false, title="back"}
  back_btn = view.new_ui_button(panel2, back_btn_set)
  --back_btn:set_pos(-200, 150)
  --back_btn:set_alpha(0)
  --back_btn:set_visible(false)
  --
  local ratio_set1 = {x=-200, y=-200, alpha=0, visible=false, title="ratio1"}
  ratio1  = view.new_ui_ratio(panel2, ratio_set1)
  local ratio_set2 = {x=-200, y=-150, alpha=0, visible=false, title="ratio2"}
  ratio2  = view.new_ui_ratio(panel2, ratio_set2)
  ratio1:set_pressed(option_data["ratio1"])
  ratio2:set_pressed(option_data["ratio2"])
  --ratio1:set_pos(-200, -200)
  --ratio2:set_pos(-200, -150)
  --ratio1:set_alpha(0)
  --ratio2:set_alpha(0)
  --ratio1:set_visible(false)
  --ratio2:set_visible(false)
  local ratio1_press = function(self)
                        print("press ratio1")
                        option_data["ratio1"] = ratio1.is_pressed
                        view.save_option(option_data)
                       end
  local ratio2_press = function(self)
                        print("press ratio2")
                        option_data["ratio2"] = ratio2.is_pressed
                        view.save_option(option_data)
                       end
  ratio1:on_press(ratio1_press)
  ratio2:on_press(ratio2_press)
  --
  local selectbox_set = { x=-200, y=-100, alpha=0, visible=false, title_tb=selectbox_title}
  selectbox1 = view.new_ui_selectbox(panel2, selectbox_set)
  selectbox1:set_index(option_data["selectbox_index"])
  --selectbox1:set_pos(-200, -100)
  --selectbox1:set_alpha(0)
  --selectbox1:set_visible(false)
  local left_press  = function(self)
                        print("press left button")
                        --
                        option_data["selectbox_index"] = selectbox1.setting.index
                        view.save_option(option_data)
                      end
  local right_press = function(self)
                        print("press right button")
                        --
                        option_data["selectbox_index"] = selectbox1.setting.index
                        view.save_option(option_data)
                      end
  selectbox1:left_on_press(left_press)
  selectbox1:right_on_press(right_press)
  --
  title_image = view.new_sprite("title", panel2, 128, 128, false)
  title_image:set_pos(-200, 0)
  title_image:set_alpha(0)
  title_image:set_visible(false)
  --
  scrollbar1 = view.new_ui_scrollbar(255, panel2)
  scrollbar1:set_index(option_data["scrollbar1_index"])
  scrollbar1:set_pos(-200, -50)
  scrollbar1:set_alpha(0)
  scrollbar1:set_visible(false)
  local scrollbar_press = function(self)
                            print("the index is "..tostring(scrollbar1.index))
                            title_image:set_blue(scrollbar1.index)
                            --
                            option_data["scrollbar1_index"] = scrollbar1.index
                            view.save_option(option_data)
                          end
  scrollbar1:on_press(scrollbar_press)

  --=======================BACK BUTTON=======================--
  local tween_cb =  function(self) end
  local tween_cb_panel2 = function(self) panel2:set_visible(false) end
  local tween_cb_title  = function(self) title_image:set_visible(false) end
  local back_btn_press =  function(self)
                            panel1:set_visible(true)
                            panel1:on_tween_line_alpha(255, 500, 0, tween_cb, 0)
                            button1:set_fade(255)
                            button2:set_fade(255)
                            button3:set_fade(255)
                            button4:set_fade(255)
                            panel2:set_visible(true)
                            panel2:on_tween_line_alpha(0, 500, 0, tween_cb_panel2, 0)
                            ratio1:set_fade(0)
                            ratio2:set_fade(0)
                            selectbox1:set_fade(0)
                            title_image:set_visible(true)
                            title_image:on_tween_line_alpha(0, 500, 0, tween_cb_title, 0)
                            scrollbar1:set_fade(0)
                            back_btn:set_fade(0)
                          end
  back_btn:on_press(back_btn_press)

  --=======================PANEL1 BUTTON2=======================--
  local tween_cb =  function(self) end
  local tween_cb_panel1 = function(self) panel1:set_visible(false) end
  local button2_press = function(self)
                          panel1:set_visible(true)
                          panel1:on_tween_line_alpha(0, 500, 0, tween_cb_panel1, 0)
                          button1:set_fade(0)
                          button2:set_fade(0)
                          button3:set_fade(0)
                          button4:set_fade(0)
                          panel2:set_visible(true)
                          panel2:on_tween_line_alpha(255, 500, 0, tween_cb, 0)
                          ratio1:set_fade(255)
                          ratio2:set_fade(255)
                          selectbox1:set_fade(255)
                          title_image:set_visible(true)
                          title_image:on_tween_line_alpha(255, 500, 0, tween_cb, 0)
                          scrollbar1:set_fade(255)
                          back_btn:set_fade(255)
                        end
  button2:on_press( button2_press )
end

local test1,
      test2,
      test3

function init_test_sprite(self)
  test1 = view.new_sprite("cubes/cube1", scene_, 64, 64, true)
  test1:set_pos(688, 384)
  test2 = view.new_sprite("cubes/cube1", scene_, 64, 64, true)
  test2:set_pos(840, 384)
  local tween_cb =  function(self)
                      test2:set_red(100)
                    end
  local test1_on_press =  function(self)
                            --test2:on_tween_line_pos(940, 384, 4000, 0, tween_cb, 500)
                            --test2:on_tween_line_rotation(0, 45, 0, 4000, 0, tween_cb, 500)
                            --test2:on_tween_line_scale(2, 1, 0, 4000, 0, tween_cb, 500)
                            --test2:on_tween_line_color_diffuse(50, 4000, 0, tween_cb, 500)
                            --test2:on_tween_line_blue(50, 4000, 0, tween_cb, 500)
                            test2:on_tween_line_alpha(50, 4000, 0, tween_cb, 500)
                          end
  test1:on_press( C.Input_get_trig1(C.Input_get_input1()), test1_on_press )

  test3 = view.new_sprite_text("click", scene_, "Star Jedi", 24, true, 255, 255, 255)
  test3:set_pos(688, 324)
end

function cycle(self)
  self = ffi.cast("TestUI*", self);
end

function destroy(self)
  self = ffi.cast("TestUI*", self);
end
