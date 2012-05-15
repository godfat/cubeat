local ffi = require 'ffi'
local C   = ffi.C
local ui  = require 'rc/script/ui/ui'
require 'rc/script/ui/demo/defs'

----------------------------------------------------------------------------

local scene_
local menu_ = {}

local function hide_everything()
  for _, item in pairs(menu_) do
    item:set_visible(false)
  end
end

local function show_everything()
  for _, item in pairs(menu_) do
    item:set_visible(true)
  end
end

function init(demo)
  demo   = ffi.cast("Demo*", demo);
  scene_ = demo:get_ui_scene()
  print '\n\nHello from Lua!\n\n'
    
  menu_.btn_vs_cpu = ui.new_button(scene_, {
    title='player vs cpu', x=500, y=300, size=32, visible = false
  })
  menu_.btn_vs_cpu.title:set_scale(1.5)
  menu_.btn_vs_cpu:on_press(function(self) 
    demo:init_vs_cpu()
    hide_everything()
  end)
  
  menu_.btn_vs_ppl = ui.new_button(scene_, {
    title='player vs player', x=500, y=360, size=32, visible = false
  })
  menu_.btn_vs_ppl.title:set_scale(1.5)
  menu_.btn_vs_ppl:on_press(function(self) 
    demo:init_vs_ppl()
    hide_everything()
  end)
  
  menu_.btn_quit = ui.new_button(scene_, {
    title='quit', x=500, y=420, size=32, visible = false
  })
  menu_.btn_quit.title:set_scale(1.5)
  
end

function mainmenu()
  show_everything()
end
