local ffi   = require 'ffi'
local C     = ffi.C
local view  = require 'rc/script/ui/view'
local ui    = require 'rc/script/ui/ui'
local switch= require 'rc/script/ui/demo/switch/switch'
local event = require 'rc/script/event/event'
require 'rc/script/demo/defs'
require 'rc/script/strict'
local challenge = require 'rc/script/ui/demo/challenge/challenge'
local jit = require 'jit'
local recordboard = require 'rc/script/ui/demo/challenge/recordboard'

----------------------------------------------------------------------------

local demo_
local scene_

--local win_ = false -- win state for SinglePlayer modes.
--local puzzle_level_ = 2 

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

-- occurs right before game start (but after init), mainly used for UI transition
function slide_out(inplace)
  if inplace then
    hide_everything()
  else
    switch.slide_out_page_obj()
  end
end

-- occurs right after game ends (no reinit, go back to menu), mainly used for UI transition
-- If you have setup some UI during gameplay in Lua, hide/remove them here.
function slide_in()
  show_everything()
  switch.load_page('mainmenu', 'in')
  switch.slide_in_page_obj()
end

function init(demo)
  demo_  = ffi.cast("Demo*", demo);
  scene_ = demo_:get_ui_scene()
  
  --------------------------------------
  
  switch.init(scene_, demo_)
  
  --preload ui
  switch.load_page('select'   )
  switch.load_page('mainmenu' )
  
  -- test for temporary menu in Puzzle mode end
  recordboard.create_record_board(scene_)
end

--- Other must-implemented events for main game on Lua-side: ---

-- occurs right after basic stage/player/map data have been initialized
-- Currently submode == 0 means PUZZLE mode.
function init_override(in_place, submode)
  print("Inplace: "..tostring(in_place))
  print("Submode: "..submode)
  
  challenge.init_override(demo_, in_place, submode)
end

-- occurs each frame, after map and player states update,
-- but it WILL NOT BE CALLED after endgame(map_id) is called.
function check_ending_condition_by_frame(submode)
  --print('---- single mode: check_ending_condition_by_frame ----')
  challenge.check_ending_condition_by_frame(demo_, submode)
end

-- occurs when win/lose reached. Will be reached if you call endgame(map_id) in Lua
-- mainly used for setting up ending UI
function ending(submode)
  print('---- single mode: ending ----')
  challenge.ending(demo_, submode)
end

-- occurs right after game ends (no reinit, go back to menu)
-- Will be reached if you call leave_and_cleanup() in Lua, 
-- it will also be called if you PAUSE & QUIT. 
function cleanup(submode)
  print('---- single mode: cleanup ----')
  challenge.cleanup()
  demo_:set_countdown(false)
end


-- This really should just be a temporary solution, a separated menu page should be better
-- DEPRECATED

-- function tutorial_update(state, data)
  
  -- print("Lua: ".."tutorial state "..tostring(state))
  
  -- local tut_script = nil
  -- if state == 0 then
    -- tut_script = require 'rc/script/ui/demo/tutorial/state0'
  -- elseif state == 1 then
    -- tut_script = require 'rc/script/ui/demo/tutorial/state1'
  -- elseif state == 2 then 
    -- tut_script = require 'rc/script/ui/demo/tutorial/state2'
  -- end
  
  -- if tut_script ~= nil then
    -- tut_script.update(demo_, scene_, data)
  -- end
  
-- end
