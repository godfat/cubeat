local ffi   = require 'ffi'
local C     = ffi.C
local view  = require 'rc/script/ui/view'
local ui    = require 'rc/script/ui/ui'
local switch= require 'rc/script/ui/demo/switch/switch'
local parameter= require 'rc/script/ui/demo/challenge/parameter'
local file     = require 'rc/script/ui/file'

local root_

local function init(demo, parent)
  local menu = {}
  
  local c_w = view.GET_SCREEN_W()/2
  local c_h = view.GET_SCREEN_H()/2
  
  root_ = view.new_sprite("blahblah", parent, 0, 0, true)
  root_:set_pos(c_w, c_h)
  
  menu.bg = ui.new_image9s{ parent=root_, path='textarea2', x=0, y=0, 
                            w=500, h=400, w1=34, w2=32, h1=38, h2=35, center=true }
  
  menu.btn_mode_1 = ui.new_text{ parent=menu.bg._cdata, x=0, y=-120, size=32, title='puzzle mode', center=true }
  menu.btn_mode_1:set_scale(1.5)
  menu.btn_mode_2 = ui.new_text{ parent=menu.bg._cdata, x=0, y=-60, size=32, title='emergency mode', center=true }
  menu.btn_mode_2:set_scale(1.5)
  menu.btn_mode_3 = ui.new_text{ parent=menu.bg._cdata, x=0, y=0, size=32, title='score attack', center=true }
  menu.btn_mode_3:set_scale(1.5)
  menu.btn_back  = ui.new_text{ parent=menu.bg._cdata, x=0, y=120, size=32, title='go back', center=true }
  menu.btn_back:set_scale(1.5)
  
  menu.btn_mode_1:on_press(function(self)
    switch.load_page('challengesubmenu', nil, parameter.OneShotClear)
  end)
  menu.btn_mode_2:on_press(function(self)
    switch.load_page('challengesubmenu', nil, parameter.WarningCondition)
  end)
  menu.btn_mode_3:on_press(function(self)
    -- switch.load_page('challengesubmenu', nil, parameter.UnLimited)
    demo:init_single(parameter.UnLimited_Countdown, 1, 'char/char1_new', 'stage/jungle1', false)
  end)
  menu.btn_back:on_press(function(self)
    switch.load_page('mainmenu')
  end)
  
  --[[
  menu.btn_mode_4 = ui.new_text{ parent=root_, x=0, y=180, size=32, title='print record'}
  menu.btn_mode_4:set_scale(1.5)
  menu.btn_mode_4:on_press(function(self)
    local challenge_record = file.load_data('challenge_record', "rb")
    if challenge_record then
      for k, v in pairs(challenge_record) do
        if type(v)=='table' then
          for _, s in pairs(v) do
            print( tostring(k) .. ' ----------------- ' .. tostring(s) )
          end
        else
          print( tostring(k) .. ' ----------------- ' .. tostring(v) )
        end
      end
    end
  end)
  --]]
  
  return menu
end

return {
  init = init
}