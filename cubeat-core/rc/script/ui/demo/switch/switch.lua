local ffi  = require 'ffi'
local C    = ffi.C
local view = require 'rc/script/ui/view'
local ui   = require 'rc/script/ui/ui'
local master_menu = require 'rc/script/ui/demo/switch/master_menu'

local page_obj_ = {}
local to_be_delete_
local data_ = nil
local current_page_ = nil
local starter_page_ = nil
--
local game_demo_
local vorig_
--
local effect_ = {}
local effect_count_ = 0

------------------------------------------------------------

local function init(parent, demo)
  game_demo_ = demo

  vorig_ = view.new_sprite("blahblah", parent, 0, 0, false)
  --vorig_:set_pos(480, 300)
  print '\n\nHello from Lua!\n\n'

  master_menu.init(parent, demo)
end

------------------------------------------------------------

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

effect_.storyend = function(effect)
  for k,v in pairs(to_be_delete_) do
    if k~="content2" and k~="panel2" and k~="light2" and k~="clickBlock" then
      v:set_visible(true)
    end
  end
end

effect_.slide_out = function(effect)
  local s1 = ffi.new("v2", 0, 0)
  local e1 = ffi.new("v2", 2000, 0)
  vorig_:tween("OSine", "Pos2D", s1, e1, 1000)
  master_menu.slide_out(s1, e1, 1000)
end

effect_.slide_in = function(effect)
  local s1 = ffi.new("v2", -2160, 0)
  local e1 = ffi.new("v2", 0, 0)
  vorig_:tween("ISine", "Pos2D", s1, e1, 1000)
  master_menu.slide_in(s1, e1, 1000)
end

effect_.slide_in_to_talk = function(effect)
  ui.set_input_lock(true)
  
  local s1 = ffi.new("v2", -2160, 0)
  local e1 = ffi.new("v2", 0, 0)
  starter_page_ = require ('rc/script/ui/demo/talk/talk')
  local function cb() starter_page_.starter(page_obj_) ui.set_input_lock(false) end
  vorig_:tween("ISine", "Pos2D", s1, e1, 1000, 0, cb, 0)
  master_menu.slide_in(s1, e1, 1000)
end

effect_.slide_out_title = function(effect)
end

effect_.slide_in_title = function(effect)
end

effect_.slide_out_transfer = function(effect)
  master_menu.slide_out_transfer(effect)
end

effect_.slide_out_transfer_to_talk = function(effect)
  starter_page_ = require ('rc/script/ui/demo/talk/talk')
  local function cb() starter_page_.starter(page_obj_) ui.set_input_lock(false) end
  
  master_menu.slide_out_transfer_to_talk(effect, cb)
end

effect_.slide_in_transfer = function(effect)
  master_menu.slide_in_transfer(effect)
end

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
    if type(v) == 'table' and v.remove_cb ~= nil then v:remove_cb() end
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
    master_menu.startscreen_to_mainmenu()
  elseif name == 'startscreen' and prev_page == 'rc/script/ui/demo/mainmenu/mainmenu' then
    master_menu.mainmenu_to_startscreen()
  elseif name ~= 'startscreen' and name ~= 'mainmenu' and prev_page == 'rc/script/ui/demo/mainmenu/mainmenu' then
    master_menu.fade_in_blocker()
  elseif name == 'mainmenu' and prev_page ~= 'rc/script/ui/demo/mainmenu/startscreen' then
    master_menu.fade_out_blocker()
  end
  
  show_effect(effect)

  view.debug_hack()
end

local function refresh_master_menu()
  if current_page_ ~= 'rc/script/ui/demo/startscreen/startscreen' and 
     current_page_ ~= 'rc/script/ui/demo/mainmenu/mainmenu' then
    print('---- not in startscreen or mainmenu, not refreshing master_menu ----')
    return false
  end
  master_menu.cleanup()
  
  package.loaded['rc/script/ui/demo/switch/master_menu'] = nil --unrequire
  master_menu = require 'rc/script/ui/demo/switch/master_menu'
  
  local parent = game_demo_:get_ui_scene()
  
  master_menu.init(parent, game_demo_)
  
  if current_page_ == 'rc/script/ui/demo/mainmenu/mainmenu' then
    master_menu.startscreen_to_mainmenu()
  end
  
  view.debug_hack()
  
  return true
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

local function show_master_menu()
  master_menu.show()
end

return {
  init  = init,
  ----
  show_effect   = show_effect,
  ----
  preload_page  = preload_page,
  load_page     = load_page,
  refresh_page  = refresh_page,
  refresh_master_menu = refresh_master_menu,
  get_page_obj  = get_page_obj,
  show_master_menu = show_master_menu
}
