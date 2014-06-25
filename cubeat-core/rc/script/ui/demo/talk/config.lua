local ffi  = require 'ffi'
local C    = ffi.C
local view = require 'rc/script/ui/view'

local lang_     = 'JP'

local bg_path_  = 'bg4/99complete'
local bg_x_     = 0
local bg_y_     = 0
local bg_w_     = view.GET_SCREEN_W()
local bg_h_     = view.GET_SCREEN_H()

local act_w_  = 512
local act_h_  = 1024
local act_x_  = {-80, bg_w_-act_w_+80}
local act_y_  = {16, 16}
local act_d_  = -150
local out_x_  = {-act_w_, bg_w_}
local out_y_  = {16, 16}

local conBG_path_ = 'dialogue_bubble_1'
local conBG_w_    = 640
local conBG_h_    = 320
local conBG_x_    = {bg_w_/2-conBG_w_/2, bg_w_/2-conBG_w_/2}
local conBG_y_    = {200, 300}
local conBG_d_    = -40

local con_offset_x_ =100
local con_offset_y_ =90
local con_x_        = {conBG_x_[1]+con_offset_x_, conBG_x_[2]+con_offset_x_}
local con_y_        = {conBG_y_[1]+con_offset_y_, conBG_y_[2]+con_offset_y_}
local con_d_        = -60

local light_path_     = 'dialog_next'
local light_time_     = 1000
local light_offset_x_ = 500
local light_offset_y_ = 200
local light_x_  = {conBG_x_[1]+light_offset_x_, conBG_x_[2]+light_offset_x_}
local light_y_  = {conBG_y_[1]+light_offset_y_, conBG_y_[2]+light_offset_y_}
local light_w_  = 32
local light_h_  = 32
local light_d_  = -80

local block_x_  = 0
local block_y_  = 0
local block_w_  = bg_w_
local block_h_  = bg_h_
local block_a_  = 0
local block_d_  = -100
----
----
local slide_in_time_  = 200
local slide_out_time_ = 200
local fade_in_time_   = 1000
local fade_out_time_  = 1000

local word_size_L_  = 1.6
local word_size_M_  = 1
local word_size_S_  = 0.7

local actor_shake_loop_     = 6
local actor_shake_distance_ = 5
local actor_shake_time_     = 50
local word_shake_loop_      = 3
local word_shake_distance_  = 3
local word_shake_time_      = 100
----
----
local cube_path_  = 'cubes/cube-r-1'
local cube_x1_    = -100
local cube_y1_    = -50
local cube_x2_    = 100
local cube_y2_    = 150
local cube_x3_    = 200
local cube_y3_    = -150
local cube_w_     = 60
local cube_h_     = 60
local cube_time_  = 200
----
----
local act_pos_ = {}
act_pos_['char1_new/anger']   = { {x=0,y=-100}, {x=768,y=-100} }
act_pos_['char1_new/glad']    = { {x=0,y=-100}, {x=768,y=-100} }
act_pos_['char1_new/normal']  = { {x=0,y=-100}, {x=768,y=-100} }
act_pos_['char1_new/sad']     = { {x=0,y=-100}, {x=768,y=-100} }
act_pos_['char2_new/anger']   = { {x=0,y=-100}, {x=768,y=-100} }
act_pos_['char2_new/glad']    = { {x=0,y=-140}, {x=768,y=-100} }
act_pos_['char2_new/normal']  = { {x=0,y=-100}, {x=768,y=-100} }
act_pos_['char2_new/sad']     = { {x=0,y=-100}, {x=768,y=-100} }
act_pos_['char3_new/anger']   = { {x=0,y=-120}, {x=768,y=-120} }
act_pos_['char3_new/glad']    = { {x=0,y=-120}, {x=768,y=-120} }
act_pos_['char3_new/normal']  = { {x=0,y=-120}, {x=768,y=-120} }
act_pos_['char3_new/sad']     = { {x=0,y=-100}, {x=768,y=-120} }
act_pos_['char4_new/anger']   = { {x=0,y=-100}, {x=768,y=-100} }
act_pos_['char4_new/glad']    = { {x=0,y=-100}, {x=768,y=-100} }
act_pos_['char4_new/normal']  = { {x=0,y=-100}, {x=768,y=-100} }
act_pos_['char4_new/sad']     = { {x=0,y=-100}, {x=768,y=-100} }
act_pos_['char5_new/anger']   = { {x=0,y=-100}, {x=768,y=-100} }
act_pos_['char5_new/glad']    = { {x=0,y=-100}, {x=768,y=-100} }
act_pos_['char5_new/normal']  = { {x=0,y=-100}, {x=768,y=-100} }
act_pos_['char5_new/sad']     = { {x=0,y=-100}, {x=768,y=-100} }
act_pos_['char6_new/anger']   = { {x=0,y=-100}, {x=768,y=-100} }
act_pos_['char6_new/glad']    = { {x=0,y=-100}, {x=768,y=-100} }
act_pos_['char6_new/normal']  = { {x=0,y=-100}, {x=768,y=-100} }
act_pos_['char6_new/sad']     = { {x=0,y=-100}, {x=768,y=-100} }
----
----
local conBG_pos_ = {}
conBG_pos_['char1_new/anger']   = { {x=330,y=20}, {x=340,y=300} }
conBG_pos_['char1_new/glad']    = { {x=330,y=60}, {x=360,y=320} }
conBG_pos_['char1_new/normal']  = { {x=270,y=-10}, {x=400,y=240} }
conBG_pos_['char1_new/sad']     = { {x=350,y=130}, {x=310,y=380} }
conBG_pos_['char2_new/anger']   = { {x=300,y=300}, {x=350,y=300} }
conBG_pos_['char2_new/glad']    = { {x=280,y=170}, {x=340,y=260} }
conBG_pos_['char2_new/normal']  = { {x=280,y=0}, {x=385,y=250} }
conBG_pos_['char2_new/sad']     = { {x=320,y=200}, {x=390,y=260} }
conBG_pos_['char3_new/anger']   = { {x=300,y=200}, {x=320,y=300} }
conBG_pos_['char3_new/glad']    = { {x=300,y=240}, {x=360,y=240} }
conBG_pos_['char3_new/normal']  = { {x=260,y=80}, {x=410,y=330} }
conBG_pos_['char3_new/sad']     = { {x=320,y=200}, {x=280,y=260} }
conBG_pos_['char4_new/anger']   = { {x=285,y=40}, {x=320,y=300} }
conBG_pos_['char4_new/glad']    = { {x=260,y=0}, {x=420,y=260} }
conBG_pos_['char4_new/normal']  = { {x=280,y=0}, {x=390,y=260} }
conBG_pos_['char4_new/sad']     = { {x=320,y=200}, {x=420,y=270} }
conBG_pos_['char5_new/anger']   = { {x=300,y=200}, {x=400,y=210} }
conBG_pos_['char5_new/glad']    = { {x=290,y=210}, {x=400,y=200} }
conBG_pos_['char5_new/normal']  = { {x=280,y=180}, {x=420,y=180} }
conBG_pos_['char5_new/sad']     = { {x=340,y=240}, {x=305,y=240} }
conBG_pos_['char6_new/anger']   = { {x=320,y=200}, {x=350,y=290} }
conBG_pos_['char6_new/glad']    = { {x=320,y=200}, {x=410,y=220} }
conBG_pos_['char6_new/normal']  = { {x=320,y=200}, {x=420,y=250} }
conBG_pos_['char6_new/sad']     = { {x=320,y=200}, {x=320,y=300} }


return {
  lang      = lang_,

  bg_path   = bg_path_,
  bg_x      = bg_x_,
  bg_y      = bg_y_,
  bg_w      = bg_w_,
  bg_h      = bg_h_,

  act_w     = act_w_,
  act_h     = act_h_,
  act_x     = act_x_,
  act_y     = act_y_,
  act_d     = act_d_,
  out_x     = out_x_,
  out_y     = out_y_,

  conBG_path= conBG_path_,
  conBG_w   = conBG_w_,
  conBG_h   = conBG_h_,
  conBG_x   = conBG_x_,
  conBG_y   = conBG_y_,
  conBG_d   = conBG_d_,

  con_offset_x  = con_offset_x_,
  con_offset_y  = con_offset_y_,
  con_x         = con_x_,
  con_y         = con_y_,
  con_d         = con_d_,

  light_path      = light_path_,
  light_time      = light_time_,
  light_offset_x  = light_offset_x_,
  light_offset_y  = light_offset_y_,
  light_x         = light_x_,
  light_y         = light_y_,
  light_w         = light_w_,
  light_h         = light_h_,
  light_d         = light_d_,

  block_x   = block_x_,
  block_y   = block_y_,
  block_w   = block_w_,
  block_h   = block_h_,
  block_a   = block_a_,
  block_d   = block_d_,
  ----
  ----
  slide_in_time = slide_in_time_,
  slide_out_time= slide_out_time_,
  fade_in_time  = fade_in_time_,
  fade_out_time = fade_out_time_,

  word_size_L = word_size_L_,
  word_size_M = word_size_M_,
  word_size_S = word_size_S_,

  act_s_loop  = actor_shake_loop_,
  act_s_dis   = actor_shake_distance_,
  act_s_time  = actor_shake_time_,
  word_s_loop = word_shake_loop_,
  word_s_dis  = word_shake_distance_,
  word_s_time = word_shake_time_,
  ----
  ----
  cube_path = cube_path_,
  cube_x1   = cube_x1_,
  cube_y1   = cube_y1_,
  cube_x2   = cube_x2_,
  cube_y2   = cube_y2_,
  cube_x3   = cube_x3_,
  cube_y3   = cube_y3_,
  cube_w    = cube_w_,
  cube_h    = cube_h_,
  cube_time = cube_time_,
  ----
  ----
  act_pos   = act_pos_,
  conBG_pos = conBG_pos_,
}