local ffi  = require 'ffi'
local C    = ffi.C
local view = require 'rc/script/ui/view'
local ui   = require 'rc/script/ui/ui'
require 'rc/script/ui/demo/defs'

----------------------------------------------------------------------------

local scene_
local menu_ = {}
local vorig_ 
local blocker_
local title_
local demobuild_
local teamname_

local tuts_ = {}

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

local function load_tutorials()
  for i = 1, 7 do 
    tuts_[i] = ui.new_image{ parent = scene_,
      path='tut/'..i, x=0, y=0, w=1280, h=720 }
    tuts_[i]:set_depth(-100)
    tuts_[i]:set_visible(false)
    tuts_[i]:on_press(function(self) 
      if i < 7 then
        tuts_[i+1]:set_visible(true)
      end
      tuts_[i]:set_visible(false)
    end)
  end
end

------------------------------------------

function slide_out(inplace)
  if inplace then
    hide_everything()
  else
    local s1 = ffi.new("v2", 480, 300)
    local e1 = ffi.new("v2", 1480, 300)
    vorig_:tween_osine_pos(s1, e1, 1000)
    
    local s2 = ffi.new("v2", 640, 150)
    local e2 = ffi.new("v2", 640, -200)
    title_:tween_isine_pos(s2, e2, 500)
    
    local s3 = ffi.new("v2", 1000, 280)
    local e3 = ffi.new("v2", 1000, 780)
    demobuild_:tween_isine_pos(s3, e3, 750)
    
    local s4 = ffi.new("v2", 640, 690)
    local e4 = ffi.new("v2", 640, 780)
    teamname_:tween_isine_pos(s4, e4, 400)
    
    blocker_:tween_linear_alpha(96, 0, 500, 0, nil, 0)
  end
end

function slide_in()
  local s = ffi.new("v2", -680, 300)
  local e = ffi.new("v2", 480, 300)
  vorig_:tween_isine_pos(s, e, 1000)
  
  local s2 = ffi.new("v2", 640, -200)
  local e2 = ffi.new("v2", 640, 150)
  title_:tween_osine_pos(s2, e2, 500)
  
  local s3 = ffi.new("v2", 1000, 780)
  local e3 = ffi.new("v2", 1000, 280)
  demobuild_:tween_elastic_pos(s3, e3, 750)
  
  local s4 = ffi.new("v2", 640, 780)
  local e4 = ffi.new("v2", 640, 690)
  teamname_:tween_osine_pos(s4, e4, 400)
  
  blocker_:tween_linear_alpha(0, 96, 500, 0, nil, 0)
end

function init(demo)
  demo   = ffi.cast("Demo*", demo);
  scene_ = demo:get_ui_scene()
  vorig_ = view.new_sprite("blahblah", scene_, 1, 1, true) 
  vorig_:set_pos(480, 300)
  print '\n\nHello from Lua!\n\n'
  
  load_tutorials();
  
  --------------------------------------
  
  title_   = ui.new_image{ parent = scene_,
    path='title2', x=640, y=150, w=840, h=250, center=true}
    
  demobuild_ = ui.new_image{ parent = scene_,
    path='demobuild', x=1000, y=780, w=246, h=76, center=true}
  demobuild_:set_rotation(9)
  local s = ffi.new("v2", 1000, 740)
  local e = ffi.new("v2", 1000, 280)
  demobuild_:tween_elastic_pos(s, e, 2000, 0, nil, 1000) -- delay 1000
    
  teamname_ = ui.new_image{ parent = scene_,
    path='teampsc', x=640, y=690, w=300, h=50, center=true}
  
  blocker_ = view.new_sprite("blahblah", scene_, 1280, 720, false)
  blocker_:set_pos(0, 0)
  blocker_:set_color(0, 0, 0)
  blocker_:set_alpha(96)

  --------------------------------------
  
  local score_list = ui.new_list{ parent=scene_, depth=-1000, visible=false }
  score_list:load_list('score')
  score_list:on_press_back(function(self)
    show_everything()
    title_:set_visible(true)
    score_list:set_visible(false)
    score_list:save_list('score')
  end)
  
  local target  = ui.new_image{ parent=score_list._cdata, x=0, y= 0, center=true }
  local btn1    = ui.new_text { parent=score_list._cdata, x=0, y= 90, center=true, title='alpha' }
  local btn2    = ui.new_text { parent=score_list._cdata, x=0, y=120, center=true, title='pos' }
  local btn3    = ui.new_text { parent=score_list._cdata, x=0, y=150, center=true, title='rotation'}
  local btn4    = ui.new_text { parent=score_list._cdata, x=0, y=180, center=true, title='scale'}
  
  local alpha_s = ffi.new("value1", 255)
  local alpha_e = ffi.new("value1", 50)
  local tween_alpha  = function(self) target:tween('OElastic', 'Alpha', alpha_s, alpha_e, 2000, 0, nil, 0) end
  btn1:on_press(tween_alpha)
  
  local pos_s = ffi.new("value2",   0, 0)
  local pos_e = ffi.new("value2", 100, 0)
  local tween_pos = function(self) target:tween('OElastic', 'Pos2D', pos_s, pos_e, 2000, 0, nil, 0) end
  btn2:on_press(tween_pos)
  
  local rotation_s  = ffi.new("value3", 0, 0,   0)
  local rotation_e  = ffi.new("value3", 0, 0, -180)
  local tween_rotation = function(self) target:tween('OElastic', 'Rotation', rotation_s, rotation_e, 2000, 0, nil, 0) end
  btn3:on_press(tween_rotation)
  
  local scale_s = ffi.new("value3", 1, 1, 0)
  local scale_e = ffi.new("value3", 2, 2, 0)
  local tween_scale = function(self) target:tween('OElastic', 'Scale', scale_s, scale_e, 2000, 0, nil, 0) end
  btn4:on_press(tween_scale)
  
  --------------------------------------

  menu_.btn_vs_cpu = ui.new_text{ parent = vorig_,
    title='player vs cpu', x=0, y=0, size=32, visible = false
  }
  menu_.btn_vs_cpu:set_scale(1.5)
  menu_.btn_vs_cpu:on_press(function(self) 
    demo:init_vs_cpu()
    --hide_everything()
  end)
  
  menu_.btn_vs_ppl = ui.new_text{ parent = vorig_,
    title='player vs player', x=0, y=60, size=32, visible = false
  }
  menu_.btn_vs_ppl:set_scale(1.5)
  menu_.btn_vs_ppl:on_press(function(self) 
    demo:init_vs_ppl()
    --hide_everything()
  end)
  
  menu_.btn_cpudemo = ui.new_text{ parent = vorig_,
    title='cpu demo mode', x=0, y=120, size=32, visible = false
  }
  menu_.btn_cpudemo:set_scale(1.5)
  menu_.btn_cpudemo:on_press(function(self) 
    demo:init_cpudemo()
    --hide_everything()
  end)
  
  menu_.btn_tut = ui.new_text{ parent = vorig_,
    title='show tutorial', x=0, y=180, size=32, visible = false
  }
  menu_.btn_tut:set_scale(1.5)
  menu_.btn_tut:on_press(function(self) 
    tuts_[1]:set_visible(true)
    --hide_everything()
  end)
  
  menu_.btn_quit = ui.new_text{ parent = vorig_,
    title='quit', x=0, y=240, size=32, visible = false
  }
  menu_.btn_quit:set_scale(1.5)
  menu_.btn_quit:on_press(function(self) 
    demo:quit()
  end)
  
  menu_.btn_score = ui.new_text{ parent = vorig_,
    title='score', x=0, y=300, size=32, visible = false
  }
  menu_.btn_score:set_scale(1.5)
  menu_.btn_score:on_press(function(self)
    hide_everything()
    title_:set_visible(false)
    score_list:set_visible(true)
  end)
  
end

function mainmenu()
  show_everything()
end
