local ffi   = require 'ffi'
local jit   = require 'jit'
local C     = ffi.C
local view  = require 'rc/script/ui/view'
local ui    = require 'rc/script/ui/ui'
local switch= require 'rc/script/ui/demo/switch/switch'
local event = require 'rc/script/event/event'
require 'rc/script/demo/defs'
require 'rc/script/strict'
local challenge = require 'rc/script/ui/demo/challenge/challenge'
local recordboard = require 'rc/script/ui/demo/challenge/recordboard'
local storyend    = require 'rc/script/ui/demo/storyend/storyend'
local storyend_config = require 'rc/script/ui/demo/storyend/config'
local vsend       = require 'rc/script/ui/demo/vsend/vsend'
local endingcheck = require 'rc/script/ui/demo/endingcheck/endingcheck'

-- local tutorial = require 'rc/script/demo/tutorial'

----------------------------------------------------------------------------

local demo_
local scene_
local refresh_btn_
local staffroll_btn_

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
    switch.show_effect( {id="slide_out"} )
  end
end

-- occurs right after game ends (no reinit, go back to menu), mainly used for UI transition
-- If you have setup some UI during gameplay in Lua, hide/remove them here.
function slide_in()
  if storyend_config.is_story_end then
    storyend_config.is_story_end = false
    storyend.hide()
    switch.load_page('talk', nil, {game_mode=99, game_end=true})
    switch.show_effect( {id="slide_in_to_talk"} )
  else
    show_everything()
    switch.load_page('mainmenu')
    switch.show_effect( {id="slide_in"} )
  end
end

function init(demo)
  -- This will be the only occurrence of calling randomseed() in gameplay scripts. 
  math.randomseed(os.time())

  demo_  = ffi.cast("Demo*", demo);
  scene_ = demo_:get_ui_scene()

  --------------------------------------

  switch.init(scene_, demo_)

  --preload ui
  switch.preload_page('select')
  switch.preload_page('mainmenu')
  switch.load_page('startscreen')
    
  -- test: add refresh button
  refresh_btn_ = ui.new_text{ parent = scene_, title='refresh', x=850, y=670, size=32, depth=-1000 }
  refresh_btn_:set_scale(1.2)
  local filelist = require 'rc/script/ui/demo/refresh/filelist'
  local refresh  = require 'rc/script/ui/demo/refresh/refresh'
  refresh_btn_:on_press(function(self)
    print('---- check file time ----')
    for k,root in pairs(filelist.list) do
      local t = C.get_ftime(root)
      refresh.check_file_time(root, t)
    end
    --demo_:hide_character_animations()
  end)
  -- test: switch to staffroll page
  staffroll_btn_ = ui.new_text{ parent = scene_, title='staffroll', x=850, y=630, size=32, depth=-1000 }
  staffroll_btn_:set_scale(1.2)
  staffroll_btn_:on_press(function(self)
    switch.load_page('staffroll', {id='show'}, nil)
  end)
  -- check file time once when init
  for k,root in pairs(filelist.list) do
    local t = C.get_ftime(root)
    refresh.check_file_time(root, t)
  end

  -- test for temporary menu in Puzzle mode end
  recordboard.create_record_board(scene_)
  storyend.create(scene_)
  vsend.create(scene_)
  
  --tutorial.init(demo_)
end


--- Other must-implemented events for main game on Lua-side: ---

-- occurs right after basic stage/player/map data have been initialized
-- Currently submode == 0 means PUZZLE mode.
function init_override(in_place, submode)
  print("Inplace: "..tostring(in_place))
  print("Submode: "..submode)

  -- if submode == 50 then 
    -- tutorial.init(demo_) 
  -- else
    challenge.init_override(demo_, in_place, submode)
  -- end
end

-- occurs each frame, after map and player states update,
-- but it WILL NOT BE CALLED after endgame(map_id) is called.
function check_ending_condition_by_frame(submode)
  --print('---- single mode: check_ending_condition_by_frame ----')
  -- if submode == 50 then tutorial.update() return end
  challenge.check_ending_condition_by_frame(demo_, submode)
end

-- occurs when win/lose reached. Will be reached if you call endgame(map_id) in Lua
-- mainly used for setting up ending UI
function ending(game_mode, submode)
  endingcheck.show_ending_ui(demo_, game_mode, submode)
end

-- occurs right after game ends (no reinit, go back to menu)
-- Will be reached if you call leave_and_cleanup() in Lua,
-- it will also be called if you PAUSE & QUIT.
function cleanup(submode)
  print('---- Demo (Lua side): cleanup ----')
  challenge.cleanup()
  demo_:set_countdown(false)
  
  -- tutorial.cleanup()
  collectgarbage("collect")
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
