local ffi   = require 'ffi'
local C     = ffi.C
local view  = require 'rc/script/ui/view'
local ui    = require 'rc/script/ui/ui'
local switch= require 'rc/script/ui/demo/switch/switch'
require 'rc/script/ui/demo/defs'


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

local function init(demo, parent)
  local menu = {}
  
  --load_tutorials(demo:get_ui_scene(), menu)
  
  menu.btn_vs_cpu = ui.new_text{ parent = parent,
    title='player vs cpu', x=0, y=0, size=32, visible = true
  }
  menu.btn_vs_cpu:set_scale(1.5)
  menu.btn_vs_cpu:on_press(function(self) 
    demo:init_vs_cpu("char/char1_new", "char/char2_new", "stage/jungle")
  end)
  
  menu.btn_vs_ppl = ui.new_text{ parent = parent,
    title='player vs player', x=0, y=60, size=32, visible = true
  }
  menu.btn_vs_ppl:set_scale(1.5)
  menu.btn_vs_ppl:on_press(function(self) 
    demo:init_vs_ppl("char/char1_new", "char/char2_new", "stage/jungle")
  end)
  
  menu.btn_cpudemo = ui.new_text{ parent = parent,
    title='cpu demo mode', x=0, y=120, size=32, visible = true
  }
  menu.btn_cpudemo:set_scale(1.5)
  menu.btn_cpudemo:on_press(function(self) 
    demo:init_cpudemo("char/char1_new", "char/char2_new", "stage/jungle")
  end)
  
  menu.btn_tut = ui.new_text{ parent = parent,
    title='show tutorial', x=0, y=180, size=32, visible = true
  }
  menu.btn_tut:set_scale(1.5)
  menu.btn_tut:on_press(function(self) 
    --menu['tuts_1']:set_visible(true)
    switch.load_page(demo, 'tutorial')
  end)
  
  menu.btn_quit = ui.new_text{ parent = parent,
    title='quit', x=0, y=240, size=32, visible = true
  }
  menu.btn_quit:set_scale(1.5)
  menu.btn_quit:on_press(function(self) 
    demo:quit()
  end)
  
  menu.btn_test = ui.new_text{ parent = parent,
    title='test menu', x=0, y=300, size=32, visible = true
  }
  menu.btn_test:set_scale(1.5)
  menu.btn_test:on_press(function(self)
    switch.load_page(demo, 'testmenu')
  end)
  
  return menu
end

return{
  init  = init
}