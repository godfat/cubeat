local ffi    = require 'ffi'
local C      = ffi.C
local view   = require 'rc/script/ui/view'
local ui     = require 'rc/script/ui/ui'
local parameter = require 'rc/script/ui/demo/challenge/parameter'

local board_
local title_
local cur_score_title_
local cur_score_
local best_score_title_
local best_score_
local btn_next_
local btn_retry_
local btn_quit_
local cur_retry_title_
local cur_retry_

local screen_w_ = C.Get_SCREEN_W()
local screen_h_ = C.Get_SCREEN_H()

local function show(submode, win)
  board_:set_visible(true)
  title_:set_visible(true)
  btn_retry_:set_visible(true)
  btn_quit_:set_visible(true)
  
  -- when OneShotClear mode & game win -> show Next btn.
  if win and submode==parameter.OneShotClear then
    btn_next_:set_visible(true)
    cur_retry_title_:set_visible(true)
    cur_retry_:set_visible(true)
  end
  
  -- when ?? -> show score record.
  if nil then
    cur_score_title_:set_visible(true)
    cur_score_:set_visible(true)
    best_score_title_:set_visible(true)
    best_score_:set_visible(true)
  end
end

local function hide()
  board_:set_visible(false)
  title_:set_visible(false)
  cur_score_title_:set_visible(false)
  cur_score_:set_visible(false)
  best_score_title_:set_visible(false)
  best_score_:set_visible(false)
  btn_next_:set_visible(false)
  btn_retry_:set_visible(false)
  btn_quit_:set_visible(false)
  cur_retry_title_:set_visible(false)
  cur_retry_:set_visible(false)
end

local function create_record_board(scene)
  local center_x = screen_w_/2
  local center_y = screen_h_/2
  --board_        = ui.new_image{ parent=scene, path="blahblah", x=center_x, y=center_y, w=400, h=300, depth=-100, center=true, alpha=128 }
  board_        = ui.new_image9s{ parent=scene, path='textarea2', x=center_x, y=center_y, w=400, h=300, w1=34, w2=32, h1=38, h2=35, center=true }
  title_        = ui.new_text { parent=scene, x=center_x    , y=center_y-100, size=32, title='Title'  , depth=-110, center=true }
  title_:set_scale(1.5)
  cur_score_title_  = ui.new_text { parent=scene, x=center_x-120, y=center_y-40 , size=32, title='Score'  , depth=-110, center=true }
  cur_score_        = ui.new_text { parent=scene, x=center_x+40 , y=center_y-40 , size=32, title='Record' , depth=-110, center=true }
  best_score_title_ = ui.new_text { parent=scene, x=center_x-120, y=center_y    , size=32, title='Best'   , depth=-110, center=true }
  best_score_       = ui.new_text { parent=scene, x=center_x+40 , y=center_y    , size=32, title='Record' , depth=-110, center=true }
  btn_next_         = ui.new_text { parent=scene, x=center_x-120, y=center_y+100, size=32, title='Next'   , depth=-110, center=true }
  btn_retry_        = ui.new_text { parent=scene, x=center_x    , y=center_y+100, size=32, title='Retry'  , depth=-110, center=true }
  btn_quit_         = ui.new_text { parent=scene, x=center_x+120, y=center_y+100, size=32, title='Quit'   , depth=-110, center=true }
  cur_retry_title_  = ui.new_text { parent=scene, x=center_x-120, y=center_y-40 , size=32, title='Retry'  , depth=-110, center=true }
  cur_retry_        = ui.new_text { parent=scene, x=center_x+40 , y=center_y-40 , size=32, title='Record' , depth=-110, center=true }
  board_:set_color(0,0,0)
  hide()
end

local function set_title(s)
  title_:change_text(s)
end

local function set_score(cur_score, best_score)
  cur_score_:change_text(tostring(cur_score))
  best_score_:change_text(tostring(best_score))
end

local function set_retry(s)
  cur_retry_:change_text(tostring(s))
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
  show  = show,
  hide  = hide,
  create_record_board = create_record_board,
  --
  set_title           = set_title,
  set_score           = set_score,
  set_retry           = set_retry,
  --
  on_press_next       = on_press_next,
  on_press_retry      = on_press_retry,
  on_press_quit       = on_press_quit,
}