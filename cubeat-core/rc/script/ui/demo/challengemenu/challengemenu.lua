local ffi   = require 'ffi'
local C     = ffi.C
local view  = require 'rc/script/ui/view'
local ui    = require 'rc/script/ui/ui'
local switch= require 'rc/script/ui/demo/switch/switch'
local event  = require 'rc/script/event/event'
local random = require 'rc/script/helper'.random
local basepath = require 'rc/script/helper'.basepath

local function init(demo, parent)
  local menu = {}
  
  menu.btn_mode_1 = ui.new_text{ parent=parent, x=0, y=0, size=32, title='mode_1' }
  menu.btn_mode_1:set_scale(1.5)
  menu.btn_mode_2 = ui.new_text{ parent=parent, x=0, y=60, size=32, title='mode_2' }
  menu.btn_mode_2:set_scale(1.5)
  menu.btn_mode_3 = ui.new_text{ parent=parent, x=0, y=120, size=32, title='mode_3' }
  menu.btn_mode_3:set_scale(1.5)
  menu.btn_mode_4 = ui.new_text{ parent=parent, x=0, y=180, size=32, title='mode_4'}
  menu.btn_mode_4:set_scale(1.5)
  menu.btn_back  = ui.new_text{ parent=parent, x=0, y=360, size=32, title='back' }
  menu.btn_back:set_scale(1.5)
  
  menu.btn_mode_1:on_press(function(self)
  end)
  menu.btn_mode_2:on_press(function(self)
    local ch1 = random(6)+1
    local ch2 = random(6)+1
    demo:init_tutorial("char/char"..ch1.."_new", "char/char"..ch2.."_new", "stage/jungle"..ch1)
    switch.slide_out_title()
  end)
  menu.btn_mode_3:on_press(function(self)
  end)
  menu.btn_mode_4:on_press(function(self)
  end)
  menu.btn_back:on_press(function(self)
    switch.load_page('testmenu')
  end)
  
  return menu
end

return {
  init = init
}