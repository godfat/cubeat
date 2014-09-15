local ffi    = require 'ffi'
local C      = ffi.C
local view   = require 'rc/script/ui/view'
local ui     = require 'rc/script/ui/ui'
local switch = require 'rc/script/ui/demo/switch/switch'
local event  = require 'rc/script/event/event'
local random = require 'rc/script/helper'.random
local basepath = require 'rc/script/helper'.basepath
local select_config = require 'rc/script/ui/demo/select/config'
local storystage    = require 'rc/script/ui/demo/storyend/config'

-------------------------------------------------------------------

local root_ = nil
local menu_

local function check_tutorial(ask)
  local filemark = io.open(basepath().."rc/config/tmp/tutored", "r")
  if filemark then
    io.write 'Lua: tutorial mark found.\n'
    filemark:close()
    return true
  else
    io.write 'Lua: tutorial mark NOT found.\n'
    ask:set_visible(true)
    filemark = io.open(basepath().."rc/config/tmp/tutored", "w") 
    filemark:write("1") -- just write something
    filemark:close()
    return false
  end
end

local function load_tutorials(parent, menu)
  for i = 1, 7 do
    local k = 'tuts_'..tostring(i)
    menu[k] = ui.new_image{ parent = parent,
      path='tut/'..i, x=0, y=0, w=1280, h=720 }
    menu[k]:set_depth(-100)
    menu[k]:set_visible(false)
    menu[k]:on_press(function(self)
      if i < 7 then
        menu['tuts_'..tostring(i+1)]:set_visible(true)
      end
      menu[k]:set_visible(false)
    end)
  end
end

local function hide_main_buttons()
  menu_.btn_story:set_visible(false)
  menu_.btn_vs_cpu:set_visible(false)
  menu_.btn_vs_ppl:set_visible(false)
  menu_.btn_chall:set_visible(false)
  menu_.btn_option:set_visible(false)
end

local function init(demo, parent)
  local menu = {}
  
  switch.show_master_menu()
  
  root_ = view.new_sprite("blahblah", parent, 0, 0, true)
  root_:set_pos(0, 0)
  
  --load_tutorials(demo:get_ui_scene(), menu)
  local ask_tutorial = ui.new_askbox { parent = demo:get_ui_scene(), title='Trying the tutorial first\n is HIGHLY recommended.', depth=-100 }
  ask_tutorial:set_visible(false)
  ask_tutorial.ok:change_text("Okay")
  ask_tutorial.cancel:change_text("Bah")
  
  ask_tutorial:on_press_ok(function(self)
    ask_tutorial:set_visible(false)
    demo:init_tutorial('char/char1_new', 'char/char2_new', 'stage/jungle1') -- test
    --switch.show_effect( {id="slide_out_title"} )
  end, 1)
  ask_tutorial:on_press_cancel(function(self)
    ask_tutorial:set_visible(false)
  end, 1)
  
  -- menu.btn_story   = ui.new_text{ parent = root_, title='story mode', x=0, y=0, size=32 }
  -- menu.btn_story:set_scale(1.5)
  -- menu.btn_vs_cpu  = ui.new_text{ parent = root_, title='player vs cpu', x=0, y=60, size=32 }
  -- menu.btn_vs_cpu:set_scale(1.5)
  -- menu.btn_vs_ppl  = ui.new_text{ parent = root_, title='player vs player', x=0, y=120, size=32 }
  -- menu.btn_vs_ppl:set_scale(1.5)
  -- menu.btn_chall   = ui.new_text{ parent = root_, title='challenge mode', x=0, y=180, size=32 }
  -- menu.btn_chall:set_scale(1.5)
  -- menu.btn_cpudemo = ui.new_text{ parent = root_, title='cpu demo mode', x=0, y=240, size=32 }
  -- menu.btn_cpudemo:set_scale(1.5)
  -- menu.btn_tut     = ui.new_text{ parent = root_, title='show tutorial', x=0, y=240, size=32 }
  -- menu.btn_tut:set_scale(1.5)
  -- menu.btn_prac    = ui.new_text{ parent = root_, title='chain practice', x=0, y=300, size=32 }
  -- menu.btn_prac:set_scale(1.5)
  -- menu.btn_quit    = ui.new_text{ parent = root_, title='quit', x=0, y=300, size=32 }
  -- menu.btn_quit:set_scale(1.5)
  --menu.btn_test    = ui.new_text{ parent = root_, title='test menu', x=360, y=300, size=32 }
  --menu.btn_test:set_scale(1.5)
  
  -- New positioning here
  menu.btn_story   = ui.new_image{ parent = root_, path='mainmenu/text_story', x=460, y=40, w=339, h=145 }
  menu.btn_story:set_pickable(false) -- so it won't interfere when overlapping with the underlying button
  menu.btn_vs_cpu  = ui.new_image{ parent = root_, path='mainmenu/text_vs_cpu', x=660, y=90, w=381, h=146 }
  menu.btn_vs_cpu:set_pickable(false)
  menu.btn_vs_ppl  = ui.new_image{ parent = root_, path='mainmenu/text_vs_ppl', x=700, y=350, w=453, h=137 }
  menu.btn_vs_ppl:set_pickable(false)
  menu.btn_chall   = ui.new_image{ parent = root_, path='mainmenu/text_chall', x=500, y=150, w=363, h=127 }
  menu.btn_chall:set_pickable(false)
  menu.btn_option  = ui.new_image{ parent = root_, path='mainmenu/text_option', x=450, y=350, w=290, h=127 }
  menu.btn_option:set_pickable(false)
  
  menu.btn_cpudemo = ui.new_text{ parent = root_, title='cpu demo mode', x=400, y=660, size=32 }
  menu.btn_cpudemo:set_scale(1.5)
  
  -- menu.btn_tut2     = ui.new_text{ parent = root_, title='tutorial test', x=360, y=180, size=32 }
  -- menu.btn_tut2:set_scale(1.5)
  menu.btn_backtostart = ui.new_text{ parent = root_, title='start screen', x=1050, y=670, size=32 }
  
  menu.btn_story:on_press(function(self)
    storystage.set_stage(1)
    switch.load_page('select', nil, { game_mode=99, level=0 })
  end)
  
  menu.btn_vs_cpu:on_press(function(self)
    if not check_tutorial(ask_tutorial) then return end
    switch.load_page('difficulty', nil, { game_mode = 1 })
    --switch.load_page('difficulty', {id="fade"}, { game_mode = 1 })
  end)
  
  menu.btn_vs_ppl:on_press(function(self)
    if not check_tutorial(ask_tutorial) then return end
    switch.load_page('select', nil, { game_mode = 0 })
  end)
  
  menu.btn_chall:on_press(function(self)
    switch.load_page('challengemenu')
  end)
  
  menu.btn_cpudemo:on_press(function(self) 
    local ch1 = random(6)+1
    local ch2 = random(6)+1
    select_config.ch_choose[1] = ch1
    select_config.ch_choose[2] = ch2
    demo:init_cpudemo("char/char"..ch1.."_new", "char/char"..ch2.."_new", "stage/jungle"..ch1)
    --switch.show_effect( {id="slide_out_title"} )
  end)
    
  -- menu.btn_tut:on_press(function(self) 
    -- switch.load_page('tutorial')
  -- end)
  
  -- menu.btn_tut2:on_press(function(self)
    -- demo:init_tutorial('char/char1_new', 'char/char1_new', 'stage/jungle1')
    -- switch.show_effect( {id="slide_out_title"} )
  -- end)
  
  -- menu.btn_prac:on_press(function(self)
    -- if not check_tutorial(ask_tutorial) then return end
    -- -- init SinglePlayer, in Submode 0, and Level is 2 by default
    -- -- the last false means "in_place" is false, there will be slide-in/out effects.
    -- demo:init_single(0, 2, 'char/char1_new', 'stage/jungle1', false)
    -- switch.show_effect( {id="slide_out_title"} )
  -- end)  
  
  menu.btn_backtostart:on_press(function(self)
    switch.load_page('startscreen')
  end)
  
  -- menu.btn_test:on_press(function(self)
    -- switch.load_page('testmenu')
  -- end)
  
  menu_ = menu
  hide_main_buttons()
  
  return menu
end

local function show_button(btn_name)
  if menu_[btn_name] then 
    menu_[btn_name]:set_visible(true)
  else
    print('Lua (mainmenu): no button named'..btn_name) 
  end
end

local function hide_button(btn_name)
  if menu_[btn_name] then 
    menu_[btn_name]:set_visible(false)
  else
    print('Lua (mainmenu): no button named'..btn_name) 
  end
end

return{
  init  = init,
  hide_main_buttons = hide_main_buttons,
  show_button = show_button,
  hide_button = hide_button
}