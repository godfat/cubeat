local ffi  = require 'ffi'
local C    = ffi.C
local view = require 'rc/script/ui/view'
local ui   = require 'rc/script/ui/ui'
require 'rc/script/ui/demo/defs'

----------------------------------------------------------------------------

local scene_
local menu_ = {}
local vorig_ 

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

function slide_out()
  local s = ffi.new("v2", 480, 300)
  local e = ffi.new("v2", 1480, 300)
  vorig_:tween_osine_pos(s, e, 1000)
end

function slide_in()
  local s = ffi.new("v2", -680, 300)
  local e = ffi.new("v2", 480, 300)
  vorig_:tween_isine_pos(s, e, 1000)
end

function init(demo)
  demo   = ffi.cast("Demo*", demo);
  scene_ = demo:get_ui_scene()
  vorig_ = view.new_sprite("blahblah", scene_, 1, 1, true) 
  vorig_:set_pos(480, 300)
  print '\n\nHello from Lua!\n\n'
    
  menu_.btn_vs_cpu = ui.new_button(vorig_, {
    title='player vs cpu', x=0, y=0, size=32, visible = false
  })
  menu_.btn_vs_cpu.title:set_scale(1.5)
  menu_.btn_vs_cpu:on_press(function(self) 
    demo:init_vs_cpu()
    --hide_everything()
  end)
  
  menu_.btn_vs_ppl = ui.new_button(vorig_, {
    title='player vs player', x=0, y=60, size=32, visible = false
  })
  menu_.btn_vs_ppl.title:set_scale(1.5)
  menu_.btn_vs_ppl:on_press(function(self) 
    demo:init_vs_ppl()
    --hide_everything()
  end)
  
  menu_.btn_quit = ui.new_button(vorig_, {
    title='quit', x=0, y=120, size=32, visible = false
  })
  menu_.btn_quit.title:set_scale(1.5)
  
end

function mainmenu()
  show_everything()
end
