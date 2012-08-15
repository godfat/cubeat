local ffi   = require 'ffi'
local C     = ffi.C
local view  = require 'rc/script/ui/view'
local ui    = require 'rc/script/ui/ui'
local switch= require 'rc/script/ui/demo/switch/switch'
require 'rc/script/ui/demo/defs'
require 'rc/script/strict'

----------------------------------------------------------------------------

local scene_
local menu_ = {}

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
end

function init(demo)
  demo   = ffi.cast("Demo*", demo);
  scene_ = demo:get_ui_scene()
  
  --------------------------------------
  
  switch.init(scene_, demo)
  
  --preload all ui
  switch.load_page('tutorial' )
  switch.load_page('score'    )
  switch.load_page('tweentest')
  switch.load_page('select'   )
  switch.load_page('testmenu' )
  switch.load_page('mainmenu' )
end

function mainmenu()
  show_everything()
end
