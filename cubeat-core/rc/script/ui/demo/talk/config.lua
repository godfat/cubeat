local ffi  = require 'ffi'
local C    = ffi.C
local view = require 'rc/script/ui/view'

local bg_path_  = 'stage_preview/jungle'
local bg_x_     = -480
local bg_y_     = -300
local bg_w_     = view.GET_SCREEN_W()
local bg_h_     = view.GET_SCREEN_H()

local act_w_  = 425
local act_h_  = 742
local act_x_  = {0, bg_w_-act_w_}
local act_y_  = {0, 0}
local act_d_  = -20
local out_x_  = {-act_w_, bg_w_}
local out_y_  = {0, 0}

local conBG_w_  = 300
local conBG_h_  = 150
local conBG_x_  = {bg_w_/2-conBG_w_/2, bg_w_/2-conBG_w_/2}
local conBG_y_  = {150, 400}
local conBG_d_  = -50

local con_offset_x_ =30
local con_offset_y_ =20
local con_x_        = {conBG_x_[1]+con_offset_x_, conBG_x_[2]+con_offset_x_}
local con_y_        = {conBG_y_[1]+con_offset_y_, conBG_y_[2]+con_offset_y_}
local con_d_        = -50

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


return {
  bg_path   = bg_path_,
  bg_x      = bg_x_,
  bg_y      = bg_y_,
  bg_w      = bg_w_,
  bg_h      = bg_h_,
  ----
  act_w     = act_w_,
  act_h     = act_h_,
  act_x     = act_x_,
  act_y     = act_y_,
  act_d     = act_d_,
  out_x     = out_x_,
  out_y     = out_y_,
  ----
  conBG_w   = conBG_w_,
  conBG_h   = conBG_h_,
  conBG_x   = conBG_x_,
  conBG_y   = conBG_y_,
  conBG_d   = conBG_d_,
  ----
  con_offset_x  = con_offset_x_,
  con_offset_y  = con_offset_y_,
  con_x         = con_x_,
  con_y         = con_y_,
  con_d         = con_d_,
  ----
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
  ----
  word_size_L = word_size_L_,
  word_size_M = word_size_M_,
  word_size_S = word_size_S_,
  ----
  act_s_loop  = actor_shake_loop_,
  act_s_dis   = actor_shake_distance_,
  act_s_time  = actor_shake_time_,
  word_s_loop = word_shake_loop_,
  word_s_dis  = word_shake_distance_,
  word_s_time = word_shake_time_
}