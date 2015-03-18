local ffi   = require 'ffi'
local C     = ffi.C
local view  = require 'rc/script/ui/view'
local ui    = require 'rc/script/ui/ui'
local switch= require 'rc/script/ui/demo/switch/switch'
local select_config = require 'rc/script/ui/demo/select/config'
local storystage    = require 'rc/script/ui/demo/storyend/config'
local file          = require 'rc/script/ui/file'

local root_

local function init(demo, parent)
  local menu = {}
  
  root_ = view.new_sprite("blahblah", parent, 0, 0, true)
  root_:set_pos(480, 300)
  
  menu.new_game = ui.new_text{ parent=root_, x=0, y=0, size=32, title='new game' }
  menu.new_game:set_scale(1.5)
  menu.quick_load = ui.new_text{ parent=root_, x=0, y=60, size=32, title='quick load' }
  menu.quick_load:set_scale(1.5)
  menu.btn_back  = ui.new_text{ parent=root_, x=0, y=180, size=32, title='go back' }
  menu.btn_back:set_scale(1.5)
  
  menu.new_game:on_press(function(self)
    storystage.set_stage(1)
    switch.load_page('select', nil, { game_mode=99, level=0 })
  end)
  menu.btn_back:on_press(function(self)
    switch.load_page('mainmenu')
  end)
  
  local challenge_record = file.load_data('challenge_record', "rb")
  
  -- have quicksave data & didn't clear story.
  if challenge_record and challenge_record["quicksave"] and challenge_record["quicksave"]["stage"]~=6 then
    local ch    = challenge_record["quicksave"]["ch"]
    local stage = challenge_record["quicksave"]["stage"]

    -- set character and next stage.
    storystage.set_stage(stage+1)
    select_config.ch_choose[1] = ch
    local story_data = storystage.get_data(select_config.ch_choose[1])
    select_config.ch_choose[2] = story_data.ch
    
    -- set quick_load btn.
    menu.quick_load:on_press(function(self)
      local function load_talk_page()
        switch.load_page('talk', nil, { game_mode=99 })
        switch.show_effect( {id="slide_out_transfer_to_talk", stage_id=select_config.ch_choose[2]} )
      end
      switch.show_effect( { id="slide_in_transfer", cb=load_talk_page } )
    end)
  
  -- if we should not use quickload, we will not set on_press() function for quick_load btn
  else
    menu.quick_load:set_red(190)
    menu.quick_load:set_green(190)
    menu.quick_load:set_blue(190)
  end
  
  return menu
end

return {
  init = init
}