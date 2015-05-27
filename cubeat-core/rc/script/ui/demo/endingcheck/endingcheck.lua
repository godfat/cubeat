local ffi     = require 'ffi'
local C       = ffi.C
local view    = require 'rc/script/ui/view'
local ui      = require 'rc/script/ui/ui'
local config  = require 'rc/script/ui/demo/endingcheck/config'
local storyend        = require 'rc/script/ui/demo/storyend/storyend'
local storyend_config = require 'rc/script/ui/demo/storyend/config'
local challenge       = require 'rc/script/ui/demo/challenge/challenge'
local vsend   = require 'rc/script/ui/demo/vsend/vsend'
local select_config = require 'rc/script/ui/demo/select/config'
local storystage    = require 'rc/script/ui/demo/storyend/config'
local parameter     = require 'rc/script/ui/demo/challenge/parameter'
local record        = require 'rc/script/ui/demo/challenge/record'



local function vs_mode_end(demo, game_mode)

  local p1_win = (demo:get_map_warning_level(1) >= demo:get_map_max_warning_count(1) * 1000 )
  
  vsend.show(demo, game_mode, nil, p1_win)
  
  --[[
  -- PVC
  if game_mode == config.GM_PVC then
    print('---- endingcheck: PVC ----')
  
  -- PVP
  elseif game_mode == config.GM_PVP then
    print('---- endingcheck: PVP ----')
  
  -- CVC
  elseif game_mode == config.GM_CVC then
    print('---- endingcheck: CVC ----')
  end
  --]]
end

local function tutorial_mode_end(demo)
  -- tutorial win
  if demo:get_map_warning_level(0) < demo:get_map_max_warning_count(0) * 1000 then
    print('---- endingcheck: tutorial win ----')
    storyend.show(demo, storyend_config.tutorial_win)
    
  -- tutorial lose
  else
    print('---- endingcheck: tutorial lose ----')
    storyend.show(demo, storyend_config.tutorial_lose)
  
  end
end

-- story_mode is inheritly Player vs CPU mode. 
local function story_mode_end(demo, submode)
  -- story win
  if demo:get_map_warning_level(1) >= demo:get_map_max_warning_count(1) * 1000 then
    print('---- endingcheck: story win ----')
    --storyend.show(demo, storyend_config.story_win)
    vsend.show(demo, config.GM_PVC, submode, true)
    -- save story quicksave data
    local data = { character=select_config.ch_choose[1], stage=storystage.get_stage() }
    record.save(demo, parameter.quicksave, data)
    -- save story character clear data
    if storystage.get_stage()==6 then
      print('-------- save story character clear data --------')
      local data = { win='true', character=select_config.ch_choose[1] }
      record.save(demo, parameter.story, data)
      challenge.update_achievement("achieve_story_"..select_config.ch_choose[1], true)
    end

  -- story lose
  else
    print('---- endingcheck: story lose ----')
    --storyend.show(demo, storyend_config.story_lose)
    vsend.show(demo, config.GM_PVC, submode, false)
    
  end
end

local function challenge_mode_end(demo, submode)
  -- challenge mode ending
  print('---- endingcheck: challenge mode ending ----')
  challenge.ending(demo, submode)
end


local function show_ending_ui(demo, game_mode, submode)
  
  -- not vs mode ending
  if game_mode==config.GM_SINGLE or (game_mode==config.GM_PVC and submode==config.submode_story) or game_mode==config.GM_TUT then
    if submode==config.submode_tutorial then
      tutorial_mode_end(demo)
    elseif submode==config.submode_story then
      story_mode_end(demo, submode)
    else
      challenge_mode_end(demo, submode)
    end
  
  -- vs mode ending
  else
    vs_mode_end(demo, game_mode)
  end
end


return {
  show_ending_ui  = show_ending_ui,
}