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
  --[[
  menu_.ask_end = ui.new_askbox { parent = demo_:get_ui_scene(), title = 'TITLE', depth=-100 }
  menu_.ask_end:set_visible(false)
  --]]
  recordboard.create_record_board(scene_)
end

--- Other must-implemented events for main game on Lua-side: ---

-- occurs right after basic stage/player/map data have been initialized
-- Currently submode == 0 means PUZZLE mode.
function init_override(in_place, submode)
  print("Inplace: "..tostring(in_place))
  print("Submode: "..submode)
  
  challenge.set_win(false)  -- re-initialize certain states
  
  if submode == 0 then -- Puzzle Mode
    demo_:set_only_one_shot_for_puzzle()
    -- Currently don't do anything else than this when initializing PUZZLE.
    -- C++ handles generating PUZZLE map when initialization
  else
    challenge.init_override(demo_, in_place, submode, scene_)
  end
end

-- occurs each frame, after map and player states update,
-- but it WILL NOT BE CALLED after endgame(map_id) is called.
function check_ending_condition_by_frame(submode)
  --print('-------------------- check_ending_condition_by_frame -----------------------')
  if submode == 0 then
    if demo_:is_puzzle_started() then
      if demo_:is_map_empty(0) then
        challenge.set_win(true)
        demo_:endgame(0) 
        -- NOTE: 
        -- In the merged "SinglePlayer" game modes, what you passed into endgame() is not relevent.
        -- You have to track win/lose state in Lua.
      elseif demo_:is_map_all_waiting(0) then
        challenge.set_win(false)
        demo_:endgame(0)
      end
    end
  else
    challenge.check_ending_condition_by_frame(demo_, submode)
  end
end

-- occurs when win/lose reached. Will be reached if you call endgame(map_id) in Lua
-- mainly used for setting up ending UI
function ending(submode)
  if submode == 0 then
    --[[
    menu_.ask_end:set_title( challenge.get_win() and 'SUCCESS' or 'FAIL' )
    menu_.ask_end.ok:change_text( challenge.get_win() and 'Next' or 'Retry' )
    menu_.ask_end.cancel:change_text('Quit')
    
    menu_.ask_end:on_press_ok(function(self)
      menu_.ask_end:set_visible(false)
      if challenge.get_win() then
        if challenge.get_puzzle_level() < 19 then challenge.add_puzzle_level(1) end
        if challenge.get_puzzle_level() < 2 then challenge.set_puzzle_level(2) end
      end
      -- init SinglePlayer, in Submode 0, and Level is decided by challenge.get_puzzle_level() variable
      -- the last true means "in_place" is true, there won't be slide-in/out effects.
      demo_:init_single(0, challenge.get_puzzle_level(), 'char/char1_new', 'stage/jungle1', true)
    end, 1)
    
    menu_.ask_end:on_press_cancel(function(self)
      menu_.ask_end:set_visible(false) 
      demo_:leave_and_cleanup()
    end, 1)
    
    menu_.ask_end:set_visible(true)
    --]]
    recordboard.set_title( challenge.get_win() and 'SUCCESS' or 'FAIL' )
    
    recordboard.on_press_next(function(self)
      recordboard.hide()
      if challenge.get_win() then
        if challenge.get_puzzle_level() < 19 then challenge.add_puzzle_level(1) end
        if challenge.get_puzzle_level() < 2 then challenge.set_puzzle_level(2) end
      end
      -- init SinglePlayer, in Submode 0, and Level is decided by challenge.get_puzzle_level() variable
      -- the last true means "in_place" is true, there won't be slide-in/out effects.
      demo_:init_single(0, challenge.get_puzzle_level(), 'char/char1_new', 'stage/jungle1', true)
    end)
    recordboard.on_press_retry(function(self)
      recordboard.hide()
      demo_:init_single(0, challenge.get_puzzle_level(), 'char/char1_new', 'stage/jungle1', true)
    end)
    recordboard.on_press_quit(function(self)
      recordboard.hide() 
      demo_:leave_and_cleanup()
    end)
    
    recordboard.show(submode)
  else
    --[[
    menu_.ask_end:set_title( challenge.get_win() and 'SUCCESS' or 'FAIL' )
    menu_.ask_end.ok:change_text('Retry')
    menu_.ask_end.cancel:change_text('Quit')
    
    menu_.ask_end:on_press_ok(function(self)
      menu_.ask_end:set_visible(false)
      demo_:init_single(submode, 1, 'char/char1_new', 'stage/jungle1', true)
      print('reinit submode: ' .. tostring(submode))
    end, 1)
    
    menu_.ask_end:on_press_cancel(function(self)
      menu_.ask_end:set_visible(false)
      demo_:leave_and_cleanup()
    end, 1)
    menu_.ask_end:set_visible(true)
    --]]
    challenge.ending(demo_, submode)
  end
end

-- occurs right after game ends (no reinit, go back to menu)
-- Will be reached if you call leave_and_cleanup() in Lua, 
-- it will also be called if you PAUSE & QUIT. 
function cleanup(submode)
  print('-------- cleanup --------')
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
