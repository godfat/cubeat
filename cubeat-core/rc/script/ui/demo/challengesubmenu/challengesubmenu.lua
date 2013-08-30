local ffi   = require 'ffi'
local C     = ffi.C
local view  = require 'rc/script/ui/view'
local ui    = require 'rc/script/ui/ui'
local switch= require 'rc/script/ui/demo/switch/switch'
local parameter= require 'rc/script/ui/demo/challenge/parameter'
local challenge= require 'rc/script/ui/demo/challenge/challenge'

local function init(demo, parent, submode)
  local menu = {}
  
  -- OneShotClear
  if submode == parameter.OneShotClear then
    menu.panel = ui.new_image{ parent = parent, path='area_rect', x=0, y=0, w=300, h=350 }
    menu.panel:set_depth(-150)
    menu.btn1 = ui.new_text{ parent=parent, x=80, y= 40, size=32, depth=-200, title='3chain' }
    menu.btn2 = ui.new_text{ parent=parent, x=80, y= 80, size=32, depth=-200, title='4chain' }
    menu.btn3 = ui.new_text{ parent=parent, x=80, y=120, size=32, depth=-200, title='5chain' }
    menu.btn4 = ui.new_text{ parent=parent, x=80, y=160, size=32, depth=-200, title='6Chain' }
    menu.btn5 = ui.new_text{ parent=parent, x=80, y=200, size=32, depth=-200, title='7chain' }
    menu.btn6 = ui.new_text{ parent=parent, x=80, y=240, size=32, depth=-200, title='8Chain' }
    menu.btn7 = ui.new_text{ parent=parent, x=80, y=280, size=32, depth=-200, title='Unlimited' }
    menu.back = ui.new_text{ parent=parent, x=80, y=320, size=32, depth=-200, title='back' }
    menu.btn1:on_press(function(self)
      demo:init_single(parameter.OneShotClear, 3, 'char/char1_new', 'stage/jungle1', false)
      challenge.set_puzzle_level(3)
      switch.slide_out_title()
    end)
    menu.btn2:on_press(function(self)
      demo:init_single(parameter.OneShotClear, 4, 'char/char1_new', 'stage/jungle1', false)
      challenge.set_puzzle_level(4)
      switch.slide_out_title()
    end)
    menu.btn3:on_press(function(self)
      demo:init_single(parameter.OneShotClear, 5, 'char/char1_new', 'stage/jungle1', false)
      challenge.set_puzzle_level(5)
      switch.slide_out_title()
    end)
    menu.btn4:on_press(function(self)
      demo:init_single(parameter.OneShotClear, 6, 'char/char1_new', 'stage/jungle1', false)
      challenge.set_puzzle_level(6)
      switch.slide_out_title()
    end)
    menu.btn5:on_press(function(self)
      demo:init_single(parameter.OneShotClear, 7, 'char/char1_new', 'stage/jungle1', false)
      challenge.set_puzzle_level(7)
      switch.slide_out_title()
    end)
    menu.btn6:on_press(function(self)
      demo:init_single(parameter.OneShotClear, 8, 'char/char1_new', 'stage/jungle1', false)
      challenge.set_puzzle_level(8)
      switch.slide_out_title()
    end)
    menu.btn7:on_press(function(self)
      demo:init_single(parameter.OneShotClear, 2, 'char/char1_new', 'stage/jungle1', false)
      challenge.set_puzzle_level(2)
      challenge.set_level_unlimited(true) -- puzzle unlimited level mode
      switch.slide_out_title()
    end)
    menu.back:on_press(function(self)
      switch.load_page('challengemenu')
    end)
  end
  
  -- WarningCondition
  if submode == parameter.WarningCondition then
    menu.panel = ui.new_image{ parent = parent, path='area_rect', x=0, y=0, w=300, h=350 }
    menu.panel:set_depth(-150)
    menu.btn1 = ui.new_text{ parent=parent, x=80, y= 40, size=32, depth=-200, title='garbage20' }
    menu.btn2 = ui.new_text{ parent=parent, x=80, y= 80, size=32, depth=-200, title='garbage40' }
    menu.btn3 = ui.new_text{ parent=parent, x=80, y=120, size=32, depth=-200, title='garbage60' }
    menu.btn4 = ui.new_text{ parent=parent, x=80, y=160, size=32, depth=-200, title='garbage80' }
    menu.btn5 = ui.new_text{ parent=parent, x=80, y=200, size=32, depth=-200, title='garbage100' }
    menu.back = ui.new_text{ parent=parent, x=80, y=280, size=32, depth=-200, title='back' }
    menu.btn1:on_press(function(self)
      demo:init_single(parameter.WarningCondition_20, 1, 'char/char1_new', 'stage/jungle1', false)
      switch.slide_out_title()
    end)
    menu.btn2:on_press(function(self)
      demo:init_single(parameter.WarningCondition_40, 1, 'char/char1_new', 'stage/jungle1', false)
      switch.slide_out_title()
    end)
    menu.btn3:on_press(function(self)
      demo:init_single(parameter.WarningCondition_60, 1, 'char/char1_new', 'stage/jungle1', false)
      switch.slide_out_title()
    end)
    menu.btn4:on_press(function(self)
      demo:init_single(parameter.WarningCondition_80, 1, 'char/char1_new', 'stage/jungle1', false)
      switch.slide_out_title()
    end)
    menu.btn5:on_press(function(self)
      demo:init_single(parameter.WarningCondition_100, 1, 'char/char1_new', 'stage/jungle1', false)
      switch.slide_out_title()
    end)
    menu.back:on_press(function(self)
      switch.load_page('challengemenu')
    end)
  end
  
  -- UnLimited
  if submode == parameter.UnLimited then
    menu.panel = ui.new_image{ parent = parent, path='area_rect', x=0, y=0, w=300, h=350 }
    menu.panel:set_depth(-150)
    menu.btn1 = ui.new_text{ parent=parent, x=80, y= 40, size=32, depth=-200, title='normal' }
    menu.btn2 = ui.new_text{ parent=parent, x=80, y= 80, size=32, depth=-200, title='countdown' }
    menu.back = ui.new_text{ parent=parent, x=80, y=280, size=32, depth=-200, title='back' }
    menu.btn1:on_press(function(self)
      demo:init_single(parameter.UnLimited_Normal, 1, 'char/char1_new', 'stage/jungle1', false)
      switch.slide_out_title()
    end)
    menu.btn2:on_press(function(self)
      demo:init_single(parameter.UnLimited_Countdown, 1, 'char/char1_new', 'stage/jungle1', false)
      switch.slide_out_title()
    end)
    menu.back:on_press(function(self)
      switch.load_page('challengemenu')
    end)
  end
  
  return menu
end

return {
  init = init
}