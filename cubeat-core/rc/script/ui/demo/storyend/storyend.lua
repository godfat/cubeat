local ffi     = require 'ffi'
local C       = ffi.C
local view    = require 'rc/script/ui/view'
local ui      = require 'rc/script/ui/ui'
local switch  = require 'rc/script/ui/demo/switch/switch'

local screen_w_ = C.Get_SCREEN_W()
local screen_h_ = C.Get_SCREEN_H()

local board_
local title_
local btn_next_
local btn_quit_

local function hide()
  board_:set_visible(false)
  title_:set_visible(false)
  btn_next_:set_visible(false)
  btn_quit_:set_visible(false)
end

local function show()
  board_:set_visible(true)
  title_:set_visible(true)
  btn_next_:set_visible(true)
  btn_quit_:set_visible(true)
end

local function create(scene)
  local center_x = screen_w_/2
  local center_y = screen_h_/2
  board_        = ui.new_image{ parent=scene, path="blahblah", x=center_x, y=center_y, w=400, h=300, depth=-100, center=true, alpha=128 }
  title_        = ui.new_text { parent=scene, x=center_x    , y=center_y-120, size=32, title='Title'  , depth=-110, center=true }
  title_:set_scale(1.5)
  btn_next_         = ui.new_text { parent=scene, x=center_x-120, y=center_y+130, size=32, title='Next'   , depth=-110, center=true }
  btn_quit_         = ui.new_text { parent=scene, x=center_x+120, y=center_y+130, size=32, title='Quit'   , depth=-110, center=true }
  board_:set_color(0,0,0)
  hide()
end

local function on_press_next(f)
  btn_next_:on_press(f)
end

local function on_press_quit(f)
  btn_quit_:on_press(f)
end

local function set_title(s)
  title_:change_text(s)
end

return {
  hide            = hide,
  show            = show,
  create          = create,
  on_press_next   = on_press_next,
  on_press_quit   = on_press_quit,
  set_title       = set_title
}