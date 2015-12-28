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
  menu.score_list = ui.new_list{ parent=root_, depth=-100, visible=true, x=640, y=400 }
  
  menu.left_btn   = ui.new_text{ parent=root_, x=340, y=300, size=32, depth=-200, title='<<', center=true }
  menu.left_btn:set_scale(2)
  
  menu.right_btn  = ui.new_text{ parent=root_, x=view.GET_SCREEN_W()-340, y=300, size=32, depth=-200, title='>>', center=true }
  menu.right_btn:set_scale(2)
  
  -- load score data
  local score_data = {}
  score_data["Normal"]    = record.load( parameter.score, {submode = parameter.UnLimited_Normal} )
  score_data["Countdown"] = record.load( parameter.score, {submode = parameter.UnLimited_Countdown} )
  
  -- sort score data
  local sortFunc = function(a, b) return b < a end
  if score_data["Normal"]     then table.sort(score_data["Normal"]   , sortFunc) end
  if score_data["Countdown"]  then table.sort(score_data["Countdown"], sortFunc) end
  
  -- create some function
  local function set_data(t)
    menu.score_list:set_title(t)
    if score_data[t] then
      menu.score_list:set_list(score_data[t])
    end    
  end
  
  -- show normal mode score first
  local current_data = "Normal"
  set_data(current_data)
  
  -- set on_press funciton
  menu.left_btn:on_press(function(self)
    menu.score_list:clear_list()
    current_data = (current_data=="Normal" and "Countdown") or (current_data=="Countdown" and "Normal")
    set_data(current_data)
  end)
  
  menu.right_btn:on_press(function(self)
    menu.score_list:clear_list()
    current_data = (current_data=="Normal" and "Countdown") or (current_data=="Countdown" and "Normal")
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