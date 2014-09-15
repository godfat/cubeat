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
local icon_w      = 160
local icon_h      = 160
local icon_x_base = screen_w/2
local icon_y_base = 50
local icon_x      = { icon_x_base-160,
                      icon_x_base,
                      icon_x_base-160,
                      icon_x_base,
                      icon_x_base-160,
                      icon_x_base }           
local icon_y      = { icon_y_base,
                      icon_y_base,
                      icon_y_base+160,
                      icon_y_base+160,
                      icon_y_base+320,
                      icon_y_base+320 }
local icon_depth  = -300

-- actor_full
local full_path = function(i) 
  if i == 3 then return 'char3_new/normal' end
  return ('char'..tostring(i)..'_new/glad') 
end
local full_w    = 435
local full_h    = 870
local full_x    = { 0,
                    screen_w-full_w }
local full_y    = -100
local full_depth= -150

local fade_time = 300
local move_time = 300
local move_start= { ffi.new("value2", -full_w,   full_y), ffi.new("value2", screen_w,  full_y) }
local move_end  = { ffi.new("value2", full_x[1], full_y), ffi.new("value2", full_x[2], full_y) }

-- ready_image
local ready_y     = full_y + 300
local ready_depth = -300
local ready_size  = 64

-- start button
local start_x     = screen_w/2
local start_y     = (screen_h/2)+100
local start_depth = ready_depth
local start_size  = 64

-- character description
local char_desc = require 'rc/script/ui/demo/select/char_desc_EN'

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
  move_end  = move_end,
  --
  ready_y     = ready_y,
  ready_depth = ready_depth,
  ready_size  = ready_size,
  --
  start_x     = start_x,
  start_y     = start_y,
  start_depth = start_depth,
  start_size  = start_size,
  -- 
  char_desc = char_desc
}
