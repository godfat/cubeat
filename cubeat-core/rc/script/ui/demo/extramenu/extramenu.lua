local ffi   = require 'ffi'
local C     = ffi.C
local view  = require 'rc/script/ui/view'
local ui    = require 'rc/script/ui/ui'
local switch= require 'rc/script/ui/demo/switch/switch'

local root_

local function init(demo, parent)
  local menu = {}
  
  root_ = view.new_sprite("blahblah", parent, 0, 0, true)
  root_:set_pos(480, 300)
  
  menu.btn_mode_1 = ui.new_text{ parent=root_, x=0, y=0, size=32, title='Achievement' }
  menu.btn_mode_1:set_scale(1.5)
  menu.btn_mode_2 = ui.new_text{ parent=root_, x=0, y=60, size=32, title='Score' }
  menu.btn_mode_2:set_scale(1.5)
  menu.btn_mode_3 = ui.new_text{ parent=root_, x=0, y=120, size=32, title='Option'}
  menu.btn_mode_3:set_scale(1.5)
  menu.btn_back  = ui.new_text{ parent=root_, x=0, y=180, size=32, title='go back' }
  menu.btn_back:set_scale(1.5)

  menu.btn_mode_1:on_press(function(self)
    switch.load_page('achievement')
  end)
  menu.btn_mode_2:on_press(function(self)
    switch.load_page('score')
  end)
  menu.btn_mode_3:on_press(function(self)
    switch.load_page('optionmenu')
  end)
  menu.btn_back:on_press(function(self)
    switch.load_page('mainmenu')
  end)
  
  return menu
end

return {
  init = init
}