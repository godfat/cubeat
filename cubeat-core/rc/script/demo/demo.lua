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

----------------------------------------------------------------------------

local demo_
local scene_
local tut_phase_ = 1
local tut_actions_ = {}
local ask_panel_ = nil

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
  ask_panel_:set_visible(true)
  tut_actions_[tut_phase_]()
  demo_:eventual_pause()
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
    --if demo_:get_map_warning_level(1)==100 then -- story win
    if true then
      storyend.set_title('WIN')
      storyend.on_press_next(function(self)
        storyend.hide()
        switch.load_page('talk', nil, {game_mode=99, game_end=true})
        switch.slide_in_page_obj()
      end)
    else
      storyend.set_title('LOSE')
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
  ask_panel_:set_visible(false)
end

tut_actions_[1] = function()
  ask_panel_:set_title("Tutorial about cursor color and player position")
end

tut_actions_[2] = function()
  ask_panel_:set_title("Tutorial about cursor color and player position:\nYour turn")
end

tut_actions_[3] = function()
  ask_panel_:set_title("Tutorial about shooting")
end

tut_actions_[4] = function()
  ask_panel_:set_title("Tutorial about shooting:\nYour turn")
end

tut_actions_[5] = function()
  ask_panel_:set_title("Tutorial about a match")
end

tut_actions_[6] = function()
  ask_panel_:set_title("Tutorial about a match:\nYour turn")
end

tut_actions_[7] = function()
  ask_panel_:set_title("Tutorial about chains")
end

tut_actions_[8] = function()
  ask_panel_:set_title("Tutorial about chains:\nYour turn")
end

tut_actions_[9] = function()
  ask_panel_:set_title("Tutorial about chains:\nDon't forget to restore the broken cubes")
end

tut_actions_[10] = function()
  ask_panel_:set_title("Tutorial about overheat:\nDon't do it!")
end

tut_actions_[11] = function()
  ask_panel_:set_title("Tutorial about emergency")
end

tut_actions_[12] = function()
  ask_panel_:set_title("Tutorial about emergency: about countdown lock")
end

tut_actions_[13] = function()
  ask_panel_:set_title("Tutorial about emergency: your turn")
end

tut_actions_[14] = function()
  ask_panel_:set_title("Tutorial about win/lose")
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
