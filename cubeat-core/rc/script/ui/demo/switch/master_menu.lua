local ffi  = require 'ffi'
local C    = ffi.C
local view = require 'rc/script/ui/view'
local ui   = require 'rc/script/ui/ui'
local mainmenu = nil -- deterred require at initialization stage
local switch   = nil -- deterred require at initialization stage
local select_config = require 'rc/script/ui/demo/select/config'
local storystage    = require 'rc/script/ui/demo/storyend/config'
local three_stage_alpha_tween = require 'rc/script/stage/base_function'.three_stage_alpha_tween
local event = require 'rc/script/event/event'

local has_blocker_  = false

-------------------------------------------

local demo_ 
local menu_ = {}
local root_

local function setup_focus_effect(item, btn_name)
  item:on_enter_focus(function()
    item:tween("SineCirc", "Scale", ffi.new("v3", 0.93, 0.93, 1), ffi.new("v3", 0.97, 0.97, 0.97), 1000, -1)
    if btn_name then
      mainmenu.show_button(btn_name)
    end
  end)
  item:on_leave_focus(function()
    -- shit, I don't have clear tween calls here in lua view scripts.
    item:tween("Linear", "Scale", ffi.new("v3", 0.93, 0.93, 1), ffi.new("v3", 0.93, 0.93, 1), 1)
    if btn_name then
      mainmenu.hide_button(btn_name)
    end
  end)
end

local function create_startscreen_clouds(parent)
  local produce_cloud2 = function()
    local local_cloud = ui.new_image{ parent=parent, path='mainmenu/cloud2', x=1280, y=60, w=294, h=121 }
    local_cloud:set_depth(100)
    local_cloud:tween("Linear", "Pos2D", ffi.new("v2", 1280, 60), ffi.new("v2", -294, 60), 22500)
    three_stage_alpha_tween(local_cloud, 22500, 4000)
  end
  produce_cloud2()
  event.on_timer("global", produce_cloud2, 17500, -1)
  
  local produce_cloud3 = function()
    local local_cloud = ui.new_image{ parent=parent, path='mainmenu/cloud3', x=1280, y=370, w=433, h=107 }
    local_cloud:set_depth(100)
    local_cloud:tween("Linear", "Pos2D", ffi.new("v2", 1280, 370), ffi.new("v2", -433, 370), 20000)
    three_stage_alpha_tween(local_cloud, 20000, 3600)
  end
  local produce_cloud3_timer = function()  
    produce_cloud3()  
    event.on_timer("global", produce_cloud3, 16000, -1)
  end
  event.on_timer("global", produce_cloud3_timer, 6000)
  
  local produce_cloud4 = function()
    local local_cloud = ui.new_image{ parent=parent, path='mainmenu/cloud4', x=1280, y=170, w=437, h=191 }
    local_cloud:set_depth(100)
    local_cloud:tween("Linear", "Pos2D", ffi.new("v2", 1280, 170), ffi.new("v2", -437, 170), 17500)
    three_stage_alpha_tween(local_cloud, 17500, 3500)
  end
  local produce_cloud4_timer = function()  
    produce_cloud4()  
    event.on_timer("global", produce_cloud4, 14000, -1)
  end
  event.on_timer("global", produce_cloud4_timer, 8500)
  
  local produce_cloud5 = function()
    local local_cloud = ui.new_image{ parent=parent, path='mainmenu/cloud5', x=1280, y=370, w=517, h=254 }
    local_cloud:set_depth(100)
    local_cloud:tween("Linear", "Pos2D", ffi.new("v2", 1280, 370), ffi.new("v2", -517, 370), 15000)
    three_stage_alpha_tween(local_cloud, 15000, 3000)
  end
  local produce_cloud5_timer = function()  
    produce_cloud5()  
    event.on_timer("global", produce_cloud5, 12000, -1)
  end
  event.on_timer("global", produce_cloud5_timer, 10500)
end

local function create_mainmenu_clouds(parent)
  local produce_cloud1 = function()
    local local_cloud = ui.new_image{ parent=parent, path='mainmenu/cloud1', x=1280, y=60, w=303, h=142 }
    local_cloud:set_depth(100)
    local_cloud:tween("Linear", "Pos2D", ffi.new("v2", 1280, 60), ffi.new("v2", -303, 60), 50000)
    three_stage_alpha_tween(local_cloud, 50000, 9000)
  end
  local produce_cloud1_timer = function()
    produce_cloud1()
    event.on_timer("global", produce_cloud1, 32500, -1)
  end
  event.on_timer("global", produce_cloud1_timer, 30000)
  
  local produce_cloud2 = function()
    local local_cloud = ui.new_image{ parent=parent, path='mainmenu/cloud2', x=1280, y=30, w=294, h=121 }
    local_cloud:set_depth(100)
    local_cloud:tween("Linear", "Pos2D", ffi.new("v2", 1280, 30), ffi.new("v2", -294, 30), 45000)
    three_stage_alpha_tween(local_cloud, 45000, 8000)
  end
  local produce_cloud2_timer = function()
    produce_cloud2()
    event.on_timer("global", produce_cloud2, 35000, -1)
  end
  event.on_timer("global", produce_cloud2_timer, 1000)
  
  local produce_cloud3 = function()
    local local_cloud = ui.new_image{ parent=parent, path='mainmenu/cloud3', x=1280, y=110, w=433, h=107 }
    local_cloud:set_depth(100)
    local_cloud:tween("Linear", "Pos2D", ffi.new("v2", 1280, 110), ffi.new("v2", -433, 110), 30000)
    three_stage_alpha_tween(local_cloud, 30000, 5400)
  end
  local produce_cloud3_timer = function()  
    produce_cloud3()  
    event.on_timer("global", produce_cloud3, 24000, -1)
  end
  event.on_timer("global", produce_cloud3_timer, 24000)
  
  local produce_cloud4 = function()
    local local_cloud = ui.new_image{ parent=parent, path='mainmenu/cloud4', x=1280, y=40, w=437, h=191 }
    local_cloud:set_depth(100)
    local_cloud:tween("Linear", "Pos2D", ffi.new("v2", 1280, 40), ffi.new("v2", -437, 40), 35000)
    three_stage_alpha_tween(local_cloud, 35000, 7000)
  end
  local produce_cloud4_timer = function()  
    produce_cloud4()  
    event.on_timer("global", produce_cloud4, 28000, -1)
  end
  event.on_timer("global", produce_cloud4_timer, 17000)
  
  local produce_cloud5 = function()
    local local_cloud = ui.new_image{ parent=parent, path='mainmenu/cloud5', x=1280, y=30, w=517, h=254 }
    local_cloud:set_depth(100)
    local_cloud:tween("Linear", "Pos2D", ffi.new("v2", 1280, 30), ffi.new("v2", -517, 30), 40000)
    three_stage_alpha_tween(local_cloud, 40000, 8000)
  end
  local produce_cloud5_timer = function()  
    produce_cloud5()  
    event.on_timer("global", produce_cloud5, 32000, -1)
  end
  event.on_timer("global", produce_cloud5_timer, 11000)
end

local function init(parent, demo)
  mainmenu = require 'rc/script/ui/demo/mainmenu/mainmenu'
  switch   = require 'rc/script/ui/demo/switch/switch'

  demo_ = demo
  root_ = view.new_sprite("blahblah", parent, 0, 0, false)

  menu_.title = ui.new_image{ parent = root_, path='title2', x=640, y=230, w=840, h=250, center=true }

  menu_.demobuild = ui.new_image{ parent = root_, path='demobuild', x=1000, y=780, w=246, h=76, center=true }
  menu_.demobuild:set_rotation(9)
  local s = ffi.new("v2", 1000, 740)
  local e = ffi.new("v2", 1000, 360)
  menu_.demobuild:tween("OElastic", "Pos2D", s, e, 2000, 0, nil, 1000) -- delay 1000

  menu_.teamname = ui.new_image{ parent = root_, path='teampsc', x=640, y=690, w=300, h=50, center=true }

  menu_.blocker  = ui.new_image{ parent = root_, path='nothing', w=1280, h=720, center=false }
  menu_.blocker:set_color(0, 0, 0)
  menu_.blocker:set_visible(false)
  
  menu_.transfer = ui.new_image{ parent = root_, path='nothing', x=640, y=-480, w=1280, h=960, center=true, depth=-500 }
  menu_.transfer:set_red(0)
  menu_.transfer:set_blue(0)
  menu_.transfer:set_green(0)
  menu_.transfer_title = ui.new_image{ parent = menu_.transfer._cdata, path='title', x=0, y=0, w=512, h=512, center=true }
  
  menu_.bg = ui.new_image{ parent=root_, path='mainmenu/bg', y=720, w=1280, h=720, center=false }
  menu_.bg:set_depth(150)
  
  menu_.skyroot_ = view.new_sprite("blahblah", root_, 0, 0, false)
  
  menu_.skyblue = ui.new_image { parent = menu_.skyroot_, path='mainmenu/skycolor', w=1280, h=1280, center=false }
  menu_.skyblue:set_depth(200)
  
  create_startscreen_clouds(menu_.skyroot_)
  create_mainmenu_clouds(menu_.bg._cdata)
  
  -- actual main menu sprites
  
  menu_.story = ui.new_image { parent = menu_.bg._cdata, path='mainmenu/story', w=600, h=558, x=250, y=290, center=true }
  menu_.story:set_depth(-50) 
  menu_.story:set_scale(0.93, 0.93)
  menu_.story:on_press(function(self)
    storystage.set_stage(1)
    switch.load_page('select', nil, { game_mode=99, level=0 })
  end)
  setup_focus_effect(menu_.story, 'btn_story')
  
  menu_.vscpu = ui.new_image { parent = menu_.bg._cdata, path='mainmenu/vscpu', w=500, h=508, x=1060, y=302, center=true }
  menu_.vscpu:set_depth(-50)
  menu_.vscpu:set_scale(0.93, 0.93)
  menu_.vscpu:on_press(function(self)
    --switch.load_page('difficulty', nil, { game_mode = 1 })
    switch.load_page('select', nil, { game_mode = 1 })
  end)
  setup_focus_effect(menu_.vscpu, 'btn_vs_cpu')
  
  menu_.challenge = ui.new_image { parent = menu_.bg._cdata, path='mainmenu/challenge', w=438, h=197, x=705, y=300, center=true }
  menu_.challenge:set_depth(-30) 
  menu_.challenge:set_scale(0.93, 0.93)
  menu_.challenge:on_press(function(self)
    switch.load_page('challengemenu')
  end)
  setup_focus_effect(menu_.challenge, 'btn_chall')
  
  menu_.vsplayer = ui.new_image { parent = menu_.bg._cdata, path='mainmenu/vsplayer', w=699, h=290, x=970, y=565, center=true }
  menu_.vsplayer:set_depth(-60)
  menu_.vsplayer:set_scale(0.93, 0.93)
  menu_.vsplayer:on_press(function(self)
    switch.load_page('select', nil, { game_mode = 0 })
  end)
  setup_focus_effect(menu_.vsplayer, 'btn_vs_ppl')

  menu_.option = ui.new_image { parent = menu_.bg._cdata, path='mainmenu/option', w=237, h=283, x=485, y=565, center=true }
  menu_.option:set_depth(-60)
  menu_.option:set_scale(0.93, 0.93)
  setup_focus_effect(menu_.option, 'btn_option')
  -- menu_.option:set_visible(false)

  menu_.grass1 = ui.new_image { parent = menu_.bg._cdata, path='mainmenu/grass_1', w=216, h=184, x=77, y=459, center=true }
  menu_.grass1:set_scale(0.925, 0.925)
  menu_.grass1:set_depth(-70)

  menu_.grass2 = ui.new_image { parent = menu_.bg._cdata, path='mainmenu/grass_2', w=906, h=170, x=920, y=648, center=true }
  menu_.grass2:set_scale(0.81, 0.85)
  menu_.grass2:set_depth(-110)
  -- menu_.grass2:set_visible(false)

  -- menu_.door = ui.new_image { parent = menu_.bg._cdata, path='mainmenu/door', w=959, h=340, x=-4, y=481, center=false }
  -- menu_.door:set_depth(-100)  
  -- menu_.door:set_scale(0.710, 0.72)
  -- menu_.door:set_visible(false)
  
  menu_.btn_quit    = ui.new_image{ parent = menu_.bg._cdata, path='mainmenu/text_quit', x=200, y=675, w=168, h=78, center=true }
  menu_.btn_quit:on_press(function(self) 
    demo:quit()
  end)
  menu_.btn_quit:set_scale(0.93, 0.93)
  setup_focus_effect(menu_.btn_quit)
  
end

local function cleanup()
  for k,v in pairs(menu_) do
    if v.remove_cb ~= nil then v:remove_cb() end
    v:remove()
  end
end

local function hide_all()
  for k,v in pairs(menu_) do
    if k ~= 'blocker' and k ~= 'transfer' and k ~= 'transfer_title' then
      v:set_visible(false)
    end
  end
end

local function show_all()
  for k,v in pairs(menu_) do
      if k ~= 'blocker' and k ~= 'transfer' and k ~= 'transfer_title' then
      v:set_visible(true)
    end
  end
end
-----------------------------------------------------------

local function fade_in_blocker()
  if has_blocker_ then return end
  menu_.blocker:tween("Linear", "Alpha", 0, 128, 500, 0, nil, 0)
  menu_.blocker:set_visible(true)
  has_blocker_ = true
end

local function fade_out_blocker()
  if not has_blocker_ then return end
  local cb = function()
    menu_.blocker:set_visible(false)
    has_blocker_ = false
  end
  menu_.blocker:tween("Linear", "Alpha", 128, 0, 500, 0, cb, 0)
end

local function show()
  show_all()
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
  
  hide_all()
  fade_out_blocker()
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
  ui.set_input_lock(true)
  local unlock_cb = function() ui.set_input_lock(false) end

  local s2 = ffi.new("v2", 640, 230)
  local e2 = ffi.new("v2", 640, -200)
  menu_.title:tween("ISine", "Pos2D", s2, e2, 500, 0, unlock_cb)

  local s3 = ffi.new("v2", 1000, 360)
  local e3 = ffi.new("v2", 1000, 780)
  menu_.demobuild:tween("ISine", "Pos2D", s3, e3, 750)

  local s4 = ffi.new("v2", 640, 690)
  local e4 = ffi.new("v2", 640, 780)
  menu_.teamname:tween("ISine", "Pos2D", s4, e4, 400)
  
  local s6 = ffi.new("v2", 0, 0)
  local e6 = ffi.new("v2", 0, -720)
  menu_.skyroot_:tween("ISine", "Pos2D", s6, e6, 1000)

  local s5 = ffi.new("v2", 0, 720)
  local e5 = ffi.new("v2", 0, 0)
  menu_.bg:tween("ISine", "Pos2D", s5, e5, 1000)
end

local function mainmenu_to_startscreen()
  ui.set_input_lock(true)
  local unlock_cb = function() ui.set_input_lock(false) end
  
  local s2 = ffi.new("v2", 640, -200)
  local e2 = ffi.new("v2", 640, 230)
  menu_.title:tween("OSine", "Pos2D", s2, e2, 500, 0, unlock_cb)

  local s3 = ffi.new("v2", 1000, 780)
  local e3 = ffi.new("v2", 1000, 360)
  menu_.demobuild:tween("OElastic", "Pos2D", s3, e3, 750)

  local s4 = ffi.new("v2", 640, 780)
  local e4 = ffi.new("v2", 640, 690)
  menu_.teamname:tween("OSine", "Pos2D", s4, e4, 400)

  local s6 = ffi.new("v2", 0, -720)
  local e6 = ffi.new("v2", 0, 0)
  menu_.skyroot_:tween("ISine", "Pos2D", s6, e6, 1000)
  
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
