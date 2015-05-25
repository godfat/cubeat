local ffi   = require 'ffi'
local C     = ffi.C
local view  = require 'rc/script/ui/view'
local ui    = require 'rc/script/ui/ui'
local switch= require 'rc/script/ui/demo/switch/switch'
local random_shuffle = require 'rc/script/helper'.random_shuffle
local select_config  = require 'rc/script/ui/demo/select/config'

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
  menu.btn_cpu_demo = ui.new_text{ parent=root_, x=0, y=180, size=32, title='CPU demo mode'}
  menu.btn_cpu_demo:set_scale(1.5)
  menu.btn_back  = ui.new_text{ parent=root_, x=0, y=240, size=32, title='go back' }
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
  menu.btn_cpu_demo:on_press(function(self)
    local bag = {1, 2, 3, 4, 5, 6}
    random_shuffle(bag)
    select_config.ch_choose[1] = bag[1]
    select_config.ch_choose[2] = bag[2]
    demo:init_cpudemo("char/char"..bag[1].."_new", "char/char"..bag[2].."_new", "stage/jungle"..bag[1])
  end)
  menu.btn_back:on_press(function(self)
    switch.load_page('mainmenu')
  end)
  
  return menu
end

return {
  init = init
}