local ffi   = require 'ffi'
local jit   = require 'jit'
local C     = ffi.C
local view  = require 'rc/script/ui/view'
local ui    = require 'rc/script/ui/ui'
local event = require 'rc/script/event/event'
require 'rc/script/strict'
require 'rc/script/ai/ai' -- we have to use AI script here if we ant to control AI player
                          -- but then we'll also need an AI handle... : / 
local function setcmd(buf, type, delay, x, y)
  buf.x, buf.y, buf.delay, buf.type = x, y, delay, type
end

local demo_ 
local scene_ 

local tut_phase_ = 1
local tut_actions_ = {}
local tut_checkpoints_ = {}
local haste_counter_ = 0
local ask_panel_ = nil

local function init(demo)
  demo_  = demo
  scene_ = demo_:get_ui_scene()
  
  -- prepare a dialogue for tutorial
  ask_panel_ = ui.new_askbox{ parent=scene_, w=800, h=480,
                              title="Tutorial",
                              cb={function(self)
                                demo_:eventual_resume()
                                tut_phase_ = tut_phase_ + 1
                                ask_panel_:set_visible(false)
                              end} }
  ask_panel_:set_visible(false)
end

local function update()
  if tut_phase_ > #tut_actions_ then return end
  if tut_checkpoints_[tut_phase_]() then
    ask_panel_:set_visible(true)
    tut_actions_[tut_phase_]()
    demo_:eventual_pause()
  end
end

local function cleanup()
  tut_phase_ = 1
  haste_counter_ = 0
  ask_panel_:set_visible(false)
end

----------/////////////----------

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
  demo_:set_map_dropping(1, false)
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

  demo_:set_map_dropping(0, false)
  
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
  local num = demo_:get_map_cubes_cleared_data(0)[1] + demo_:get_map_cubes_cleared_data(0)[2] +
              demo_:get_map_cubes_cleared_data(0)[3] + demo_:get_map_cubes_cleared_data(0)[4]
  if num >= 1 then
    return true
  end    
  return false
end

tut_actions_[5] = function()
  ask_panel_:set_title("Tutorial about shooting cubes to align a match")
  
  local aiplayer = demo_:get_ai_player()
  local cmdbuf   = ffi.new("LuaAICommand", {0, 0, 0, C.AI_NONE}) 
  setcmd(cmdbuf, C.AI_SHOOT, 1000, 0, 1)
  aiplayer:push_command(cmdbuf)
  setcmd(cmdbuf, C.AI_SHOOT, 500, 2, 1)
  aiplayer:push_command(cmdbuf)
  
  haste_counter_ = 0;
end
--
tut_checkpoints_[6] = function()
  haste_counter_ = haste_counter_ + 1
  if haste_counter_ > 250 then return true end
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

return {
  init = init, 
  update = update, 
  cleanup = cleanup
}
