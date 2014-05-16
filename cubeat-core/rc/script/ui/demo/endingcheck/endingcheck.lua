local ffi     = require 'ffi'
local C       = ffi.C
local view    = require 'rc/script/ui/view'
local ui      = require 'rc/script/ui/ui'
local config  = require 'rc/script/ui/demo/endingcheck/config'
local storyend        = require 'rc/script/ui/demo/storyend/storyend'
local storyend_config = require 'rc/script/ui/demo/storyend/config'
local challenge       = require 'rc/script/ui/demo/challenge/challenge'



local function vs_mode_end(demo)
end

local function tutorial_mode_end(demo)
  -- tutorial win
  if demo:get_map_warning_level(0) ~= 100 then
    print('---- endingcheck: tutorial win ----')
    storyend.show(demo, storyend_config.tutorial_win)
    
  -- tutorial lose
  else
    print('---- endingcheck: tutorial lose ----')
    storyend.show(demo, storyend_config.tutorial_lose)
  
  end
end

local function story_mode_end(demo)
  -- story win
  if demo:get_map_warning_level(1)==100 then
    print('---- endingcheck: story win ----')
    storyend.show(demo, storyend_config.story_win)
    
  -- story lose
  else
    print('---- endingcheck: story lose ----')
    storyend.show(demo, storyend_config.story_lose)
    
  end
end

local function challenge_mode_end(demo, submode)
  -- challenge mode ending
  print('---- endingcheck: challenge mode ending ----')
  challenge.ending(demo, submode)
end


local function show_ending_ui(demo, submode)
  if submode==config.submode_tutorial then
    tutorial_mode_end(demo)
    
  elseif submode==config.submode_story then
    story_mode_end(demo)
    
  else
    challenge_mode_end(demo, submode)
    
  end
end


return {
  show_ending_ui  = show_ending_ui,
}