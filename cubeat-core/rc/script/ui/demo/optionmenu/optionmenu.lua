local ffi   = require 'ffi'
local C     = ffi.C
local view  = require 'rc/script/ui/view'
local ui    = require 'rc/script/ui/ui'
local switch= require 'rc/script/ui/demo/switch/switch'

local root_

local function init(demo, parent)
  local menu = {}
  
  root_ = view.new_sprite("blahblah", parent, 0, 0, true)
  local root_x = 200
  local root_y = 60
  root_:set_pos(root_x, root_y)
  
  menu.square = ui.new_image{ parent=root_, path='area_rect', x=0, y=0, w=view.GET_SCREEN_W()-(root_x*2), h=view.GET_SCREEN_H()-(root_y*2) }
  
  menu.btn_back  = ui.new_text{ parent=root_, x=(view.GET_SCREEN_W()/2)-root_x, y=view.GET_SCREEN_H()-140-root_y, size=40, title='back', center=true, depth=-200 }
  menu.btn_back:on_press(function(self)
    switch.load_page('extramenu')
  end)
  
  return menu
end

return {
  init = init
}