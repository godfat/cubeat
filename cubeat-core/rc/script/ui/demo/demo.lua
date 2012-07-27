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
  switch.slide_in_page_obj()
end

function init(demo)
  demo   = ffi.cast("Demo*", demo);
  scene_ = demo:get_ui_scene()
  
  --------------------------------------
  
  switch.init(scene_)
  
  --preload all ui
  switch.load_page(demo, 'tutorial' )
  switch.load_page(demo, 'score'    )
  switch.load_page(demo, 'tweentest')
  switch.load_page(demo, 'select'   )
  switch.load_page(demo, 'testmenu' )
  switch.load_page(demo, 'mainmenu' )
end

function mainmenu()
  show_everything()
end
