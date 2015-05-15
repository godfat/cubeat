local ffi   = require 'ffi'
local C     = ffi.C
local view  = require 'rc/script/ui/view'
local ui    = require 'rc/script/ui/ui'
local switch= require 'rc/script/ui/demo/switch/switch'
local challenge= require 'rc/script/ui/demo/challenge/challenge'
local record   = require 'rc/script/ui/demo/challenge/record'
local parameter= require 'rc/script/ui/demo/challenge/parameter'

local root_

local function init(demo, parent, submode)
  local menu = {}
  
  root_ = view.new_sprite("blahblah", parent, 0, 0, true)
  root_:set_pos(480, 300)
  
  -- OneShotClear
  if submode == parameter.OneShotClear then
    local stage_num = parameter.OneShotClearStageNum  -- how many puzzle stage, use for create btn and puzzle_level in init_single function.
    
    menu.panel = ui.new_image{ parent = root_, path='blahblah', x=-320, y=-200, w=1000, h=500, alpha=168 }
    menu.panel:set_depth(-150)
    menu.panel:set_color(0,0,0)
    
    -- create button
    for i=1, stage_num do
      local k = 'stage' .. tostring(i)
      local mx = 150 * math.ceil(i/10)
      local my = 40 * math.mod(i-1, 10)
      print(mx, my)
      menu[k] = ui.new_text{ parent=root_, x=-450+mx, y=-200+my, size=32, depth=-200, title=k }
      if i==1 or record.load(parameter.clear, {submode=submode, puzzle_level=i}) then
        menu[k]:on_press(function(self)
          local level = i+1
          demo:init_single(parameter.OneShotClear, level, 'char/char1_new', 'stage/jungle1', false)
          challenge.set_puzzle_level(level)
          --switch.show_effect( {id="slide_out_title"} )
        end)
      else
        menu[k]:set_color(128,128,128)
      end
    end
    
    menu['unlimited'] = ui.new_text{ parent=root_, x=450, y=-200, size=32, depth=-200, title='unlimited' }
    menu['unlimited']:on_press(function(self)
      demo:init_single(parameter.OneShotClear, 2 + 20, 'char/char1_new', 'stage/jungle1', false)
      
      -- 2 + 20 hackzzzzz 
      
      challenge.set_puzzle_level(2)
      challenge.set_level_unlimited(true) -- puzzle unlimited level mode
      --switch.show_effect( {id="slide_out_title"} )
    end)
    
    menu.back = ui.new_text{ parent=root_, x=120, y=240, size=32, depth=-200, title='back' }
    menu.back:on_press(function(self)
      switch.load_page('challengemenu')
    end)
  end
  
  -- WarningCondition
  if submode == parameter.WarningCondition then
    menu.panel = ui.new_image{ parent = root_, path='area_rect', x=0, y=0, w=300, h=350 }
    menu.panel:set_depth(-150)
    menu.btn1 = ui.new_text{ parent=root_, x=80, y= 40, size=32, depth=-200, title='garbage20' }
    menu.btn2 = ui.new_text{ parent=root_, x=80, y= 80, size=32, depth=-200, title='garbage40' }
    menu.btn3 = ui.new_text{ parent=root_, x=80, y=120, size=32, depth=-200, title='garbage60' }
    menu.btn4 = ui.new_text{ parent=root_, x=80, y=160, size=32, depth=-200, title='garbage80' }
    menu.btn5 = ui.new_text{ parent=root_, x=80, y=200, size=32, depth=-200, title='garbage100' }
    menu.back = ui.new_text{ parent=root_, x=80, y=280, size=32, depth=-200, title='back' }
    menu.btn1:on_press(function(self)
      demo:init_single(parameter.WarningCondition_20, 1, 'char/char1_new', 'stage/jungle1', false)
      --switch.show_effect( {id="slide_out_title"} )
    end)
    if record.load(parameter.clear, {submode=parameter.WarningCondition_20}) then
      menu.btn2:on_press(function(self)
        demo:init_single(parameter.WarningCondition_40, 1, 'char/char1_new', 'stage/jungle1', false)
        --switch.show_effect( {id="slide_out_title"} )
      end)
    else
      menu.btn2:set_color(128,128,128)
    end
    if record.load(parameter.clear, {submode=parameter.WarningCondition_40}) then
      menu.btn3:on_press(function(self)
        demo:init_single(parameter.WarningCondition_60, 1, 'char/char1_new', 'stage/jungle1', false)
        --switch.show_effect( {id="slide_out_title"} )
      end)
    else
      menu.btn3:set_color(128,128,128)
    end
    if record.load(parameter.clear, {submode=parameter.WarningCondition_60}) then
      menu.btn4:on_press(function(self)
        demo:init_single(parameter.WarningCondition_80, 1, 'char/char1_new', 'stage/jungle1', false)
        --switch.show_effect( {id="slide_out_title"} )
      end)
    else
      menu.btn4:set_color(128,128,128)
    end
    if record.load(parameter.clear, {submode=parameter.WarningCondition_80}) then
      menu.btn5:on_press(function(self)
        demo:init_single(parameter.WarningCondition_100, 1, 'char/char1_new', 'stage/jungle1', false)
        --switch.show_effect( {id="slide_out_title"} )
      end)
    else
      menu.btn5:set_color(128,128,128)
    end
    menu.back:on_press(function(self)
      switch.load_page('challengemenu')
    end)
  end
  
  -- UnLimited
  if submode == parameter.UnLimited then
    menu.panel = ui.new_image{ parent = root_, path='area_rect', x=0, y=0, w=300, h=350 }
    menu.panel:set_depth(-150)
    menu.btn1 = ui.new_text{ parent=root_, x=80, y= 40, size=32, depth=-200, title='normal' }
    menu.btn2 = ui.new_text{ parent=root_, x=80, y= 80, size=32, depth=-200, title='countdown' }
    menu.back = ui.new_text{ parent=root_, x=80, y=280, size=32, depth=-200, title='back' }
    menu.btn1:on_press(function(self)
      demo:init_single(parameter.UnLimited_Normal, 1, 'char/char1_new', 'stage/jungle1', false)
      --switch.show_effect( {id="slide_out_title"} )
    end)
    menu.btn2:on_press(function(self)
      demo:init_single(parameter.UnLimited_Countdown, 1, 'char/char1_new', 'stage/jungle1', false)
      --switch.show_effect( {id="slide_out_title"} )
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