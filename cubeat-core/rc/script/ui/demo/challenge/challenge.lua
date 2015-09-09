local parameter   = require 'rc/script/ui/demo/challenge/parameter'
local file        = require 'rc/script/ui/file'
local record      = require 'rc/script/ui/demo/challenge/record'
local recordboard = require 'rc/script/ui/demo/challenge/recordboard'
local scorelist   = require 'rc/script/ui/demo/challenge/scorelist'
local achievement_text = require 'rc/script/ui/demo/challenge/achievement_popup'

local win_              = false  -- win state for SinglePlayer modes.
local puzzle_level_     = 2
local puzzle_retry_     = 0
local level_unlimited_  = false
local update_achievement -- function forward decl

-- puzzle_level_
local function add_puzzle_level(v)
  puzzle_level_ = puzzle_level_ + v
end
local function set_puzzle_level(lv)
  puzzle_level_ = lv
end
local function get_puzzle_level()
  return puzzle_level_
end

-- level_unlimited_
local function set_level_unlimited(b)
  level_unlimited_ = b
end
local function get_level_unlimited()
  return level_unlimited_
end



------------------------------------------------------
-- Set win_ variable & end single mode game
local function endgame(demo, win)
  print('---- endgame, win: ' .. tostring(win) .. ' ----')
  
  -- NOTE: 
  -- In the merged "SinglePlayer" game modes, what you passed into endgame() is not relevent.
  -- You have to track win/lose state in Lua.
  win_ = win
  demo:endgame(parameter.player1)
end



------------------------------------------------------
-- Init Override
------------------------------------------------------
local function init_override(demo, in_place, submode)

  win_ = false  -- re-initialize certain states

  -- OneShotClear
  if submode == parameter.OneShotClear then
    demo:set_only_one_shot_for_puzzle()
    -- Currently don't do anything else than this when initializing PUZZLE.
    -- C++ handles generating PUZZLE map when initialization
  end
  
  -- WarningCondition_20
  if submode == parameter.WarningCondition_20 then
    demo:init_map_starting_line(parameter.player1, 10)
    demo:set_map_garbage_amount(parameter.player1, 20)
  end
  
  -- WarningCondition_40
  if submode == parameter.WarningCondition_40 then
    demo:init_map_starting_line(parameter.player1, 10)
    demo:set_map_garbage_amount(parameter.player1, 40)
  end
  
  -- WarningCondition_60
  if submode == parameter.WarningCondition_60 then
    demo:init_map_starting_line(parameter.player1, 10)
    demo:set_map_garbage_amount(parameter.player1, 60)
  end
  
  -- WarningCondition_80
  if submode == parameter.WarningCondition_80 then
    demo:init_map_starting_line(parameter.player1, 10)
    demo:set_map_garbage_amount(parameter.player1, 80)
  end
  
  -- WarningCondition_100
  if submode == parameter.WarningCondition_100 then
    demo:init_map_starting_line(parameter.player1, 10)
    demo:set_map_garbage_amount(parameter.player1, 100)
  end
  
  -- UnLimited_Normal
  if submode == parameter.UnLimited_Normal then
    if in_place==false then
      scorelist.create_score_list(demo:get_ui_scene())
    end
  end
  
  -- UnLimited_Countdown
  if submode == parameter.UnLimited_Countdown then
    demo:set_time(120)
    demo:set_countdown(true)
    if in_place==false then
      scorelist.create_score_list(demo:get_ui_scene())
    end
  end  
end

------------------------------------------------------
-- All Check Condition Function
------------------------------------------------------
local check_condition = {}
----------------------------------
-- OneShotClear
check_condition[parameter.OneShotClear] = function(demo)
  if demo:is_puzzle_started() then
    if demo:is_map_empty(parameter.player1) then
      endgame(demo, true)
    elseif demo:is_map_all_waiting(parameter.player1) then
      endgame(demo, false)
    end
  end
end
----------------------------------

  --[[ 
    Just got surprised by a fucking logical problem in Emergency's winning condition: 
    During the course of game it is POSSIBLE to make warning_level down to ZERO very briefly
    And if at that instant the garbages happen to have been all dropped & also some chaining condition still exist down below
    It is possible to win the game even if you still have cubes AT THE TOP ROW 
    (which should trigger warning under normal circumstance 
     -- but because chaining condition still exist and/or below_is_empty(), it won't)
  ]]

--WarningCondition
check_condition[parameter.WarningCondition_20] = function(demo)
  local garbage_left  = demo:get_map_garbage_left(parameter.player1)
  local warning_level = demo:get_map_warning_level(parameter.player1)
  if garbage_left==0 then
    if demo:is_map_dropping(parameter.player1) then
      demo:set_map_dropping(parameter.player1, false)
    end
    if warning_level == 0 then 
      endgame(demo, true)
    end
  end
end
check_condition[parameter.WarningCondition_40] = function(demo)
  local garbage_left  = demo:get_map_garbage_left(parameter.player1)
  local warning_level = demo:get_map_warning_level(parameter.player1)
  if garbage_left==0 then
    if demo:is_map_dropping(parameter.player1) then
      demo:set_map_dropping(parameter.player1, false)
    end
    if warning_level == 0 then 
      endgame(demo, true)
    end
  end
end
check_condition[parameter.WarningCondition_60] = function(demo)
  local garbage_left  = demo:get_map_garbage_left(parameter.player1)
  local warning_level = demo:get_map_warning_level(parameter.player1)
  if garbage_left==0 then
    if demo:is_map_dropping(parameter.player1) then
      demo:set_map_dropping(parameter.player1, false)
    end
    if warning_level == 0 then 
      endgame(demo, true)
    end
  end
end
check_condition[parameter.WarningCondition_80] = function(demo)
  local garbage_left  = demo:get_map_garbage_left(parameter.player1)
  local warning_level = demo:get_map_warning_level(parameter.player1)
  if garbage_left==0 then
    if demo:is_map_dropping(parameter.player1) then
      demo:set_map_dropping(parameter.player1, false)
    end
    if warning_level == 0 then 
      endgame(demo, true)
    end
  end
end
check_condition[parameter.WarningCondition_100] = function(demo)
  local garbage_left  = demo:get_map_garbage_left(parameter.player1)
  local warning_level = demo:get_map_warning_level(parameter.player1)
  if garbage_left==0 then
    if demo:is_map_dropping(parameter.player1) then
      demo:set_map_dropping(parameter.player1, false)
    end
    if warning_level == 0 then 
      endgame(demo, true)
    end
  end
end
----------------------------------
--UnLimited
check_condition[parameter.UnLimited_Normal] = function(demo)
end
check_condition[parameter.UnLimited_Countdown] = function(demo)
  local t = demo:get_time()
  if t<=0 then
    endgame(demo, true)
  end
end

------------------------------------------------------
-- Check Ending Condition By Frame
------------------------------------------------------
local function check_ending_condition_by_frame(demo, submode)
  if check_condition[submode] then
    check_condition[submode](demo)
  end
end

------------------------------------------------------
-- Ending
------------------------------------------------------
local function ending(demo, submode)
  -- save mode clear flag
  local data = { win=win_, submode=submode, puzzle_level=get_puzzle_level() }
  record.save(demo, parameter.clear, data)
  
  if submode==parameter.OneShotClear then
    -- save retry num
    local data = { win=win_, submode=submode, puzzle_level=get_puzzle_level(), retry=puzzle_retry_ }
    record.save(demo, parameter.retry, data)
    -- set record board
    recordboard.set_title( win_ and 'SUCCESS' or 'FAIL' )
    local record_retry = record.load(parameter.retry, {puzzle_level=get_puzzle_level()})
    if win_ and record_retry then recordboard.set_retry(record_retry) end
    -- set button on_press function
    recordboard.on_press_next(function(self)
      recordboard.hide()
      if win_ then
        if get_puzzle_level() < 19 then add_puzzle_level(1) end
        if get_puzzle_level() < 2  then set_puzzle_level(2) end
      end
      -- init SinglePlayer, in Submode 0, and Level is decided by puzzle_level_ variable
      -- the last true means "in_place" is true, there won't be slide-in/out effects.
      puzzle_retry_ = 0
      if get_level_unlimited() == true then
        demo:init_single(0, get_puzzle_level() + 20, 'char/char1_new', 'stage/jungle1', true)
      else 
        demo:init_single(0, get_puzzle_level(), 'char/char1_new', 'stage/jungle1', true)
      end
    end)
    recordboard.on_press_retry(function(self)
      recordboard.hide()
      puzzle_retry_ = puzzle_retry_ + 1
      if get_level_unlimited() == true then
        demo:init_single(0, get_puzzle_level() + 20, 'char/char1_new', 'stage/jungle1', true)
      else 
        demo:init_single(0, get_puzzle_level(), 'char/char1_new', 'stage/jungle1', true)
      end
    end)
    recordboard.on_press_quit(function(self)
      recordboard.hide() 
      demo:leave_and_cleanup()
    end)
    recordboard.show(submode, win_)
    
    -- need to determine how to distinguish stock levels and rng levels to make achievements
    -- update_achievement("achieve_puzzle_stages_cleared", true)
    
  elseif submode==parameter.UnLimited_Normal or submode==parameter.UnLimited_Countdown then
    -- save score
    local data = { win=win_, submode=submode, puzzle_level=get_puzzle_level(), score=demo:get_map_score(parameter.player1) }
    record.save(demo, parameter.score, data)
    -- set score list
    local record_score = record.load(parameter.score, {submode=submode})
    if record_score then
      local cur_score  = tostring( demo:get_map_score(parameter.player1) )
      scorelist.set_score(record_score, cur_score, demo:get_ui_scene())
    end
    -- set button on_press function
    scorelist.on_press_retry(function(self)
      scorelist.set_visible(false)
      demo:init_single(submode, 1, 'char/char1_new', 'stage/jungle1', true)
    end)
    scorelist.on_press_quit(function(self)
      scorelist.set_visible(false)
      demo:leave_and_cleanup()
    end)
    scorelist.set_visible(true)
    
    if data.score >= 15000 then
      update_achievement("achieve_score_attack_mastered", true)
    end
    
  else
    recordboard.set_title( win_ and 'SUCCESS' or 'FAIL' )
    -- set button on_press function
    recordboard.on_press_retry(function(self)
      recordboard.hide()
      demo:init_single(submode, 1, 'char/char1_new', 'stage/jungle1', true)
    end)
    recordboard.on_press_quit(function(self)
      recordboard.hide() 
      demo:leave_and_cleanup()
    end)
    recordboard.show(submode, win_)

    if win_ and submode == parameter.WarningCondition_100 then
      update_achievement("achieve_emergency_stages_cleared", true)
    end
    
  end
end

------------------------------------------------------
-- Cleanup
------------------------------------------------------
local function cleanup()
  set_puzzle_level(2)
  set_level_unlimited(false)
  puzzle_retry_ = 0
  scorelist.remove_score()
end

------------------------------------------------------
-- Achievement
------------------------------------------------------

-- Create achievement text
local function create_achievement_text(scene)
  achievement_text.create(scene)
end

-- Update achievement (forward declared) 
update_achievement = function(key, value)
  if key == "stat_highest_chain" then
    if value >= 4 and not record.load_raw("achieve_highest_chain_4") then
      record.save_raw("achieve_highest_chain_4", true)
      achievement_text.pop_achievement_ui("highest_chain_4")
    end
    if value >= 6 and not record.load_raw("achieve_highest_chain_6") then
      record.save_raw("achieve_highest_chain_6", true)
      achievement_text.pop_achievement_ui("highest_chain_6")
    end
  end
  
  if key == "stat_highest_color_count" then 
    if value >= 3 and not record.load_raw("achieve_three_color_match") then
      record.save_raw("achieve_three_color_match", true)
      achievement_text.pop_achievement_ui("three_color_match")
    end
  end 
  
  if key == "stat_highest_single_color_match" then 
    if value >= 9 and not record.load_raw("achieve_single_color_match_9") then
      record.save_raw("achieve_single_color_match_9", true)
      achievement_text.pop_achievement_ui("single_color_match_9")
    end
  end 
  
  -- NOTE: future cleanup needed
  
  if key == "achieve_story_1" and value == true and not record.load_raw("achieve_story_1") then
    record.save_raw("achieve_story_1", true)
    achievement_text.pop_achievement_ui("story_1")
  end

  if key == "achieve_story_2" and value == true and not record.load_raw("achieve_story_2") then
    record.save_raw("achieve_story_2", true)
    achievement_text.pop_achievement_ui("story_2")
  end
  
  if key == "achieve_story_3" and value == true and not record.load_raw("achieve_story_3") then
    record.save_raw("achieve_story_3", true)
    achievement_text.pop_achievement_ui("story_3")
  end
  
  if key == "achieve_story_4" and value == true and not record.load_raw("achieve_story_4") then
    record.save_raw("achieve_story_4", true)
    achievement_text.pop_achievement_ui("story_4")
  end
  
  if key == "achieve_story_5" and value == true and not record.load_raw("achieve_story_5") then
    record.save_raw("achieve_story_5", true)
    achievement_text.pop_achievement_ui("story_5")
  end
  
  if key == "achieve_story_6" and value == true and not record.load_raw("achieve_story_6") then
    record.save_raw("achieve_story_6", true)
    achievement_text.pop_achievement_ui("story_6")
  end
  
  if key == "achieve_emergency_stages_cleared" and not record.load_raw(key) then
    record.save_raw(key, true)
    achievement_text.pop_achievement_ui("emergency_stages_cleared")
  end
  
  if key == "achieve_puzzle_stages_cleared" and not record.load_raw(key) then
    record.save_raw(key, true)
    achievement_text.pop_achievement_ui("puzzle_stages_cleared")
  end
  
  if key == "achieve_score_attack_mastered" and not record.load_raw(key) then
    record.save_raw(key, true)
    achievement_text.pop_achievement_ui("score_attack_mastered")
  end
  
  -- below are achievements that passed in from C++ side
  
  if key == "achieve_garbage_left_60" and value == true then
    achievement_text.pop_achievement_ui("garbage_left_60")
  end
  
  if key == "achieve_efficiency_over_time" and value == true then
    achievement_text.pop_achievement_ui("efficiency_over_time")
  end
  
  if key == "achieve_two_mice_pvp" and value == true then 
    achievement_text.pop_achievement_ui("two_mice_pvp")
  end
  
  if key == "achieve_shoot_opponent" and value == true then
    achievement_text.pop_achievement_ui("shoot_opponent")
  end
  
  if key == "achieve_win_veryhard_no_haste" and value == true then
    achievement_text.pop_achievement_ui("win_veryhard_no_haste")
  end
  
  if key == "achieve_win_lightning_fast" and value == true then
    achievement_text.pop_achievement_ui("win_lightning_fast")
  end
  
  if key == "achieve_overheat" and value == true then
    achievement_text.pop_achievement_ui("overheat")
  end
  
  if key == "achieve_win_overkill1" and value == true then
    achievement_text.pop_achievement_ui("win_overkill1")
  end
 
  if key == "achieve_win_overkill2" and value == true then
    achievement_text.pop_achievement_ui("win_overkill2")
  end
  
  if key == "achieve_win_safety_first" and value == true then
    achievement_text.pop_achievement_ui("win_safety_first")
  end
  
  if key == "achieve_long_struggle" and value == true then
    achievement_text.pop_achievement_ui("long_struggle")
  end
  
  if key == "achieve_win_turn_the_tide" and value == true then
    achievement_text.pop_achievement_ui("win_turn_the_tide")
  end
  
  if key == "achieve_win_so_close" and value == true then
    achievement_text.pop_achievement_ui("win_so_close")
  end
  
  if key == "achieve_win_underdog" and value == true then
    achievement_text.pop_achievement_ui("win_underdog")
  end
  
  if key == "achieve_lose_careless" and value == true then
    achievement_text.pop_achievement_ui("lose_careless")
  end
end

--
return {
  add_puzzle_level    = add_puzzle_level,
  set_puzzle_level    = set_puzzle_level,
  get_puzzle_level    = get_puzzle_level,
  set_level_unlimited = set_level_unlimited,
  get_level_unlimited = get_level_unlimited,
  --
  init_override                   = init_override,
  check_ending_condition_by_frame = check_ending_condition_by_frame,
  ending                          = ending,
  cleanup                         = cleanup,
  --
  create_achievement_text         = create_achievement_text,
  update_achievement              = update_achievement,
}