local ffi   = require 'ffi'
local C     = ffi.C
local view  = require 'rc/script/ui/view'
local ui    = require 'rc/script/ui/ui'
local switch= require 'rc/script/ui/demo/switch/switch'


local function init(demo, parent)
  local menu = {}
  
  menu.score_list = ui.new_list{ parent=parent, depth=-1000, visible=true, x=160, y=100 }
  menu.score_list:load_list('score')
  menu.score_list:on_press_back(function(self)
    switch.load_page(demo, 'testmenu')
  end)
  
  return menu
end

return {
  init = init
}