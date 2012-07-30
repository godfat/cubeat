local ffi  = require 'ffi'
local C    = ffi.C
local view = require 'rc/script/ui/view'

local bg_path = 'stage_preview/jungle'
local bg_x    = -480
local bg_y    = -300
local bg_w    = view.GET_SCREEN_W()
local bg_h    = view.GET_SCREEN_H()

local act_L_x = -480
local act_L_y = -300
local act_R_x = bg_w-905
local act_R_y = -300
local act_w   = 425
local act_h   = 742
local act_d   = -50

local con_L_x = 50
local con_L_y = -150
local con_R_x = 50
local con_R_y = 100

local conBG_L_x = con_L_x-30
local conBG_L_y = con_L_y-20
local conBG_R_x = con_R_x-30
local conBG_R_y = con_R_y-20
local conBG_w   = 300
local conBG_h   = 150

return {
  bg_path   = bg_path,
  bg_x      = bg_x,
  bg_y      = bg_y,
  bg_w      = bg_w,
  bg_h      = bg_h,
  ----
  act_L_x   = act_L_x,
  act_L_y   = act_L_y,
  act_R_x   = act_R_x,
  act_R_y   = act_R_y,
  act_w     = act_w,
  act_h     = act_h,
  act_d     = act_d,
  ----
  con_L_x   = con_L_x,
  con_L_y   = con_L_y,
  con_R_x   = con_R_x,
  con_R_y   = con_R_y,
  ----
  conBG_L_x = conBG_L_x,
  conBG_L_y = conBG_L_y,
  conBG_R_x = conBG_R_x,
  conBG_R_y = conBG_R_y,
  conBG_w   = conBG_w,
  conBG_h   = conBG_h
}