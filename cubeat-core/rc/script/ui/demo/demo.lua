local ffi   = require 'ffi'
local C     = ffi.C
local view  = require 'rc/script/ui/view'
local ui    = require 'rc/script/ui/ui'
local switch= require 'rc/script/ui/demo/switch/switch'
require 'rc/script/ui/demo/defs'
require 'rc/script/strict'

----------------------------------------------------------------------------

local demo_
local scene_
local menu_ = {}

-- test
local ask_panel_     = nil

local function hide_everything()
  for _, item in pairs(switch.get_page_obj()) do
    item:set_visible(false)
  end
end

local function show_everything()
  for _, item in pairs(switch.get_page_obj()) do
    item:set_visible(true)
  end
end

------------------------------------------

function slide_out(inplace)
  if inplace then
    hide_everything()
  else
    switch.slide_out_page_obj()
  end
end

function slide_in()
  switch.load_page('mainmenu', 'in')
  switch.slide_in_page_obj()
  
  if ask_panel_ then ask_panel_:set_visible(false) end
end

function init(demo)
  demo_  = ffi.cast("Demo*", demo);
  scene_ = demo_:get_ui_scene()
  
  --------------------------------------
  
  switch.init(scene_, demo_)
  
  --preload all ui
  --switch.load_page('tutorial' )
  --switch.load_page('score'    )
  --switch.load_page('tweentest')
  switch.load_page('select'   )
  --switch.load_page('testmenu' )
  switch.load_page('mainmenu' )
end

-----------------------------------------
-- This really should just be a temporary solution, a separated menu page should be better

function tutorial_update(state, data)
  if not ask_panel_ then
    ask_panel_ = ui.new_askbox{ parent = scene_, title='<test>', center=true, visible=false }
    ask_panel_._cdata:set_size(1024, 576)
    ask_panel_._cdata:set_alpha(192)
    ask_panel_.text:set_scale(1.0)
    ask_panel_.panel:set_texture('nothing')
    ask_panel_.panel:set_alpha(0)
    ask_panel_.ok:set_scale(1.5)
    ask_panel_.ok:set_pos(-75, 60)
    ask_panel_.cancel:set_scale(1.5)
    ask_panel_.cancel:set_pos(75, 60)
    
    ask_panel_.text2 = ui.new_text { 
      parent = ask_panel_._cdata, 
      title  = "Shoot [left button] these:                              Match these:\n\n          colored cubes\n\n          colored cubes with chain\n\n          garbages (won't match)\n\nHaste: [right button]                                         Pause: [middle button]",
    }
    ask_panel_.text2:set_depth(-50)
    ask_panel_.text2:set_pos(-460, -240)
    
    ask_panel_.cube1 = ui.new_image { parent = ask_panel_.text2._cdata, path = 'cubes/cube1', w=48, h=48 }
    ask_panel_.cube1:set_color(255, 0, 0)
    ask_panel_.cube1:set_pos(10, 45) 
    ask_panel_.cube2 = ui.new_image { parent = ask_panel_.text2._cdata, path = 'cubes/cube2', w=48, h=48 }
    ask_panel_.cube2:set_color(255, 0, 0)
    ask_panel_.cube2:tween("SineCirc", "GradientEmissive", 0, 144, 500, -1)
    ask_panel_.cube2:set_pos(10, 95) 
    ask_panel_.cube3 = ui.new_image { parent = ask_panel_.text2._cdata, path = 'cubes/cube3', w=48, h=48 }
    ask_panel_.cube3:set_pos(10, 145) 
    
    ask_panel_.cube4 = ui.new_image { parent = ask_panel_.text2._cdata, path = 'cubes/cube1', w=48, h=48 }
    ask_panel_.cube5 = ui.new_image { parent = ask_panel_.text2._cdata, path = 'cubes/cube2', w=48, h=48 }
    ask_panel_.cube6 = ui.new_image { parent = ask_panel_.text2._cdata, path = 'cubes/cube3', w=48, h=48 }
    ask_panel_.cube7 = ui.new_image { parent = ask_panel_.text2._cdata, path = 'cubes/cube1', w=48, h=48 }
    ask_panel_.cube8 = ui.new_image { parent = ask_panel_.text2._cdata, path = 'cubes/cube2', w=48, h=48 }
    ask_panel_.cube9 = ui.new_image { parent = ask_panel_.text2._cdata, path = 'cubes/cube3', w=48, h=48 }
    ask_panel_.cube0 = ui.new_image { parent = ask_panel_.text2._cdata, path = 'cubes/cube4', w=48, h=48 }
    ask_panel_.cube4:set_color(0, 255, 0)
    ask_panel_.cube5:set_color(0, 255, 0)
    ask_panel_.cube6:set_color(0, 255, 0)
    ask_panel_.cube7:set_color(0, 0, 255)
    ask_panel_.cube8:set_color(0, 0, 255)
    ask_panel_.cube9:set_color(0, 0, 255)
    ask_panel_.cube0:set_color(0, 0, 255)
    ask_panel_.cube4:set_pos(550, 60)
    ask_panel_.cube4:tween("Linear", "Pos2D", ffi.new("v2", 550, 30), ffi.new("v2", 550, 60), 1000, -1)
    ask_panel_.cube5:set_pos(598, 60)
    ask_panel_.cube6:set_pos(646, 60)
    ask_panel_.cube7:set_pos(550, 130)
    ask_panel_.cube8:set_pos(598, 130)
    ask_panel_.cube9:set_pos(646, 130)
    ask_panel_.cube0:set_pos(694, 130)
    ask_panel_.cube9:tween("Linear", "Pos2D", ffi.new("v2", 646, 105), ffi.new("v2", 646, 130), 1000, -1)
  end
  
  print("Lua: ".."tutorial state "..tostring(state))
  
  if state == 0 then
    ask_panel_._cdata:set_size(1024, 576)
    
    ask_panel_:set_title("Tutorial is 90 seconds long.\nTry to make 8 or more matches in the first 20 seconds.")
    ask_panel_.text:set_pos(0, 90)
    ask_panel_:set_visible(true)
    ask_panel_.text2:set_visible(true)
    ask_panel_.cube4:set_visible(true) -- ?????? why not affected by parent?
    ask_panel_.cube5:set_visible(true) -- ?????? why not affected by parent?
    ask_panel_.cube6:set_visible(true) -- ?????? why not affected by parent?
    ask_panel_.cube7:set_visible(true) -- ?????? why not affected by parent?
    ask_panel_.cube8:set_visible(true) -- ?????? why not affected by parent?
    ask_panel_.cube9:set_visible(true) -- ?????? why not affected by parent?
    ask_panel_.cube0:set_visible(true) -- ?????? why not affected by parent?
    
    ask_panel_.ok:change_text('OK')
    ask_panel_.ok:set_pos(0, 200)
    ask_panel_.cancel:set_visible(false)
    
    ask_panel_:on_press_ok(function(self)
      ask_panel_:set_visible(false)
      ask_panel_.text:set_pos(0, -30)
      ask_panel_.text2:set_visible(false)
      ask_panel_.cube4:set_visible(false) -- ?????? why not affected by parent?
      ask_panel_.cube5:set_visible(false) -- ?????? why not affected by parent?
      ask_panel_.cube6:set_visible(false) -- ?????? why not affected by parent?
      ask_panel_.cube7:set_visible(false) -- ?????? why not affected by parent?
      ask_panel_.cube8:set_visible(false) -- ?????? why not affected by parent?
      ask_panel_.cube9:set_visible(false) -- ?????? why not affected by parent?
      ask_panel_.cube0:set_visible(false) -- ?????? why not affected by parent?
      demo_:eventual_resume()
    end, 1)
    demo_:eventual_pause()
  
  elseif state == 1 then
    ask_panel_._cdata:set_size(800, 480)
    ask_panel_:set_title("You only made "..data.." matches.\nTry not to think too much.")
    ask_panel_:set_visible(true)
    
    ask_panel_.ok:change_text('retry')
    ask_panel_.ok:set_pos(-75, 60)
    ask_panel_.cancel:change_text('leave')
    ask_panel_.cancel:set_visible(true)
    
    ask_panel_:on_press_ok(function(self)
      ask_panel_:set_visible(false)
      demo_:eventual_resume()
      demo_:reinit()
    end, 1)
    ask_panel_:on_press_cancel(function(self)
      ask_panel_:set_visible(false)
      demo_:eventual_resume()
      demo_:endgame()
    end, 1)
    demo_:eventual_pause()
  
  elseif state == 2 then 
    if data > 4000 then
      ask_panel_:set_title("Tutorial finished.\nSeems like you are doing just fine.")
    else
      ask_panel_:set_title("You survived tutorial, but try harder\nto make bigger matches and bigger chains.") 
    end
    ask_panel_._cdata:set_size(800, 480)
    ask_panel_:set_visible(true)
  
    ask_panel_.ok:change_text('retry')
    ask_panel_.ok:set_pos(-75, 60)
    ask_panel_.cancel:change_text('leave')
    ask_panel_.cancel:set_visible(true)
    
    ask_panel_:on_press_ok(function(self)
      ask_panel_:set_visible(false)
      demo_:eventual_resume()
      demo_:reinit()
    end, 1)
    ask_panel_:on_press_cancel(function(self)
      ask_panel_:set_visible(false)
      demo_:eventual_resume()
      demo_:endgame()
    end, 1)
    demo_:eventual_pause()
    
  end
end

function mainmenu()
  show_everything()
end
