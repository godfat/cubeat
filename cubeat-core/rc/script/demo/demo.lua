local ffi   = require 'ffi'
local C     = ffi.C
local view  = require 'rc/script/ui/view'
local ui    = require 'rc/script/ui/ui'
local switch= require 'rc/script/ui/demo/switch/switch'
require 'rc/script/demo/defs'
require 'rc/script/strict'

----------------------------------------------------------------------------

local demo_
local scene_

local win_ = false -- win state for SinglePlayer modes.
local puzzle_level_ = 2 

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
  
  --preload all ui
  --switch.load_page('tutorial' )
  --switch.load_page('score'    )
  --switch.load_page('tweentest')
  switch.load_page('select'   )
  --switch.load_page('testmenu' )
  switch.load_page('mainmenu' )
  
  -- test for temporary menu in Puzzle mode end
  menu_.ask_end = ui.new_askbox { parent = demo_:get_ui_scene(), title = 'TITLE', depth=-100 }
  menu_.ask_end:set_visible(false)
end

--- Other must-implemented events for main game on Lua-side: ---

-- occurs right after basic stage/player/map data have been initialized
-- Currently submode == 0 means PUZZLE mode.
function init_override(in_place, submode)
  print("Inplace: "..tostring(in_place))
  print("Submode: "..submode)
  
  win_ = false -- re-initialize certain states
  
  if submode == 0 then -- Puzzle Mode
    demo_:set_only_one_shot_for_puzzle()
    -- Currently don't do anything else than this when initializing PUZZLE.
    -- C++ handles generating PUZZLE map when initialization
  elseif submode == 1 then
    demo_:init_map_starting_line(0, 5);
    demo_:set_map_garbage_amount(0, 20);
  end
end

-- occurs each frame, after map and player states update,
-- but it WILL NOT BE CALLED after endgame(map_id) is called.
function check_ending_condition_by_frame(submode)
  if submode == 0 then
    if demo_:is_puzzle_started() then 
      if demo_:is_map_empty(0) then
        win_ = true
        demo_:endgame(0) 
        -- NOTE: 
        -- In the merged "SinglePlayer" game modes, what you passed into endgame() is not relevent.
        -- You have to track win/lose state in Lua.
      elseif demo_:is_map_all_waiting(0) then
        win_ = false
        demo_:endgame(0)
      end
    end
  end
end

-- occurs when win/lose reached. Will be reached if you call endgame(map_id) in Lua
-- mainly used for setting up ending UI
function ending(submode)
  if submode == 0 then
    menu_.ask_end:set_title( win_ and 'WIN' or 'LOSE' )
    menu_.ask_end.ok:change_text( win_ and 'Next Level' or 'Retry' )
    menu_.ask_end.cancel:change_text('Quit')
    
    menu_.ask_end:on_press_ok(function(self)
      menu_.ask_end:set_visible(false)
      if win_ then 
        puzzle_level_ = puzzle_level_ + 1 
      end
      demo_:init_puzzle('char/char1_new', 'stage/jungle1', puzzle_level_, true) 
    end, 1)
    
    menu_.ask_end:on_press_cancel(function(self)
      menu_.ask_end:set_visible(false) 
      demo_:leave_and_cleanup()
    end, 1)
    
    menu_.ask_end:set_visible(true)
  end
end

-- occurs right after game ends (no reinit, go back to menu)
-- Will be reached if you call leave_and_cleanup() in Lua
function cleanup(submode)
  puzzle_level_ = 2
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
