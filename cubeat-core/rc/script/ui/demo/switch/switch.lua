local ffi  = require 'ffi'
local C    = ffi.C
local view = require 'rc/script/ui/view'
local ui   = require 'rc/script/ui/ui'


local page_obj_ = {}
local to_be_delete_
local data_ = nil
local current_page_ = nil
local starter_page_ = nil
local has_blocker_  = false
--
local game_demo_
local vorig_
local root_
local title_
local demobuild_
local teamname_
local blocker_
local transfer_
local transfer_title_

-- mainmenu bg --
local skyblue_
local bg_
--
local effect_ = {}
local effect_count_ = 0

------------------------------------------------------------

local function init(parent, demo)
  game_demo_ = demo

  vorig_ = view.new_sprite("blahblah", parent, 0, 0, false)
  --vorig_:set_pos(480, 300)
  print '\n\nHello from Lua!\n\n'
  
  root_ = view.new_sprite("blahblah", parent, 0, 0, false)

  title_   = ui.new_image{ parent = root_,
    path='title2', x=640, y=150, w=840, h=250, center=true}

  demobuild_ = ui.new_image{ parent = root_,
    path='demobuild', x=1000, y=780, w=246, h=76, center=true}
  demobuild_:set_rotation(9)
  local s = ffi.new("v2", 1000, 740)
  local e = ffi.new("v2", 1000, 280)
  demobuild_:tween("OElastic", "Pos2D", s, e, 2000, 0, nil, 1000) -- delay 1000

  teamname_ = ui.new_image{ parent = root_,
    path='teampsc', x=640, y=690, w=300, h=50, center=true}

  blocker_ = view.new_sprite("blahblah", root_, 1280, 720, false)
  blocker_:set_pos(0, 0)
  blocker_:set_color(0, 0, 0)
  blocker_:set_alpha(0)
  
  transfer_ = ui.new_image{ parent=root_, path=' ', x=640, y=-480, w=1280, h=960, center=true, depth=-500 }
  transfer_:set_red(0)
  transfer_:set_blue(0)
  transfer_:set_green(0)
  transfer_title_ = ui.new_image{ parent=transfer_._cdata, path='title', x=0, y=0, w=512, h=512, center=true }
  
  skyblue_ = ui.new_image { parent = root_, path='nothing', w=1280, h=720, center=false }
  skyblue_:set_depth(100)
  skyblue_:set_red(0) 
  skyblue_:set_green(200) 
  
  bg_ = ui.new_image{ parent=root_, path='mainmenu/bg', y=720, w=1280, h=720, center=false }
  bg_:set_depth(50)
end

------------------------------------------------------------

local function fade_in_blocker()
  blocker_:tween("Linear", "Alpha", 0, 128, 500, 0, nil, 0)
  has_blocker_ = true
end

local function fade_out_blocker()
  blocker_:tween("Linear", "Alpha", 128, 0, 500, 0, nil, 0)
  has_blocker_ = false
end

local function add_effect_count()
  effect_count_ = effect_count_ + 1
end

local function remove_effect_count()
  effect_count_ = effect_count_ - 1
  if effect_count_ == 0 then ui.set_input_lock(false) end
end

------------------------------------------------------------
-- EFFECT

effect_.fade = function(effect)
  ui.set_input_lock(true)

  for k,v1 in pairs(page_obj_) do
    add_effect_count()
    v1:tween("Linear", "Alpha",   0, 255, 1500, 0, remove_effect_count, 0)
  end
  for k,v2 in pairs(to_be_delete_) do
    add_effect_count()
    v2:set_visible(true)
    v2:tween("Linear", "Alpha", 255,   0, 1500, 0, function() v2:set_visible(false) remove_effect_count() end, 0)
  end
end

effect_.slide_out = function(effect)
  local s1 = ffi.new("v2", 0, 0)
  local e1 = ffi.new("v2", 2000, 0)
  vorig_:tween("OSine", "Pos2D", s1, e1, 1000)
  root_:tween("OSine", "Pos2D", s1, e1, 1000)
  fade_out_blocker()
end

effect_.slide_in = function(effect)
  local s1 = ffi.new("v2", -2160, 0)
  local e1 = ffi.new("v2", 0, 0)
  vorig_:tween("ISine", "Pos2D", s1, e1, 1000)
  root_:tween("ISine", "Pos2D", s1, e1, 1000)
  fade_in_blocker()
end

effect_.slide_in_to_talk = function(effect)
  ui.set_input_lock(true)
  
  local s1 = ffi.new("v2", -2160, 0)
  local e1 = ffi.new("v2", 0, 0)
  starter_page_ = require ('rc/script/ui/demo/talk/talk')
  local function cb() starter_page_.starter(page_obj_) ui.set_input_lock(false) end
  vorig_:tween("ISine", "Pos2D", s1, e1, 1000, 0, cb, 0)
  fade_in_blocker()
end

effect_.slide_out_title = function(effect)
end

effect_.slide_in_title = function(effect)
end

effect_.slide_out_transfer = function(effect)
  local s1 = ffi.new("v2", 640,  360)
  local e1 = ffi.new("v2", 640, -480)
  transfer_:tween("ISine", "Pos2D", s1, e1, 1000, 0, nil, 1000)
end

effect_.slide_out_transfer_to_talk = function(effect)
  ui.set_input_lock(true)
  
  local s1 = ffi.new("v2", 640,  360)
  local e1 = ffi.new("v2", 640, -480)
  starter_page_ = require ('rc/script/ui/demo/talk/talk')
  local function cb() starter_page_.starter(page_obj_) ui.set_input_lock(false) end
  transfer_:tween("ISine", "Pos2D", s1, e1, 1000, 0, cb, 1000)
end

effect_.slide_in_transfer = function(effect)
  local s1 = ffi.new("v2", 640, -480)
  local e1 = ffi.new("v2", 640,  360)
  transfer_:tween("OElastic", "Pos2D", s1, e1, 2000, 0, effect.cb, 0)
end

-- Global Effect regarding the main background + title etc.

local function startscreen_to_mainmenu()
  local s2 = ffi.new("v2", 640, 150)
  local e2 = ffi.new("v2", 640, -200)
  title_:tween("ISine", "Pos2D", s2, e2, 500)

  local s3 = ffi.new("v2", 1000, 280)
  local e3 = ffi.new("v2", 1000, 780)
  demobuild_:tween("ISine", "Pos2D", s3, e3, 750)

  local s4 = ffi.new("v2", 640, 690)
  local e4 = ffi.new("v2", 640, 780)
  teamname_:tween("ISine", "Pos2D", s4, e4, 400)

  local s5 = ffi.new("v2", 0, 720)
  local e5 = ffi.new("v2", 0, 0)
  bg_:tween("ISine", "Pos2D", s5, e5, 1000)
end

local function mainmenu_to_startscreen()
  local s2 = ffi.new("v2", 640, -200)
  local e2 = ffi.new("v2", 640, 150)
  title_:tween("OSine", "Pos2D", s2, e2, 500)

  local s3 = ffi.new("v2", 1000, 780)
  local e3 = ffi.new("v2", 1000, 280)
  demobuild_:tween("OElastic", "Pos2D", s3, e3, 750)

  local s4 = ffi.new("v2", 640, 780)
  local e4 = ffi.new("v2", 640, 690)
  teamname_:tween("OSine", "Pos2D", s4, e4, 400)

  local s5 = ffi.new("v2", 0, 0)
  local e5 = ffi.new("v2", 0, 720)
  bg_:tween("ISine", "Pos2D", s5, e5, 1000)
end

------------------------------------------------------------

local function show_effect(effect)
  if effect and effect.id ~= nil then effect_[effect.id](effect) end
end

------------------------------------------------------------

local function hide_page_obj()
  for k,v in pairs(page_obj_) do
    v:set_visible(false)
  end
end

local function remove_to_be_delete()
  if to_be_delete_==nil then return end
  for k,v in pairs(to_be_delete_) do
    if v.remove_cb ~= nil then v:remove_cb() end
    v:remove()
  end
end

-- added data parameter to customize the same menu with different options
local function preload_page(name)
  local p = require ('rc/script/ui/demo/'..name..'/'..name)
  page_obj_ = p.init(game_demo_, vorig_)
  hide_page_obj()
  view.debug_hack()
end

local function load_page(name, effect, data)
  remove_to_be_delete()
  hide_page_obj()

  local prev_page = current_page_
  
  to_be_delete_ = page_obj_
  local p = require ('rc/script/ui/demo/'..name..'/'..name)
  data_ = data
  current_page_ = 'rc/script/ui/demo/'..name..'/'..name
  page_obj_ = p.init(game_demo_, vorig_, data)
  
  if name == 'mainmenu' and prev_page == 'rc/script/ui/demo/startscreen/startscreen' then
    startscreen_to_mainmenu()
  elseif name == 'startscreen' and prev_page == 'rc/script/ui/demo/mainmenu/mainmenu' then
    mainmenu_to_startscreen()
  end
  
  show_effect(effect)

  view.debug_hack()
end

local function refresh_page(root)
  if current_page_ ~= string.sub(root,1,-5) then
    print('---- cannot refresh, not in current page ----') 
    return false
  end

  remove_to_be_delete()
  hide_page_obj()
  
  package.loaded[string.sub(root,1,-5)] = nil --unrequire
  
  to_be_delete_ = page_obj_
  local p = require ( string.sub(root,1,-5) )
  page_obj_ = p.init(game_demo_, vorig_, data_)

  view.debug_hack()
  
  return true
end

local function get_page_obj()
  return page_obj_
end

return {
  init  = init,
  ----
  show_effect   = show_effect,
  ----
  preload_page  = preload_page,
  load_page     = load_page,
  refresh_page  = refresh_page,
  get_page_obj  = get_page_obj
}
