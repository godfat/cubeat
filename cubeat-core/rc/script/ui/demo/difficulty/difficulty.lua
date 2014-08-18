local ffi   = require 'ffi'
local C     = ffi.C
local view  = require 'rc/script/ui/view'
local ui    = require 'rc/script/ui/ui'
local switch= require 'rc/script/ui/demo/switch/switch'

local root_ = nil

local function init(demo, parent, data)
  local menu = {}
  
  root_ = view.new_sprite("blahblah", parent, 0, 0, true)
  root_:set_pos(480, 300)

  menu.btn_easy   = ui.new_text{ parent=root_, x=0, y=0, size=32, title='easy' }
  menu.btn_easy:set_scale(1.5)
  menu.btn_normal = ui.new_text{ parent=root_, x=0, y=60, size=32, title='normal' }
  menu.btn_normal:set_scale(1.5)
  menu.btn_hard   = ui.new_text{ parent=root_, x=0, y=120, size=32, title='hard' }
  menu.btn_hard:set_scale(1.5)
  menu.btn_veryhard= ui.new_text{ parent=root_, x=0, y=180, size=32, title='very hard' }
  menu.btn_veryhard:set_scale(1.5)
  menu.btn_back   = ui.new_text{ parent=root_, x=0, y=240, size=32, title='go back' }
  menu.btn_back:set_scale(1.5)

  menu.btn_easy:on_press(function(self)
    data.level = 0
    data.last_menu = 'difficulty'
    switch.load_page('select', nil, data)
  end)
  menu.btn_normal:on_press(function(self)
    data.level = 1
    data.last_menu = 'difficulty'
    switch.load_page('select', nil, data)
  end)
  menu.btn_hard:on_press(function(self)
    data.level = 2
    data.last_menu = 'difficulty'
    switch.load_page('select', nil, data)
  end)
  menu.btn_veryhard:on_press(function(self)
    data.level = 3
    data.last_menu = 'difficulty'
    switch.load_page('select', nil, data)
  end)
  menu.btn_back:on_press(function(self)
    switch.load_page('mainmenu')
  end)

  return menu
end

return {
  init = init
}
