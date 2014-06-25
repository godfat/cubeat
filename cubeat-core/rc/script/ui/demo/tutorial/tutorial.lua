local ffi   = require 'ffi'
local C     = ffi.C
local view  = require 'rc/script/ui/view'
local ui    = require 'rc/script/ui/ui'
local switch= require 'rc/script/ui/demo/switch/switch'

local root_

local function init(demo, parent)
  local menu = {}
  
  root_ = view.new_sprite("blahblah", parent, 0, 0, true)
  
  for i = 1, 7 do
    local k = 'tuts_'..tostring(i)
    menu[k] = ui.new_image{ parent = root_,
      path='tut/'..i, x= 0, y= 0, w=1280, h=720 }
    menu[k]:set_depth(-100)
    menu[k]:set_visible(false)
    menu[k]:on_press(function(self)
      if i < 7 then
        menu['tuts_'..tostring(i+1)]:set_visible(true)
        menu[k]:set_visible(false)
      else
        switch.load_page('mainmenu')
      end
    end)    
  end
  menu.tuts_1:set_visible(true)
  
  return menu
end

return {
  init = init
}