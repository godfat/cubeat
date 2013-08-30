local ffi   = require 'ffi'
local C     = ffi.C
local view  = require 'rc/script/ui/view'
local ui    = require 'rc/script/ui/ui'
local switch= require 'rc/script/ui/demo/switch/switch'
local parameter= require 'rc/script/ui/demo/challenge/parameter'
local file     = require 'rc/script/ui/file'

local function init(demo, parent)
  local menu = {}
  
  menu.btn_mode_1 = ui.new_text{ parent=parent, x=0, y=0, size=32, title='one shot clear' }
  menu.btn_mode_1:set_scale(1.5)
  menu.btn_mode_2 = ui.new_text{ parent=parent, x=0, y=60, size=32, title='warning condition' }
  menu.btn_mode_2:set_scale(1.5)
  menu.btn_mode_3 = ui.new_text{ parent=parent, x=0, y=120, size=32, title='unlimited'}
  menu.btn_mode_3:set_scale(1.5)
  menu.btn_mode_4 = ui.new_text{ parent=parent, x=0, y=180, size=32, title='print record'}
  menu.btn_mode_4:set_scale(1.5) 
  menu.btn_back  = ui.new_text{ parent=parent, x=0, y=360, size=32, title='back' }
  menu.btn_back:set_scale(1.5)
  
  menu.btn_mode_1:on_press(function(self)
    switch.load_page('challengesubmenu', nil, parameter.OneShotClear)
  end)
  menu.btn_mode_2:on_press(function(self)
    switch.load_page('challengesubmenu', nil, parameter.WarningCondition)
  end)
  menu.btn_mode_3:on_press(function(self)
    switch.load_page('challengesubmenu', nil, parameter.UnLimited)
  end)
  menu.btn_mode_4:on_press(function(self)
    local challenge_record = file.load_data('challenge_record')
    if challenge_record then
      for k, v in pairs(challenge_record) do
        print( tostring(k) .. ' ----------------- ' .. tostring(v) )
      end
    end
  end)
  menu.btn_back:on_press(function(self)
    switch.load_page('testmenu')
  end)
  
  return menu
end

return {
  init = init
}