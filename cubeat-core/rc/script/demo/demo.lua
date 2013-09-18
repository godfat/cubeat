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
local storyend    = require 'rc/script/ui/demo/storyend/storyend'
local select_config = require 'rc/script/ui/demo/select/config'

require 'rc/script/ai/ai' -- we have to use AI script here if we ant to control AI player
                          -- but then we'll also need an AI handle... : / 
local function setcmd(buf, type, delay, x, y)
  buf.x, buf.y, buf.delay, buf.type = x, y, delay, type
end

----------------------------------------------------------------------------

local demo_
local scene_

local tut_phase_ = 1
local tut_actions_ = {}
local tut_checkpoints_ = {}
local haste_counter_ = 0
local ask_panel_ = nil

local story_flag_ = false

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
  if story_flag_ then
    story_flag_ = false
    storyend.hide()
    switch.load_page('talk', nil, {game_mode=99, game_end=true})
    switch.slide_in_page_obj()
  else
    show_everything()
    switch.load_page('mainmenu', 'in')
    switch.slide_in_page_obj()
  end
end

function init(demo)
  demo_  = ffi.cast("Demo*", demo);
  scene_ = demo_:get_ui_scene()

  --------------------------------------

  switch.init(scene_, demo_)

  --preload ui
  switch.load_page('select'   )
  switch.load_page('mainmenu' )
  
  -- prepare a dialogue for tutorial
  ask_panel_ = ui.new_askbox{ parent=scene_, w=800, h=480,
                              title="Tutorial",
                              cb={function(self)
                                demo_:eventual_resume()
                                tut_phase_ = tut_phase_ + 1
                                ask_panel_:set_visible(false)
                              end} }
  ask_panel_:set_visible(false)

  -- test for temporary menu in Puzzle mode end
  recordboard.create_record_board(scene_)
  storyend.create(scene_)
end

function tutorial()
  if tut_phase_ > #tut_actions_ then return end
  if tut_checkpoints_[tut_phase_]() then
    ask_panel_:set_visible(true)
    tut_actions_[tut_phase_]()
    demo_:eventual_pause()
  end
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
  if submode == 50 then tutorial() return end
  challenge.check_ending_condition_by_frame(demo_, submode)
end

-- occurs when win/lose reached. Will be reached if you call endgame(map_id) in Lua
-- mainly used for setting up ending UI
function ending(submode)
  print('---- single mode: ending ----')
  
  if submode==99 then
      if demo_:get_map_warning_level(1)==100 then -- story win
        storyend.set_board('win')
        storyend.set_btn_title('Next')
        storyend.on_press_next(function(self)
          story_flag_ = true
          demo_:leave_and_cleanup()
          --[[
          storyend.hide()
          switch.load_page('talk', nil, {game_mode=99, game_end=true})
          switch.slide_in_page_obj()
          --]]
        end)
      else
        storyend.set_board('lose')
        storyend.set_btn_title('Retry')
        storyend.on_press_next(function(self)
          storyend.hide()
          local c1p = "char/char"..tostring(select_config.ch_choose[1]).."_new"
          local c2p = "char/char"..tostring(select_config.ch_choose[2]).."_new"
          local sconf = "stage/jungle"..tostring(select_config.ch_choose[2])
          demo_:init_story(c1p, c2p, sconf, 0)
        end)
      end
      storyend.on_press_quit(function(self)
        storyend.hide()
        demo_:leave_and_cleanup()
      end)
      storyend.show()
  else
    challenge.ending(demo_, submode)
  end
end

-- occurs right after game ends (no reinit, go back to menu)
-- Will be reached if you call leave_and_cleanup() in Lua,
-- it will also be called if you PAUSE & QUIT.
function cleanup(submode)
  print('---- single mode: cleanup ----')
  challenge.cleanup()
  demo_:set_countdown(false)
  
  tut_phase_ = 1
  haste_counter_ = 0
  ask_panel_:set_visible(false)
end

tut_checkpoints_[1] = function()
  return true
end

-- we need an introduction to player map and player cursor before everything.

tut_actions_[1] = function()
  ask_panel_:set_title("Tutorial about haste")
  -- should not just show the text here. it should directly be the indication of your map and your cursor
  -- a highlight/mask effect 
  -- but in this tutorial we should avoid of speaking: "You are RED, you shoot left"
  -- instead, say: "Player 1 is RED, and Player 1's stage is on the left" 
  -- so we don't deny the possibility to "shoot others"
  
  demo_:set_map_dropping(0, false)
  demo_:set_map_dropping(1, true)

  local aiplayer = demo_:get_ai_player()
  local cmdbuf   = ffi.new("LuaAICommand", {0, 0, 0, C.AI_NONE}) 
  setcmd(cmdbuf, C.AI_HASTE, 1000, 0, 0)
  aiplayer:push_command(cmdbuf)
  setcmd(cmdbuf, C.AI_HASTE, 0, 0, 0)
  aiplayer:push_command(cmdbuf)
end
--
tut_checkpoints_[2] = function()
  haste_counter_ = haste_counter_ + 1
  if haste_counter_ > 150 then return true end
  return false
end

tut_actions_[2] = function()
  ask_panel_:set_title("Tutorial about haste:\nYour turn")
  demo_:set_map_dropping(0, true)
  haste_counter_ = 0
end
--
tut_checkpoints_[3] = function()
  if view.is_haste_button_down(view.Input1) then
    haste_counter_ = haste_counter_ + 1
  end
  if haste_counter_ > 60 then 
    return true 
  end
  return false
end

tut_actions_[3] = function()
  ask_panel_:set_title("Tutorial about shooting")

  -- AI setup shooting here   
  local aiplayer = demo_:get_ai_player()
  local cmdbuf   = ffi.new("LuaAICommand", {0, 0, 0, C.AI_NONE}) 
  setcmd(cmdbuf, C.AI_SHOOT, 500, 3, 2)
  aiplayer:push_command(cmdbuf)
  haste_counter_ = 0;
end
--
tut_checkpoints_[4] = function()
  haste_counter_ = haste_counter_ + 1
  if haste_counter_ > 150 then return true end
  return false
end

tut_actions_[4] = function()
  ask_panel_:set_title("Tutorial about shooting:\nYour turn")
end
--
tut_checkpoints_[5] = function()
  -- if sizeof demo_:get_map_cubes_cleared_data() > 1 then
  --  return true
  -- end    
  return false
end

tut_actions_[5] = function()
  ask_panel_:set_title("Tutorial about shooting cubes to align a match")
  
  -- AI setup to shoot 2 more (or 3 more?) cubes to make a match here?? 
end
--
tut_checkpoints_[6] = function()
  return false
end

tut_actions_[6] = function()
  ask_panel_:set_title("Tutorial about a match:\nYour turn")
end
--
tut_checkpoints_[7] = function()
  if demo_:get_map_highest_chain(0) >= 1 then 
    return true
  end
  return false
end

tut_actions_[7] = function() 
  ask_panel_:set_title("Tutorial about chains")

  -- refresh map maybe needed
  -- make AI shoot cubes to make at least 2 combo
end
--
tut_checkpoints_[8] = function()
  return false
end

tut_actions_[8] = function()
  ask_panel_:set_title("Tutorial about chains:\nYour turn")
end
--
tut_checkpoints_[9] = function()
  if demo_:get_map_highest_chain(0) >= 2 then 
    return true
  end
  return false
end

tut_actions_[9] = function()
  ask_panel_:set_title("Tutorial about chains:\nDon't forget to restore the broken cubes")
end
--
tut_checkpoints_[10] = function()
  -- if demo_:get_map_broken_num(0) == 0 then 
    -- return true
  -- end
  return false
end

tut_actions_[10] = function()
  ask_panel_:set_title("Tutorial about overheat:\nDon't do it!")
  
  -- refresh map is needed, a near full map
  -- make AI do many useless shots
end
--
tut_checkpoints_[11] = function()
  return false
end

tut_actions_[11] = function()
  ask_panel_:set_title("Tutorial about emergency")

  -- no need to refresh map, let AI idle for 3 secs 
end
--
tut_checkpoints_[12] = function()
  return false
end

tut_actions_[12] = function()
  ask_panel_:set_title("Tutorial about emergency: about countdown lock")

  -- now following the previous stage, 
  -- make AI shoot for a few secs and let it use chain to lock down  
end
--
tut_checkpoints_[13] = function()
  return false
end

tut_actions_[13] = function()
  ask_panel_:set_title("Tutorial about emergency: your turn")
  -- to not let AI die too fast, it must have some moves here?
end
--
tut_checkpoints_[14] = function()
  -- have to keep track of players' warning_level for a few secs
  -- if the last_warning_level and curr_warning_level can be the same for 3 seconds
  -- then it's passed.
  return false
end

tut_actions_[14] = function()
  ask_panel_:set_title("Tutorial about win/lose")
  -- just simply let the player or AI die. most likely AI will die here 
  -- not sure.
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
