local ffi  = require 'ffi'
local C    = ffi.C
local view = require 'rc/script/ui/view'
local ui   = require 'rc/script/ui/ui'


local page_obj_ = {}
local to_be_delete_
local data_ = nil
local current_page_ = nil
local starter_page_ = nil

local game_demo_
local vorig_
local root_
local title_
local demobuild_
local teamname_
local blocker_
local transfer_
local transfer_title_
--local ask_panel_
local has_blocker_ = true

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
  blocker_:set_alpha(128)
  
  transfer_ = ui.new_image{ parent=root_, path=' ', x=640, y=-480, w=1280, h=960, center=true, depth=-500 }
  transfer_:set_red(0)
  transfer_:set_blue(0)
  transfer_:set_green(0)
  transfer_title_ = ui.new_image{ parent=transfer_._cdata, path='title', x=0, y=0, w=512, h=512, center=true }
  
  --ask_panel_  = ui.new_askbox{ parent = root_, depth = -500, visible=false }
end

------------------------------------------------------------
--[[
local function set_ask_panel_title(title)
  ask_panel_:set_title(title)
end

local function set_press_ok(func)
  ask_panel_:on_press_ok(func)
end

local function set_press_cancel(func)
  ask_panel_:on_press_cancel(func)
end

local function show_ask_panel()
  ask_panel_:set_visible(true)
end

local function hide_ask_panel()
  ask_panel_:set_visible(false)
end
--]]
------------------------------------------------------------

local function slide_out_transfer(name)
  local s1 = ffi.new("v2", 640,  360)
  local e1 = ffi.new("v2", 640, -480)
  if name then
    starter_page_ = require ('rc/script/ui/demo/'..name..'/'..name)
    local function cb() starter_page_.starter(page_obj_) end
    transfer_:tween("ISine", "Pos2D", s1, e1, 1000, 0, cb, 1000)
  else
    transfer_:tween("ISine", "Pos2D", s1, e1, 1000, 0, nil, 1000)
  end
end

local function slide_in_transfer(cb)
  local s1 = ffi.new("v2", 640, -480)
  local e1 = ffi.new("v2", 640,  360)
  transfer_:tween("OElastic", "Pos2D", s1, e1, 2000, 0, cb, 0)
end

------------------------------------------------------------

local function slide_out_title(keep_blocker)
  local s2 = ffi.new("v2", 640, 150)
  local e2 = ffi.new("v2", 640, -200)
  title_:tween("ISine", "Pos2D", s2, e2, 500)

  local s3 = ffi.new("v2", 1000, 280)
  local e3 = ffi.new("v2", 1000, 780)
  demobuild_:tween("ISine", "Pos2D", s3, e3, 750)

  local s4 = ffi.new("v2", 640, 690)
  local e4 = ffi.new("v2", 640, 780)
  teamname_:tween("ISine", "Pos2D", s4, e4, 400)

  if not keep_blocker then
    blocker_:tween("Linear", "Alpha", 128, 0, 500, 0, nil, 0)
    has_blocker_ = false
  end
end

local function slide_in_title()
  local s2 = ffi.new("v2", 640, -200)
  local e2 = ffi.new("v2", 640, 150)
  title_:tween("OSine", "Pos2D", s2, e2, 500)

  local s3 = ffi.new("v2", 1000, 780)
  local e3 = ffi.new("v2", 1000, 280)
  demobuild_:tween("OElastic", "Pos2D", s3, e3, 750)

  local s4 = ffi.new("v2", 640, 780)
  local e4 = ffi.new("v2", 640, 690)
  teamname_:tween("OSine", "Pos2D", s4, e4, 400)
  
  if not has_blocker_ then
    blocker_:tween("Linear", "Alpha", 0, 128, 500, 0, nil, 0)
  end
end

------------------------------------------------------------

local function slide_out_page_obj()
  local s1 = ffi.new("v2", 0, 0)
  local e1 = ffi.new("v2", 2000, 0)
  vorig_:tween("OSine", "Pos2D", s1, e1, 1000)
  blocker_:tween("Linear", "Alpha", 128, 0, 500, 0, nil, 0)
  has_blocker_ = false
end

local function slide_in_page_obj(name)
  local s1 = ffi.new("v2", -2160, 0)
  local e1 = ffi.new("v2", 0, 0)
  if name then
    starter_page_ = require ('rc/script/ui/demo/'..name..'/'..name)
    local function cb() starter_page_.starter(page_obj_) end
    vorig_:tween("ISine", "Pos2D", s1, e1, 1000, 0, cb, 0)
  else
    vorig_:tween("ISine", "Pos2D", s1, e1, 1000)
  end
  blocker_:tween("Linear", "Alpha", 0, 128, 500, 0, nil, 0)
  has_blocker_ = true
end

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
local function load_page(name, slide_title, data)
  remove_to_be_delete()
  hide_page_obj()

  if slide_title == 'in' then
    slide_in_title()
  elseif slide_title == 'out' then
    slide_out_title(true)
  end

  to_be_delete_ = page_obj_
  local p = require ('rc/script/ui/demo/'..name..'/'..name)
  data_ = data
  current_page_ = 'rc/script/ui/demo/'..name..'/'..name
  page_obj_ = p.init(game_demo_, vorig_, data)

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
  --[[
  set_ask_panel_title = set_ask_panel_title,
  set_press_ok        = set_press_ok,
  set_press_cancel    = set_press_cancel,
  show_ask_panel      = show_ask_panel,
  hide_ask_panel      = hide_ask_panel,
  --]]
  ----
  slide_out_transfer  = slide_out_transfer,
  slide_in_transfer   = slide_in_transfer,
  slide_out_page_obj  = slide_out_page_obj,
  slide_in_page_obj   = slide_in_page_obj,
  load_page           = load_page,
  refresh_page        = refresh_page,
  get_page_obj        = get_page_obj,
  ----
  slide_out_title = slide_out_title,
  slide_in_title  = slide_in_title
}
