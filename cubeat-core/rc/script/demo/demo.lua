local ffi   = require 'ffi'
local C     = ffi.C
local view  = require 'rc/script/ui/view'
local ui    = require 'rc/script/ui/ui'
local switch= require 'rc/script/ui/demo/switch/switch'
local event = require 'rc/script/event/event'
require 'rc/script/demo/defs'
require 'rc/script/strict'

----------------------------------------------------------------------------

local demo_
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

function init_override(in_place, submode)
  print("Inplace: "..tostring(in_place))
  print("Submode: "..submode)
  demo_:init_map_starting_line(0, 5);
  --demo_:set_map_garbage_amount(0, 20);
  
  event.on_timer("challenge", function() 
    --print(demo_:get_map_score(1)) 
    --print(demo_:get_map_highest_chain(1))
    --print(demo_:get_map_cubes_cleared_data(1)[1])
    end, 1000, -1 )
end

function tutorial_update(state, data)
  
  print("Lua: ".."tutorial state "..tostring(state))
  
  local tut_script = nil
  if state == 0 then
    tut_script = require 'rc/script/ui/demo/tutorial/state0'
  elseif state == 1 then
    tut_script = require 'rc/script/ui/demo/tutorial/state1'
  elseif state == 2 then 
    tut_script = require 'rc/script/ui/demo/tutorial/state2'
  end
  
  if tut_script ~= nil then
    tut_script.update(demo_, scene_, data)
  end
  
end

function mainmenu()
  show_everything()
end
