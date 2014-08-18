local ffi   = require 'ffi'
local C     = ffi.C
local view  = require 'rc/script/ui/view'
local ui    = require 'rc/script/ui/ui'
local switch= require 'rc/script/ui/demo/switch/switch'
local basepath = require 'rc/script/helper'.basepath
local storystage = require 'rc/script/ui/demo/storyend/config'

local root_

local function init(demo, parent)
  local menu = {}
  
  root_ = view.new_sprite("blahblah", parent, 0, 0, true)
  root_:set_pos(480, 300)

  menu.btn_score = ui.new_text{ parent=root_, x=0, y=0, size=32, title='score' }
  menu.btn_score:set_scale(1.5)
  menu.btn_challenge = ui.new_text{ parent=root_, x=0, y=60, size=32, title='challenge' }
  menu.btn_challenge:set_scale(1.5)
  menu.btn_select= ui.new_text{ parent=root_, x=0, y=120, size=32, title='select actor' }
  menu.btn_select:set_scale(1.5)
  menu.btn_talk  = ui.new_text{ parent=root_, x=0, y=180, size=32, title='talk'}
  menu.btn_talk:set_scale(1.5)
  menu.btn_aitest= ui.new_text{ parent=root_, x=0, y=240, size=32, title='ai test'}
  menu.btn_aitest:set_scale(1.5)
  menu.btn_tut1  = ui.new_text{ parent=root_, x=0, y=300, size=32, title='tutorial test'}
  menu.btn_tut1:set_scale(1.5)
  menu.btn_story  = ui.new_text{ parent=root_, x=320, y=0, size=32, title='story mode' }
  menu.btn_story:set_scale(1.5)
  menu.btn_back  = ui.new_text{ parent=root_, x=0, y=360, size=32, title='back to title' }
  menu.btn_back:set_scale(1.5)

  menu.btn_score:on_press(function(self)
    switch.load_page('score')
  end)
  menu.btn_challenge:on_press(function(self)
    --switch.load_page('tweentest')
    switch.load_page('challengemenu')
  end)
  menu.btn_select:on_press(function(self)
    switch.load_page('select', nil, { game_mode = 1, level = 0, last_menu = 'testmenu' })
  end)
  menu.btn_talk:on_press(function(self)
    switch.load_page('talk', nil)
  end)
  menu.btn_aitest:on_press(function(self)
    demo:init_ai_logging('char/char1_new', 'char/char2_new', 'stage/jungle1') -- actually these parameters are not important
    --switch.show_effect( {id="slide_out_title"} )
  end)
  menu.btn_tut1:on_press(function(self)
    demo:init_single(1, 1, 'char/char1_new', 'stage/jungle1', false) -- test
    --switch.show_effect( {id="slide_out_title"} )
    local filemark = io.open(basepath().."rc/config/tmp/tutored", "w") 
    filemark:write("1") -- just write something
    filemark:close()
  end)
  menu.btn_story:on_press(function(self)
    storystage.set_stage(1)
    switch.load_page('select', nil, { game_mode=99, level=0 })
  end)
  menu.btn_back:on_press(function(self)
    switch.load_page('mainmenu')
  end)

  return menu
end

return {
  init = init
}
