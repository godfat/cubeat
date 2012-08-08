local ffi  = require 'ffi'
local C    = ffi.C
local view = require 'rc/script/ui/view'

local bg_path = 'stage_preview/jungle'
local bg_x    = -480
local bg_y    = -300
local bg_w    = view.GET_SCREEN_W()
local bg_h    = view.GET_SCREEN_H()

local act_x = {-480, bg_w-905}
local act_y = {-300, -300}
local act_w = 425
local act_h = 742
local act_d = -50

local con_x = {50, 50}
local con_y = {-150, 100}

local conBG_x = {con_x[1]-30, con_x[2]-30}
local conBG_y = {con_y[1]-20, con_y[2]-20}
local conBG_w = 300
local conBG_h = 150


return {
  bg_path   = bg_path,
  bg_x      = bg_x,
  bg_y      = bg_y,
  bg_w      = bg_w,
  bg_h      = bg_h,
  ----
  act_x     = act_x,
  act_y     = act_y,
  act_w     = act_w,
  act_h     = act_h,
  act_d     = act_d,
  ----
  con_x     = con_x,
  con_y     = con_y,
  ----
  conBG_x   = conBG_x,
  conBG_y   = conBG_y,
  conBG_w   = conBG_w,
  conBG_h   = conBG_h
}