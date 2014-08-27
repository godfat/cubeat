local ffi  = require 'ffi'
local C    = ffi.C
local view = require 'rc/script/ui/view'
local ui   = require 'rc/script/ui/ui'

local has_blocker_  = false

-------------------------------------------

local demo_ 
local menu_ = {}
local root_

local function init(parent, demo)
  demo_ = demo
  root_ = view.new_sprite("blahblah", parent, 0, 0, false)

  menu_.title = ui.new_image{ parent = root_, path='title2', x=640, y=150, w=840, h=250, center=true }

  menu_.demobuild = ui.new_image{ parent = root_, path='demobuild', x=1000, y=780, w=246, h=76, center=true }
  menu_.demobuild:set_rotation(9)
  local s = ffi.new("v2", 1000, 740)
  local e = ffi.new("v2", 1000, 280)
  menu_.demobuild:tween("OElastic", "Pos2D", s, e, 2000, 0, nil, 1000) -- delay 1000

  menu_.teamname = ui.new_image{ parent = root_, path='teampsc', x=640, y=690, w=300, h=50, center=true }

  menu_.blocker  = ui.new_image{ parent = root_, path='nothing', w=1280, h=720, center=false }
  menu_.blocker:set_color(0, 0, 0)
  menu_.blocker:set_alpha(0)
  
  menu_.transfer = ui.new_image{ parent = root_, path='nothing', x=640, y=-480, w=1280, h=960, center=true, depth=-500 }
  menu_.transfer:set_red(0)
  menu_.transfer:set_blue(0)
  menu_.transfer:set_green(0)
  menu_.transfer_title = ui.new_image{ parent = menu_.transfer._cdata, path='title', x=0, y=0, w=512, h=512, center=true }
  
  menu_.skyblue = ui.new_image { parent = root_, path='nothing', w=1280, h=720, center=false }
  menu_.skyblue:set_depth(100)
  menu_.skyblue:set_red(0) 
  menu_.skyblue:set_green(172) 
  
  menu_.bg = ui.new_image{ parent=root_, path='mainmenu/bg', y=720, w=1280, h=720, center=false }
  menu_.bg:set_depth(50)
end

local function cleanup()
  for k,v in pairs(menu_) do
    if v.remove_cb ~= nil then v:remove_cb() end
    v:remove()
  end
end

-----------------------------------------------------------

local function fade_in_blocker()
  menu_.blocker:tween("Linear", "Alpha", 0, 128, 500, 0, nil, 0)
  has_blocker_ = true
end

local function fade_out_blocker()
  menu_.blocker:tween("Linear", "Alpha", 128, 0, 500, 0, nil, 0)
  has_blocker_ = false
end

local function show()
  menu_.bg:set_alpha(255)
  menu_.skyblue:set_alpha(255)
end

-- Global Effect regarding the main background + title etc.

local function slide_out(s, e, dur)
  root_:tween("OSine", "Pos2D", s, e, dur)
end

local function slide_in(s, e, dur)
  root_:tween("ISine", "Pos2D", s, e, dur)
end

local slide_out_transfer = function(effect)
  local s1 = ffi.new("v2", 640,  360)
  local e1 = ffi.new("v2", 640, -480)
  menu_.transfer:tween("ISine", "Pos2D", s1, e1, 1000, 0, nil, 1000)
end

local slide_out_transfer_to_talk = function(effect, cb)
  ui.set_input_lock(true)
  
  menu_.bg:set_alpha(0)
  menu_.skyblue:set_alpha(0)
  demo_:load_stage(effect.stage_id)
  
  local s1 = ffi.new("v2", 640,  360)
  local e1 = ffi.new("v2", 640, -480)
  menu_.transfer:tween("ISine", "Pos2D", s1, e1, 1000, 0, cb, 1000)
  
  print('root_ position when slide out to talk: ('..tostring(root_:get_pos_x())..','..tostring(root_:get_pos_y())..')')
end

local slide_in_transfer = function(effect)
  local s1 = ffi.new("v2", 640, -480)
  local e1 = ffi.new("v2", 640,  360)
  menu_.transfer:tween("OElastic", "Pos2D", s1, e1, 2000, 0, effect.cb, 0)
  
  print('root_ position when slide out to talk: ('..tostring(root_:get_pos_x())..','..tostring(root_:get_pos_y())..')')
end

local function startscreen_to_mainmenu()
  local s2 = ffi.new("v2", 640, 150)
  local e2 = ffi.new("v2", 640, -200)
  menu_.title:tween("ISine", "Pos2D", s2, e2, 500)

  local s3 = ffi.new("v2", 1000, 280)
  local e3 = ffi.new("v2", 1000, 780)
  menu_.demobuild:tween("ISine", "Pos2D", s3, e3, 750)

  local s4 = ffi.new("v2", 640, 690)
  local e4 = ffi.new("v2", 640, 780)
  menu_.teamname:tween("ISine", "Pos2D", s4, e4, 400)

  local s5 = ffi.new("v2", 0, 720)
  local e5 = ffi.new("v2", 0, 0)
  menu_.bg:tween("ISine", "Pos2D", s5, e5, 1000)
end

local function mainmenu_to_startscreen()
  local s2 = ffi.new("v2", 640, -200)
  local e2 = ffi.new("v2", 640, 150)
  menu_.title:tween("OSine", "Pos2D", s2, e2, 500)

  local s3 = ffi.new("v2", 1000, 780)
  local e3 = ffi.new("v2", 1000, 280)
  menu_.demobuild:tween("OElastic", "Pos2D", s3, e3, 750)

  local s4 = ffi.new("v2", 640, 780)
  local e4 = ffi.new("v2", 640, 690)
  menu_.teamname:tween("OSine", "Pos2D", s4, e4, 400)

  local s5 = ffi.new("v2", 0, 0)
  local e5 = ffi.new("v2", 0, 720)
  menu_.bg:tween("ISine", "Pos2D", s5, e5, 1000)
end

return {
  init = init,
  cleanup = cleanup,
  fade_in_blocker = fade_in_blocker,
  fade_out_blocker = fade_out_blocker,
  show = show,
  slide_out = slide_out,
  slide_in  = slide_in,
  slide_in_transfer = slide_in_transfer,
  slide_out_transfer = slide_out_transfer,
  slide_out_transfer_to_talk = slide_out_transfer_to_talk,
  mainmenu_to_startscreen = mainmenu_to_startscreen,
  startscreen_to_mainmenu = startscreen_to_mainmenu,
}
