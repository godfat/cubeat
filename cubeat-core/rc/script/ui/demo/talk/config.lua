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
  block_d   = block_d
}