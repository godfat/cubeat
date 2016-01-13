local ffi    = require 'ffi'
local C      = ffi.C
local view   = require 'rc/script/ui/view'
local ui     = require 'rc/script/ui/ui'
local parameter = require 'rc/script/ui/demo/challenge/parameter'

local score_title_enum_ = { '1st', '2nd', '3rd', '4th', '5th', '6th', '7th', '8th', '9th', '10th' }
local score_title_= {}
local score_list_ = {}

local board_
local title_
local your_score_
local btn_retry_
local btn_quit_
local is_created_ = false

local screen_w_ = C.Get_SCREEN_W()
local screen_h_ = C.Get_SCREEN_H()

local function set_visible(b)
  if board_ then board_:set_visible(b) end
  if title_ then title_:set_visible(b) end
  if your_score_ then your_score_:set_visible(b) end
  if btn_retry_ then btn_retry_:set_visible(b) end
  if btn_quit_ then btn_quit_:set_visible(b) end
  for k,v in pairs(score_list_) do
    if v then v:set_visible(b) end
  end
  for k,v in pairs(score_title_) do
    if v then v:set_visible(b) end
  end
end

local function create_score_list(scene)
  if is_created_ == false then
    local center_x = screen_w_/2
    local center_y = screen_h_/2
    
    --board_ = ui.new_image{ parent=scene, path="blahblah", x=center_x, y=center_y, w=400, h=600, depth=-100, center=true, alpha=128 }
    board_      = ui.new_image9s{ parent=scene, path='textarea2', x=center_x, y=center_y, w=400, h=640, w1=34, w2=32, h1=38, h2=35, center=true }
    title_      = ui.new_text { parent=scene, x=center_x    , y=center_y-260, size=32, title='SCORE' , depth=-110, center=true }
    title_:set_scale(1.5)
    --your_score_ = ui.new_text { parent=scene, x=center_x    , y=center_y-210, size=32, title='score' , depth=-110, center=true }
    btn_retry_  = ui.new_text { parent=scene, x=center_x-80 , y=center_y+260, size=32, title='Retry' , depth=-110, center=true }
    btn_quit_   = ui.new_text { parent=scene, x=center_x+80 , y=center_y+260, size=32, title='Quit' , depth=-110, center=true }
    board_:set_color(0,0,0)
    set_visible(false)
    
    is_created_ = true
  end
end

local function remove_score()
    --if board_ then board_:remove() end
    --if title_ then title_:remove() end
    if your_score_ then your_score_:remove() end
    --if btn_retry_ then btn_retry_:remove() end
    --if btn_quit_ then btn_quit_:remove() end
    for k,v in pairs(score_list_) do
      if v then v:remove() end
      score_list_[k]=nil
    end
    for k,v in pairs(score_title_) do
      if v then v:remove() end
      score_title_[k]=nil
    end
end

local function set_score(t, cur_score, scene)
  local center_x = screen_w_/2
  local center_y = screen_h_/2
  
  -- set current score
  your_score_ = ui.new_text { parent=scene, x=center_x    , y=center_y-210, size=32, title='score' , depth=-110, center=true }
  your_score_:change_text(tostring(cur_score))
  
  -- sort socre record
  local sortFunc = function(a, b) return b < a end
  table.sort(t, sortFunc)
  
  -- set score record
  for i=1,10 do
    if t[i] then
      score_title_[i]= ui.new_text { parent=scene, x=center_x-80 , y=center_y-200+(i*40), size=32, title=score_title_enum_[i] , depth=-110, center=true }
      score_list_[i] = ui.new_text { parent=scene, x=center_x+80 , y=center_y-200+(i*40), size=32, title=tostring(t[i]) , depth=-110, center=true }
    end
  end
end

local function on_press_retry(f)
  btn_retry_:on_press(f, view.Input1_left) -- only player1 can input
end

local function on_press_quit(f)
  btn_quit_:on_press(f, view.Input1_left) -- only player1 can input
end

return{
  set_visible       = set_visible,
  create_score_list = create_score_list,
  set_score         = set_score,
  remove_score      = remove_score,
  --
  on_press_retry    = on_press_retry,
  on_press_quit     = on_press_quit,
}