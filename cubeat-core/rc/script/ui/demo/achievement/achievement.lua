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
  
  menu.square = ui.new_image{ parent=root_, path='area_rect', x=(view.GET_SCREEN_W()-400)/2, y=100, w=400, h=600 }
  
  menu.left_btn   = ui.new_text{ parent=root_, x=200, y=300, size=32, title='<<' }
  menu.left_btn:set_scale(1.5)
  
  menu.right_btn  = ui.new_text{ parent=root_, x=view.GET_SCREEN_W()-200, y=300, size=32, title='>>' }
  menu.right_btn:set_scale(1.5)
  
  menu.btn_back = ui.new_text{ parent=root_, x=view.GET_SCREEN_W()/2, y=640, size=40, title='back', depth=-200, center=true }
  
  for i=0,4 do
    menu["title_" .. tostring(i*10)] = ui.new_text{ parent=root_, x=640, y=180, size=40, title="Title " .. tostring(i+1), center=true }
    if i~=0 then menu["title_" .. tostring(i*10)]:set_visible(false) end
  end
  
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
    menu["title_" .. tostring(current_group)]:set_visible(false)
    
    current_group = current_group - 10
    
    if current_group<0 then current_group=40 end
    for i=1,9 do
      menu["achevement_" .. tostring(current_group+i)]:set_visible(true)
    end
    menu["title_" .. tostring(current_group)]:set_visible(true)
  end)
  
  menu.right_btn:on_press(function(self)
    for i=1,9 do
      menu["achevement_" .. tostring(current_group+i)]:set_visible(false)
    end
    menu["title_" .. tostring(current_group)]:set_visible(false)
    
    current_group = current_group + 10
    
    if current_group>40 then current_group=0 end
    for i=1,9 do
      menu["achevement_" .. tostring(current_group+i)]:set_visible(true)
    end
    menu["title_" .. tostring(current_group)]:set_visible(true)
  end)
  
  menu.btn_back:on_press(function(self)
    switch.load_page('extramenu')
  end)
  
  return menu
end

return {
  init = init
}