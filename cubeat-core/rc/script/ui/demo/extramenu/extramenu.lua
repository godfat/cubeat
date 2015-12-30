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
  
  local c_w = view.GET_SCREEN_W()/2
  local c_h = view.GET_SCREEN_H()/2
  
  root_ = view.new_sprite("blahblah", parent, 0, 0, true)
  root_:set_pos(c_w, c_h)
  
  menu.bg = ui.new_image9s{ parent=root_, path='textarea2', x=0, y=0, 
                            w=500, h=480, w1=34, w2=32, h1=38, h2=35, center=true }
  
  menu.btn_mode_1 = ui.new_text{ parent=root_, x=0, y=-150, size=32, title='Achievement', depth=-60, center=true }
  menu.btn_mode_1:set_scale(1.5)
  menu.btn_mode_2 = ui.new_text{ parent=root_, x=0, y=-90, size=32, title='Score', depth=-60, center=true }
  menu.btn_mode_2:set_scale(1.5)
  menu.btn_mode_3 = ui.new_text{ parent=root_, x=0, y=-30, size=32, title='Option', depth=-60, center=true }
  menu.btn_mode_3:set_scale(1.5)
  menu.btn_cpu_demo = ui.new_text{ parent=root_, x=0, y=30, size=32, title='CPU demo mode', depth=-60, center=true }
  menu.btn_cpu_demo:set_scale(1.5)
  menu.btn_back  = ui.new_text{ parent=root_, x=0, y=150, size=32, title='go back', depth=-60, center=true }
  menu.btn_back:set_scale(1.5)

  menu.btn_mode_1:on_press(function(self)
    switch.load_page('achievement')
  end, view.Input1_left)
  menu.btn_mode_2:on_press(function(self)
    switch.load_page('score')
  end, view.Input1_left)
  menu.btn_mode_3:on_press(function(self)
    switch.load_page('optionmenu')
  end, view.Input1_left)
  menu.btn_cpu_demo:on_press(function(self)
    local bag = {1, 2, 3, 4, 5, 6}
    random_shuffle(bag)
    select_config.ch_choose[1] = bag[1]
    select_config.ch_choose[2] = bag[2]
    demo:init_cpudemo("char/char"..bag[1].."_new", "char/char"..bag[2].."_new", "stage/jungle"..bag[1])
  end, view.Input1_left)
  menu.btn_back:on_press(function(self)
    switch.load_page('mainmenu')
  end, view.Input1_left)
  
  return menu
end

return {
  init = init
}