local ffi   = require 'ffi'
local C     = ffi.C
local view  = require 'rc/script/ui/view'
local ui    = require 'rc/script/ui/ui'
local switch= require 'rc/script/ui/demo/switch/switch'


local function init(demo, parent)
  local menu = {}

  menu.btn_score = ui.new_text{ parent=parent, x=0, y=0, size=32, title='score' }
  menu.btn_score:set_scale(1.5)
  menu.btn_tween = ui.new_text{ parent=parent, x=0, y=60, size=32, title='tween test' }
  menu.btn_tween:set_scale(1.5)
  menu.btn_select= ui.new_text{ parent=parent, x=0, y=120, size=32, title='select actor' }
  menu.btn_select:set_scale(1.5)
  menu.btn_talk  = ui.new_text{ parent=parent, x=0, y=180, size=32, title='talk'} 
  menu.btn_talk:set_scale(1.5)
  menu.btn_back  = ui.new_text{ parent=parent, x=0, y=300, size=32, title='back to title' }
  menu.btn_back:set_scale(1.5)
  
  menu.btn_score:on_press(function(self)
    switch.load_page('score')
  end)
  menu.btn_tween:on_press(function(self)
    switch.load_page('tweentest')
  end)
  menu.btn_select:on_press(function(self)
    switch.load_page('select', 'out')
  end)
  menu.btn_talk:on_press(function(self)
    switch.load_page('talk', 'out')
  end)
  menu.btn_back:on_press(function(self)
    switch.load_page('mainmenu')
  end)
  
  return menu
end

return {
  init = init
}