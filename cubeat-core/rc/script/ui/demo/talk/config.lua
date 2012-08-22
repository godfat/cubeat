local ffi  = require 'ffi'
local C    = ffi.C
local view = require 'rc/script/ui/view'

local bg_path = 'stage_preview/jungle'
local bg_x    = -480
local bg_y    = -300
local bg_w    = view.GET_SCREEN_W()
local bg_h    = view.GET_SCREEN_H()

local act_w = 425
local act_h = 742
local act_x = {0, bg_w-act_w}
local act_y = {0, 0}
local act_d = -50
local out_x = {-act_w, bg_w}
local out_y = {0, 0}

local conBG_w = 300
local conBG_h = 150
local conBG_x = {bg_w/2-conBG_w/2, bg_w/2-conBG_w/2}
local conBG_y = {150, 400}

local con_x = {conBG_x[1]+30, conBG_x[2]+30}
local con_y = {conBG_y[1]+20, conBG_y[2]+20}

local block_x = 0
local block_y = 0
local block_w = bg_w
local block_h = bg_h
local block_a = 0
local block_d = -100
----
----
local slide_in_time = 200
local slide_out_time= 200
local fade_in_time  = 1000
local fade_out_time = 1000

local word_size_L = 1.7
local word_size_M = 1
local word_size_S = 0.7

local actor_shake_loop      = 6
local actor_shake_distance  = 5
local actor_shake_time      = 50
local word_shake_loop       = 3
local word_shake_distance   = 3
local word_shake_time       = 100


return {
  bg_path   = bg_path,
  bg_x      = bg_x,
  bg_y      = bg_y,
  bg_w      = bg_w,
  bg_h      = bg_h,
  ----
  act_w     = act_w,
  act_h     = act_h,
  act_x     = act_x,
  act_y     = act_y,
  act_d     = act_d,
  out_x     = out_x,
  out_y     = out_y,
  ----
  conBG_w   = conBG_w,
  conBG_h   = conBG_h,
  conBG_x   = conBG_x,
  conBG_y   = conBG_y,
  ----
  con_x     = con_x,
  con_y     = con_y,
  ----
  block_x   = block_x,
  block_y   = block_y,
  block_w   = block_w,
  block_h   = block_h,
  block_a   = block_a,
  block_d   = block_d,
  ----
  ----
  slide_in_time = slide_in_time,
  slide_out_time= slide_out_time,
  fade_in_time  = fade_in_time,
  fade_out_time = fade_out_time,
  ----
  word_size_L = word_size_L,
  word_size_M = word_size_M,
  word_size_S = word_size_S,
  ----
  act_s_loop  = actor_shake_loop,
  act_s_dis   = actor_shake_distance,
  act_s_time  = actor_shake_time,
  word_s_loop = word_shake_loop,
  word_s_dis  = word_shake_distance,
  word_s_time = word_shake_time
}