local ffi   = require 'ffi'
local C     = ffi.C
local view  = require 'rc/script/ui/view'
local ui    = require 'rc/script/ui/ui'
local switch= require 'rc/script/ui/demo/switch/switch'

local root_

local function init(demo, parent)
  local menu = {}
  
  local c_w = view.GET_SCREEN_W()/2
  local c_h = view.GET_SCREEN_H()/2
  
  root_ = view.new_sprite("blahblah", parent, 0, 0, true)
  root_:set_pos(c_w, c_h)
  --local root_x = 200
  --local root_y = 60
  --root_:set_pos(root_x, root_y)
  
  menu.bg = ui.new_image9s{ parent=root_, path='textarea2', x=0, y=0, 
                            w=600, h=480, w1=34, w2=32, h1=38, h2=35, center=true }
  
  --menu.square = ui.new_image{ parent=root_, path='area_rect', x=0, y=0, w=view.GET_SCREEN_W()-(root_x*2), h=view.GET_SCREEN_H()-(root_y*2) }
  
  menu.title = ui.new_text{ parent=root_, x=0, y=-160, title='Option', size=40, depth=-60, center=true }
  
  menu.music_title = ui.new_text{ parent=root_, title='Music', x=-220, y=-80, depth=-60, size=32 }
  menu.music_scrollbar = ui.new_scrollbar{ parent=root_, x=-60, y=-80, range=100, depth=-60, index=50 }
  
  menu.sound_title = ui.new_text{ parent=root_, title='Sound', x=-220, y=-20, depth=-60, size=32 }
  menu.sound_scrollbar = ui.new_scrollbar{ parent=root_, x=-60, y=-20, range=100, depth=-60, index=50 }
  
  menu.ratio_1 = ui.new_ratio{ parent=root_, x=-220, y=40, depth=-60, title='Effect 1' }
  menu.ratio_1:on_press(function(self) end, view.Input1_left)
  menu.ratio_2 = ui.new_ratio{ parent=root_, x=-220, y=100, depth=-60, title='Effect 2' }
  menu.ratio_2:on_press(function(self) end, view.Input1_left)
  
  menu.btn_back  = ui.new_text{ parent=root_, x=0, y=180, size=40, title='back', center=true, depth=-200 }
  menu.btn_back:on_press(function(self)
    switch.load_page('extramenu')
  end, view.Input1_left)
  
  return menu
end

return {
  init = init
}