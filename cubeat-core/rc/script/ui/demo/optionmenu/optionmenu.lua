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
  
  menu.btn_back  = ui.new_text{ parent=root_, x=0, y=180, size=32, title='go back' }
  menu.btn_back:set_scale(1.5)
  menu.btn_back:on_press(function(self)
    switch.load_page('extramenu')
  end)
  
  return menu
end

return {
  init = init
}