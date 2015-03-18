local ffi   = require 'ffi'
local C     = ffi.C
local view  = require 'rc/script/ui/view'
local ui    = require 'rc/script/ui/ui'
local switch= require 'rc/script/ui/demo/switch/switch'
local storystage  = require 'rc/script/ui/demo/storyend/config'

local root_

local function init(demo, parent)
  local menu = {}
  
  root_ = view.new_sprite("blahblah", parent, 0, 0, true)
  root_:set_pos(480, 300)
  
  menu.new_game = ui.new_text{ parent=root_, x=0, y=0, size=32, title='new game' }
  menu.new_game:set_scale(1.5)
  menu.quick_load = ui.new_text{ parent=root_, x=0, y=60, size=32, title='quick load' }
  menu.quick_load:set_scale(1.5)
  menu.quick_load:set_red(190)
  menu.quick_load:set_green(190)
  menu.quick_load:set_blue(190)
  menu.btn_back  = ui.new_text{ parent=root_, x=0, y=180, size=32, title='go back' }
  menu.btn_back:set_scale(1.5)
  
  menu.new_game:on_press(function(self)
    storystage.set_stage(1)
    switch.load_page('select', nil, { game_mode=99, level=0 })
  end)
  --menu.quick_load:on_press(function(self)
  --end)
  menu.btn_back:on_press(function(self)
    switch.load_page('mainmenu')
  end)
  
  return menu
end

return {
  init = init
}