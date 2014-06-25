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
  
  menu.target  = ui.new_image{ parent=root_, x=0, y= -50, center=true }
  menu.btn1    = ui.new_text { parent=root_, x=-100, y= 110, title='alpha' }
  menu.btn2    = ui.new_text { parent=root_, x=-100, y= 140, title='pos' }
  menu.btn3    = ui.new_text { parent=root_, x=-100, y= 170, title='rotation'}
  menu.btn4    = ui.new_text { parent=root_, x=-100, y= 200, title='scale'}
  menu.scroll  = ui.new_scrollbar{ parent=root_, x=-80,y=230, range=255, index=255 }
  menu.btn5    = ui.new_text { parent=root_, x=-100, y= 300, size=32, title='back' }
  
  local alpha_s = 255
  local alpha_e = 50
  local tween_alpha  = function(self) menu.target:tween('OElastic', 'Alpha', alpha_s, alpha_e, 2000, 0, nil, 0) end
  menu.btn1:on_press(tween_alpha)
  local pos_s = ffi.new("value2",   0, -50)
  local pos_e = ffi.new("value2", 100, -50)
  local tween_pos = function(self) menu.target:tween('OElastic', 'Pos2D', pos_s, pos_e, 2000, 0, nil, 0) end
  menu.btn2:on_press(tween_pos)
  local rotation_s  = ffi.new("value3", 0, 0,   0)
  local rotation_e  = ffi.new("value3", 0, 0, -180)
  local tween_rotation = function(self) menu.target:tween('OElastic', 'Rotation', rotation_s, rotation_e, 2000, 0, nil, 0) end
  menu.btn3:on_press(tween_rotation)
  local scale_s = ffi.new("value3", 1, 1, 0)
  local scale_e = ffi.new("value3", 2, 2, 0)
  local tween_scale = function(self) menu.target:tween('OElastic', 'Scale', scale_s, scale_e, 2000, 0, nil, 0) end
  menu.btn4:on_press(tween_scale)
  local on_down_scroll = function(self)
    menu.target:set_red( menu.scroll.index )
  end
  menu.scroll:on_down(on_down_scroll)
  menu.btn5:on_press(function(self)
    switch.load_page('testmenu')
  end)
  
  return menu
end

return {
  init = init
}