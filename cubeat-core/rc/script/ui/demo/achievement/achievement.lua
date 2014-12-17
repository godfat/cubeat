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
  
  menu.left_btn   = ui.new_text{ parent=root_, x=200, y=300, size=32, title='<<' }
  menu.left_btn:set_scale(1.5)
  
  menu.right_btn  = ui.new_text{ parent=root_, x=view.GET_SCREEN_W()-200, y=300, size=32, title='>>' }
  menu.right_btn:set_scale(1.5)
  
  menu.btn_back = ui.new_text{ parent=root_, x=10, y=view.GET_SCREEN_H()-60, size=32, title='< go back' }
  menu.btn_back:set_scale(1.5)
  
  for i=1,9 do
    menu["achevement_" .. tostring(   i)] = ui.new_text{ parent=root_, x=640, y= 200+i*40, size=32, title='Achievement ' .. tostring(   i), center=true }
    menu["achevement_" .. tostring(10+i)] = ui.new_text{ parent=root_, x=640, y= 200+i*40, size=32, title='Achievement ' .. tostring(10+i), center=true, visible=false }
    menu["achevement_" .. tostring(20+i)] = ui.new_text{ parent=root_, x=640, y= 200+i*40, size=32, title='Achievement ' .. tostring(20+i), center=true, visible=false }
    menu["achevement_" .. tostring(30+i)] = ui.new_text{ parent=root_, x=640, y= 200+i*40, size=32, title='Achievement ' .. tostring(30+i), center=true, visible=false }
    menu["achevement_" .. tostring(40+i)] = ui.new_text{ parent=root_, x=640, y= 200+i*40, size=32, title='Achievement ' .. tostring(40+i), center=true, visible=false }
  end
  
  local current_group = 0
  
  menu.left_btn:on_press(function(self)
    for i=1,9 do
      menu["achevement_" .. tostring(current_group+i)]:set_visible(false)
    end
    current_group = current_group - 10
    if current_group<0 then current_group=40 end
    for i=1,9 do
      menu["achevement_" .. tostring(current_group+i)]:set_visible(true)
    end
  end)
  
  menu.right_btn:on_press(function(self)
    for i=1,9 do
      menu["achevement_" .. tostring(current_group+i)]:set_visible(false)
    end
    current_group = current_group + 10
    if current_group>40 then current_group=0 end
    for i=1,9 do
      menu["achevement_" .. tostring(current_group+i)]:set_visible(true)
    end
  end)
  
  menu.btn_back:on_press(function(self)
    switch.load_page('extramenu')
  end)
  
  return menu
end

return {
  init = init
}