local ffi   = require 'ffi'
local C     = ffi.C
local view  = require 'rc/script/ui/view'
local ui    = require 'rc/script/ui/ui'
local switch= require 'rc/script/ui/demo/switch/switch'

local root_

local function init(demo, parent)
  local menu = {}
  
  root_ = view.new_sprite("blahblah", parent, 0, 0, false)
  
  menu.score_list = ui.new_list{ parent=root_, depth=-1000, visible=true, x=640, y=400 }
  menu.score_list:load_list('score')
  menu.score_list:on_press_back(function(self)
    switch.load_page('testmenu')
  end)
  
  return menu
end

return {
  init = init
}