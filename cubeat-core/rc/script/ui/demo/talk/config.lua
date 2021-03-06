local ffi  = require 'ffi'
local C    = ffi.C
local view = require 'rc/script/ui/view'

local lang_     = 'EN'

local bg_path_  = 'bg4/99complete'
local bg_x_     = -480
local bg_y_     = -300
local bg_w_     = view.GET_SCREEN_W()
local bg_h_     = view.GET_SCREEN_H()

local act_w_  = 480
local act_h_  = 720
local act_x_  = {0, bg_w_-act_w_}
local act_y_  = {0, 0}
local act_d_  = -20
local out_x_  = {-act_w_, bg_w_}
local out_y_  = {0, 0}

local conBG_path_ = 'dialogue_bubble_1'
local conBG_w_    = 512
local conBG_h_    = 224
local conBG_x_    = {bg_w_/2-conBG_w_/2, bg_w_/2-conBG_w_/2}
local conBG_y_    = {70, 300}
local conBG_d_    = -40

local con_offset_x_ =45
local con_offset_y_ =52
local con_x_        = {conBG_x_[1]+con_offset_x_, conBG_x_[2]+con_offset_x_}
local con_y_        = {conBG_y_[1]+con_offset_y_, conBG_y_[2]+con_offset_y_}
local con_d_        = -60

local light_path_     = 'cubes/cube-r-1'
local light_time_     = 1000
local light_offset_x_ = 435
local light_offset_y_ = 180
local light_x_  = {conBG_x_[1]+light_offset_x_, conBG_x_[2]+light_offset_x_}
local light_y_  = {conBG_y_[1]+light_offset_y_, conBG_y_[2]+light_offset_y_}
local light_w_  = 20
local light_h_  = 20
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

local word_size_L_  = 1.7
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
  cube_time = cube_time_
}