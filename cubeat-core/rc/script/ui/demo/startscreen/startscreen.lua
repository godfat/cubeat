local ffi    = require 'ffi'
local C      = ffi.C
local view   = require 'rc/script/ui/view'
local ui     = require 'rc/script/ui/ui'
local switch = require 'rc/script/ui/demo/switch/switch'
local event  = require 'rc/script/event/event'
local random = require 'rc/script/helper'.random
local basepath = require 'rc/script/helper'.basepath

-------------------------------------------------------------
 
local root_ = nil

local function init(demo, parent)
  root_ = view.new_sprite("blahblah", parent, 0, 0, false)
  root_:set_pos(0, 0)
  
  local menu = {}
  
  menu.start_button = ui.new_text{ parent = root_, title='press any key', x=640, y=480, size=32, center=true }
  menu.start_button:set_scale(2.0)
  menu.start_button:tween("SineCirc", "Alpha", 255, 0, 1000, -1)

  menu.start_button:on_press(function(self)
    switch.load_page('mainmenu')
  end)

  return menu
end

return {
  init = init 
}
