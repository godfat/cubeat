local ffi   = require 'ffi'
local C     = ffi.C
local view  = require 'rc/script/ui/view'
local ui    = require 'rc/script/ui/ui'
local switch= require 'rc/script/ui/demo/switch/switch'
local parameter= require 'rc/script/ui/demo/challengemenu/parameter'

local function init(demo, parent, submode)
  local menu = {}
  
  -- OneShotClear
  if submode == parameter.OneShotClear then
  end
  
  -- HighestChain
  if submode == parameter.HighestChain then
    menu.panel = ui.new_image{ parent = parent, path='area_rect', x=0, y=0, w=300, h=350 }
    menu.panel:set_depth(-150)
    menu.btn1 = ui.new_text{ parent=parent, x=80, y= 40, size=32, depth=-200, title='3chain' }
    menu.btn2 = ui.new_text{ parent=parent, x=80, y= 80, size=32, depth=-200, title='4chain' }
    menu.btn3 = ui.new_text{ parent=parent, x=80, y=120, size=32, depth=-200, title='5chain' }
    menu.btn4 = ui.new_text{ parent=parent, x=80, y=160, size=32, depth=-200, title='3chain_1min' }
    menu.btn5 = ui.new_text{ parent=parent, x=80, y=200, size=32, depth=-200, title='4chain_2min' }
    menu.btn6 = ui.new_text{ parent=parent, x=80, y=240, size=32, depth=-200, title='5chain_3min' }
    menu.back = ui.new_text{ parent=parent, x=80, y=280, size=32, depth=-200, title='back' }
    menu.btn1:on_press(function(self)
      demo:init_single(parameter.Highest_3Chain, 1, 'char/char1_new', 'stage/jungle1', false)
      switch.slide_out_title()
    end)
    menu.btn2:on_press(function(self)
      demo:init_single(parameter.Highest_4Chain, 1, 'char/char1_new', 'stage/jungle1', false)
      switch.slide_out_title()
    end)
    menu.btn3:on_press(function(self)
      demo:init_single(parameter.Highest_5Chain, 1, 'char/char1_new', 'stage/jungle1', false)
      switch.slide_out_title()
    end)
    menu.btn4:on_press(function(self)
      demo:init_single(parameter.Highest_3Chain_1Min, 1, 'char/char1_new', 'stage/jungle1', false)
      switch.slide_out_title()
    end)
    menu.btn5:on_press(function(self)
      demo:init_single(parameter.Highest_4Chain_2Min, 1, 'char/char1_new', 'stage/jungle1', false)
      switch.slide_out_title()
    end)
    menu.btn6:on_press(function(self)
      demo:init_single(parameter.Highest_5Chain_3Min, 1, 'char/char1_new', 'stage/jungle1', false)
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
    menu.btn1 = ui.new_text{ parent=parent, x=80, y= 40, size=32, depth=-200, title='garbage10' }
    menu.btn2 = ui.new_text{ parent=parent, x=80, y= 80, size=32, depth=-200, title='garbage20' }
    menu.btn3 = ui.new_text{ parent=parent, x=80, y=120, size=32, depth=-200, title='garbage30' }
    menu.btn4 = ui.new_text{ parent=parent, x=80, y=160, size=32, depth=-200, title='garbage40' }
    menu.btn5 = ui.new_text{ parent=parent, x=80, y=200, size=32, depth=-200, title='garbage50' }
    menu.back = ui.new_text{ parent=parent, x=80, y=280, size=32, depth=-200, title='back' }
    menu.btn1:on_press(function(self)
      demo:init_single(parameter.WarningCondition_10, 1, 'char/char1_new', 'stage/jungle1', false)
      switch.slide_out_title()
    end)
    menu.btn2:on_press(function(self)
      demo:init_single(parameter.WarningCondition_20, 1, 'char/char1_new', 'stage/jungle1', false)
      switch.slide_out_title()
    end)
    menu.btn3:on_press(function(self)
      demo:init_single(parameter.WarningCondition_30, 1, 'char/char1_new', 'stage/jungle1', false)
      switch.slide_out_title()
    end)
    menu.btn4:on_press(function(self)
      demo:init_single(parameter.WarningCondition_40, 1, 'char/char1_new', 'stage/jungle1', false)
      switch.slide_out_title()
    end)
    menu.btn5:on_press(function(self)
      demo:init_single(parameter.WarningCondition_50, 1, 'char/char1_new', 'stage/jungle1', false)
      switch.slide_out_title()
    end)
    menu.back:on_press(function(self)
      switch.load_page('challengemenu')
    end)
  end
  
  -- TimeLimit
  if submode == parameter.TimeLimit then
  end
  
  return menu
end

return {
  init = init
}