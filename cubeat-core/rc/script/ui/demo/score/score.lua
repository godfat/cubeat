local ffi   = require 'ffi'
local C     = ffi.C
local view  = require 'rc/script/ui/view'
local ui    = require 'rc/script/ui/ui'
local switch= require 'rc/script/ui/demo/switch/switch'
local record    = require 'rc/script/ui/demo/challenge/record'
local parameter = require 'rc/script/ui/demo/challenge/parameter'

local root_

local function init(demo, parent)
  local menu = {}
  
  root_ = view.new_sprite("blahblah", parent, 0, 0, false)
  
  -- create ui object
  menu.score_list = ui.new_list{ parent=root_, depth=-1000, visible=true, x=640, y=400 }
  
  menu.left_btn   = ui.new_text{ parent=root_, x=200, y=300, size=32, title='<<' }
  menu.left_btn:set_scale(1.5)
  
  menu.right_btn  = ui.new_text{ parent=root_, x=view.GET_SCREEN_W()-200, y=300, size=32, title='>>' }
  menu.right_btn:set_scale(1.5)
  
  -- load score data
  local score_data = {}
  score_data["normal"]    = record.load( parameter.score, {submode = parameter.UnLimited_Normal} )
  score_data["countdown"] = record.load( parameter.score, {submode = parameter.UnLimited_Countdown} )
  
  -- sort score data
  local sortFunc = function(a, b) return b < a end
  if score_data["normal"]     then table.sort(score_data["normal"]   , sortFunc) end
  if score_data["countdown"]  then table.sort(score_data["countdown"], sortFunc) end
  
  -- create some function
  local function set_data(t)
    menu.score_list:set_title(t)
    if score_data[t] then
      menu.score_list:set_list(score_data[t])
    end    
  end
  
  -- show normal mode score first
  local current_data = "normal"
  set_data(current_data)
  
  -- set on_press funciton
  menu.left_btn:on_press(function(self)
    menu.score_list:clear_list()
    current_data = (current_data=="normal" and "countdown") or (current_data=="countdown" and "normal")
    set_data(current_data)
  end)
  
  menu.right_btn:on_press(function(self)
    menu.score_list:clear_list()
    current_data = (current_data=="normal" and "countdown") or (current_data=="countdown" and "normal")
    set_data(current_data)
  end)
  
  menu.score_list:on_press_back(function(self)
    switch.load_page('extramenu')
  end)
  
  return menu
end

return {
  init = init
}