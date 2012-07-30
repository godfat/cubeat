local ffi  = require 'ffi'
local C    = ffi.C
local view = require 'rc/script/ui/view'
local ui   = require 'rc/script/ui/ui'


local page_obj_ = {}
local to_be_delete_

local vorig_ 
local title_
local demobuild_
local teamname_
local blocker_

------------------------------------------------------------

local function init(parent)
  vorig_ = view.new_sprite("blahblah", parent, 1, 1, true) 
  vorig_:set_pos(480, 300)
  print '\n\nHello from Lua!\n\n'

  title_   = ui.new_image{ parent = parent,
    path='title2', x=640, y=150, w=840, h=250, center=true}
    
  demobuild_ = ui.new_image{ parent = parent,
    path='demobuild', x=1000, y=780, w=246, h=76, center=true}
  demobuild_:set_rotation(9)
  local s = ffi.new("v2", 1000, 740)
  local e = ffi.new("v2", 1000, 280)
  demobuild_:tween_elastic_pos(s, e, 2000, 0, nil, 1000) -- delay 1000
    
  teamname_ = ui.new_image{ parent = parent,
    path='teampsc', x=640, y=690, w=300, h=50, center=true}
  
  blocker_ = view.new_sprite("blahblah", parent, 1280, 720, false)
  blocker_:set_pos(0, 0)
  blocker_:set_color(0, 0, 0)
  blocker_:set_alpha(96)
end

local function slide_out_title()
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

local function slide_in_title()
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

------------------------------------------------------------

local function slide_out_page_obj()
  local s1 = ffi.new("v2", 480, 300)
  local e1 = ffi.new("v2", 1960, 300)
  vorig_:tween_osine_pos(s1, e1, 1000)
end

local function slide_in_page_obj()
  local s = ffi.new("v2", -680, 300)
  local e = ffi.new("v2", 480, 300)
  vorig_:tween_isine_pos(s, e, 1000)
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

local function load_page(demo, name, slide_title)
  remove_to_be_delete()
  hide_page_obj()

  if slide_title~=nil and slide_title=='in'  then slide_in_title()  end
  if slide_title~=nil and slide_title=='out' then slide_out_title() end
  
  to_be_delete_ = page_obj_
  local p = require ('rc/script/ui/demo/'..name..'/'..name)
  page_obj_ = p.init(demo, vorig_)
  
  view.debug_hack()
end

local function get_page_obj()
  return page_obj_
end

return {
  init  = init,
  ----
  slide_out_page_obj  = slide_out_page_obj,
  slide_in_page_obj   = slide_in_page_obj,
  load_page           = load_page,
  get_page_obj        = get_page_obj
}