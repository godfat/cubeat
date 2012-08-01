local ffi  = require 'ffi'
local C    = ffi.C
local view = require 'rc/script/ui/view'

-- player selected actor no.
local ch_choose = { 1, 1 }

-- screen
local screen_w  = view.GET_SCREEN_W()
local screen_h  = view.GET_SCREEN_H()

-- select_actor_page
local bg_path = 'stage_preview/jungle'
local bg_w    = view.GET_SCREEN_W()
local bg_h    = view.GET_SCREEN_H()

-- actor_icon
local icon_path   = function(i) return ('char'..tostring(i)..'_new/character_icon_'..tostring(i)) end
local icon_w      = 173
local icon_h      = 173
local icon_x_base = 80
local icon_x      = { icon_x_base,
                      icon_x_base+180,
                      icon_x_base+360,
                      screen_w-icon_x_base-360-icon_w,
                      screen_w-icon_x_base-180-icon_w,
                      screen_w-icon_x_base-icon_w }
local icon_y      = screen_h-250
local icon_depth  = -100

-- actor_full
local full_path = function(i) return ('char'..tostring(i)..'_new/glad') end
local full_w    = 480
local full_h    = 720
local full_x    = { 0,
                    screen_w-full_w }
local full_y    = 0
local full_depth= -50

local fade_time = 300
local move_time = 300
local move_start= { ffi.new("value2", -full_w,   full_y), ffi.new("value2", screen_w,  full_y) }
local move_end  = { ffi.new("value2", full_x[1], full_y), ffi.new("value2", full_x[2], full_y) }

return {
  ch_choose = ch_choose,
  --
  screen_w  = screen_w,
  screen_h  = screen_h,
  --
  bg_path   = bg_path,
  bg_w      = bg_w,
  bg_h      = bg_h,
  --
  icon_path = icon_path,
  icon_x    = icon_x,
  icon_y    = icon_y,
  icon_w    = icon_w,
  icon_h    = icon_h,
  icon_depth= icon_depth,
  --
  full_path = full_path,
  full_x    = full_x,
  full_y    = full_y,
  full_w    = full_w,
  full_h    = full_h,
  full_depth= full_depth,
  --
  fade_time = fade_time,
  move_time = move_time,
  move_start= move_start,
  move_end  = move_end
}