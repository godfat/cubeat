local ffi   = require 'ffi'
local C     = ffi.C
local view  = require 'rc/script/ui/view'
local ui    = require 'rc/script/ui/ui'
local switch= require 'rc/script/ui/demo/switch/switch'
local record    = require 'rc/script/ui/demo/challenge/record'
local parameter = require 'rc/script/ui/demo/challenge/parameter'

local root_

local function init(demo, parent)
  local menu = {}
  
  root_ = view.new_sprite("blahblah", parent, 0, 0, false)
  
  menu.score_list = ui.new_list{ parent=root_, depth=-1000, visible=true, x=640, y=400 }
  
  local score_data = record.load( parameter.score, {submode = parameter.UnLimited_Normal} )
  if score_data then
    local sortFunc = function(a, b) return b < a end
    table.sort(score_data, sortFunc)
    menu.score_list:set_list(score_data)
  end
  
  menu.score_list:on_press_back(function(self)
    switch.load_page('extramenu')
  end)
  
  return menu
end

return {
  init = init
}