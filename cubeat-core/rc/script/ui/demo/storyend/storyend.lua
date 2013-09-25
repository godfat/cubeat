local ffi     = require 'ffi'
local C       = ffi.C
local view    = require 'rc/script/ui/view'
local ui      = require 'rc/script/ui/ui'
local switch  = require 'rc/script/ui/demo/switch/switch'

local screen_w_ = C.Get_SCREEN_W()
local screen_h_ = C.Get_SCREEN_H()

local blocker_
local board_
local title_
local btn_next_
local btn_quit_

local function hide()
  blocker_:set_visible(false)
  board_:set_visible(false)
  --title_:set_visible(false)
  btn_next_:set_visible(false)
  btn_quit_:set_visible(false)
end

local function show()
  blocker_:set_visible(true)
  board_:set_visible(true)
  --title_:set_visible(true)
  btn_next_:set_visible(true)
  btn_quit_:set_visible(true)
  
  board_:tween('OElastic', 'Scale', ffi.new("value3",0,0,0), ffi.new("value3",1,1,1), 1000)
end

local function create(scene)
  local center_x = screen_w_/2
  local center_y = screen_h_/2
  blocker_       = ui.new_image{ parent=scene, path="blahblah", x=center_x, y=center_y, w=1280, h=420, depth=-50, alpha=128, center=true }
  blocker_:set_color(0, 0, 0)
  board_         = ui.new_image{ parent=scene, path="blahblah", x=center_x, y=center_y, w=384, h=192, depth=-100, center=true }
  --title_       = ui.new_text { parent=scene, x=center_x    , y=center_y-120, size=32, title='Title'  , depth=-110, center=true }
  --title_:set_scale(1.5)
  btn_next_      = ui.new_text { parent=scene, x=center_x - 100, y=center_y+130, size=32, title='Next'   , depth=-110, center=true }
  btn_next_:set_scale(1.3)
  btn_quit_      = ui.new_text { parent=scene, x=center_x + 100, y=center_y+130, size=32, title='Leave'  , depth=-110, center=true }
  btn_quit_:set_scale(1.3)
  --board_:set_color(0,0,0)
  hide()
end

local function on_press_next(f)
  btn_next_:on_press(f)
end

local function on_press_quit(f)
  btn_quit_:on_press(f)
end

local function set_board(path)
  board_:set_texture(path)
end

local function set_btn_title(s)
  btn_next_:change_text(s)
end

--[[
local function set_title(s)
  title_:change_text(s)
end
--]]

return {
  hide            = hide,
  show            = show,
  create          = create,
  on_press_next   = on_press_next,
  on_press_quit   = on_press_quit,
  set_board       = set_board,
  set_btn_title   = set_btn_title
  --set_title       = set_title
}