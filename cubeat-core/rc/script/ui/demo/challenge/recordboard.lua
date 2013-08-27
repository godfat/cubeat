local ffi    = require 'ffi'
local C      = ffi.C
local view   = require 'rc/script/ui/view'
local ui     = require 'rc/script/ui/ui'

local board_
local time_title_
local time_record_
local retry_title_
local retry_record_
local btn_next_
local btn_retry_
local btn_quit_

local screen_w_ = C.Get_SCREEN_W()
local screen_h_ = C.Get_SCREEN_H()

local function set_visible(v)
  board_:set_visible(v)
  time_title_:set_visible(v)
  time_record_:set_visible(v)
  retry_title_:set_visible(v)
  retry_record_:set_visible(v)
  btn_next_:set_visible(v)
  btn_retry_:set_visible(v)
  btn_quit_:set_visible(v)
end

local function create_record_board(scene)
  local center_x = screen_w_/2
  local center_y = screen_h_/2
  board_        = ui.new_image{ parent=scene, path="blahblah", x=center_x, y=center_y, w=320, h=200, depth=-100, center=true, alpha=128 }
  time_title_   = ui.new_text { parent=scene, x=center_x-120, y=center_y-80 , size=32, title='Time'   , depth=-110, center=true }
  time_record_  = ui.new_text { parent=scene, x=center_x+40 , y=center_y-80 , size=32, title='Record' , depth=-110, center=true }
  retry_title_  = ui.new_text { parent=scene, x=center_x-120, y=center_y-40 , size=32, title='Retry'  , depth=-110, center=true }
  retry_record_ = ui.new_text { parent=scene, x=center_x+40 , y=center_y-40 , size=32, title='Record' , depth=-110, center=true }
  btn_next_     = ui.new_text { parent=scene, x=center_x-120, y=center_y+80 , size=32, title='Next'   , depth=-110, center=true }
  btn_retry_    = ui.new_text { parent=scene, x=center_x    , y=center_y+80 , size=32, title='Retry'  , depth=-110, center=true }
  btn_quit_     = ui.new_text { parent=scene, x=center_x+120, y=center_y+80 , size=32, title='Quit'   , depth=-110, center=true }
  board_:set_color(0,0,0)
  set_visible(false)
end

local function on_press_next(f)
  btn_next_:on_press(f)
end

local function on_press_retry(f)
  btn_retry_:on_press(f)
end

local function on_press_quit(f)
  btn_quit_:on_press(f)
end

return {
  set_visible         = set_visible,
  create_record_board = create_record_board,
  --
  on_press_next       = on_press_next,
  on_press_retry      = on_press_retry,
  on_press_quit       = on_press_quit,
}