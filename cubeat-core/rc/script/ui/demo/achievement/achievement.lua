local ffi   = require 'ffi'
local C     = ffi.C
local view  = require 'rc/script/ui/view'
local ui    = require 'rc/script/ui/ui'
local switch= require 'rc/script/ui/demo/switch/switch'

local root_

local function init(demo, parent)
  local menu = {}
  
  root_ = view.new_sprite("blahblah", parent, 0, 0, true)
  root_:set_pos(0, 0)
  menu.bt_group1  = ui.new_text{ parent=root_, x=50, y= 60, size=32, title='Group 1' }
  menu.bt_group2  = ui.new_text{ parent=root_, x=50, y=120, size=32, title='Group 2' }
  menu.bt_group3  = ui.new_text{ parent=root_, x=50, y=180, size=32, title='Group 3' }
  menu.bt_group4  = ui.new_text{ parent=root_, x=50, y=240, size=32, title='Group 4' }
  menu.bt_group5  = ui.new_text{ parent=root_, x=50, y=300, size=32, title='Group 5' }
  menu.btn_back   = ui.new_text{ parent=root_, x=50, y=600, size=32, title='go back' }
  menu.bt_group1:set_scale(1.5)
  menu.bt_group2:set_scale(1.5)
  menu.bt_group3:set_scale(1.5)
  menu.bt_group4:set_scale(1.5)
  menu.bt_group5:set_scale(1.5)
  menu.btn_back:set_scale(1.5)
  
  for i=1,9 do
    menu["achevement_" .. tostring(   i)] = ui.new_text{ parent=root_, x=400, y= 20+i*40, size=32, title='Achievement ' .. tostring(   i) }
    menu["achevement_" .. tostring(10+i)] = ui.new_text{ parent=root_, x=400, y= 20+i*40, size=32, title='Achievement ' .. tostring(10+i), visible=false }
    menu["achevement_" .. tostring(20+i)] = ui.new_text{ parent=root_, x=400, y= 20+i*40, size=32, title='Achievement ' .. tostring(20+i), visible=false }
    menu["achevement_" .. tostring(30+i)] = ui.new_text{ parent=root_, x=400, y= 20+i*40, size=32, title='Achievement ' .. tostring(30+i), visible=false }
    menu["achevement_" .. tostring(40+i)] = ui.new_text{ parent=root_, x=400, y= 20+i*40, size=32, title='Achievement ' .. tostring(40+i), visible=false }
  end
  
  local current_group = 0
  
  menu.bt_group1:on_press(function(self)
    for i=1,9 do
      menu["achevement_" .. tostring(current_group+i)]:set_visible(false)
      menu["achevement_" .. tostring(i)]:set_visible(true)
    end
    current_group = 0
  end)
  menu.bt_group2:on_press(function(self)
    for i=1,9 do
      menu["achevement_" .. tostring(current_group+i)]:set_visible(false)
      menu["achevement_" .. tostring(10+i)]:set_visible(true)
    end
    current_group = 10
  end)
  menu.bt_group3:on_press(function(self)
    for i=1,9 do
      menu["achevement_" .. tostring(current_group+i)]:set_visible(false)
      menu["achevement_" .. tostring(20+i)]:set_visible(true)
    end
    current_group = 20
  end)
  menu.bt_group4:on_press(function(self)
    for i=1,9 do
      menu["achevement_" .. tostring(current_group+i)]:set_visible(false)
      menu["achevement_" .. tostring(30+i)]:set_visible(true)
    end
    current_group = 30
  end)
  menu.bt_group5:on_press(function(self)
    for i=1,9 do
      menu["achevement_" .. tostring(current_group+i)]:set_visible(false)
      menu["achevement_" .. tostring(40+i)]:set_visible(true)
    end
    current_group = 40
  end)
  
  menu.btn_back:on_press(function(self)
    switch.load_page('extramenu')
  end)
  
  return menu
end

return {
  init = init
}