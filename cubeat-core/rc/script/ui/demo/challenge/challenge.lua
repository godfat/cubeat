local parameter   = require 'rc/script/ui/demo/challenge/parameter'
local file        = require 'rc/script/ui/file'
local recordboard = require 'rc/script/ui/demo/challenge/recordboard'
local scorelist   = require 'rc/script/ui/demo/challenge/scorelist'

local win_              = false  -- win state for SinglePlayer modes.
local puzzle_level_     = 2
local puzzle_retry_     = 0
local level_unlimited_  = false

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
-- Save mode clear flag
local function save_mode_clear_flag(demo, submode)
  local k
  if submode==parameter.OneShotClear then
    k = 'clear_0_' .. tostring(get_puzzle_level())
  else
    k = 'clear_' .. tostring(submode)
  end
  
  local challenge_record = file.load_data('challenge_record', "rb")

  -- only when win_==true we should save clear flag.
  if win_ then
    if challenge_record then -- find record file
      challenge_record[k] = true
      file.save_data('challenge_record', challenge_record, "wb")
    else -- not have record file, create one & save it.
      challenge_record = {}
      challenge_record[k] = true
      file.save_data('challenge_record', challenge_record, "wb")
    end
  end
end

------------------------------------------------------
-- Save play num record
local function save_retry_record(demo, submode)
  local k = 'retry_' .. tostring(get_puzzle_level())
  
  local challenge_record = file.load_data('challenge_record', "rb")
  
  -- only when win_==true we should save retry record
  if win_ then
    if challenge_record then -- find record file
      -- only if there is not have retry record, we should save it.
      if challenge_record[k]==nil then
        challenge_record[k] = puzzle_retry_
        file.save_data('challenge_record', challenge_record, "wb")
      end
    else -- not have record file, create one & save it.
      challenge_record = {}
      challenge_record[k] = puzzle_retry_
      file.save_data('challenge_record', challenge_record, "wb")
    end
  end
end

------------------------------------------------------
-- Save challenge mode score record
local function save_score_record(demo, submode)

  local k = 'score_' .. tostring(submode)
  local cur_score = demo:get_map_score(parameter.player1) -- current score
  
  local challenge_record = file.load_data('challenge_record', "rb")
  if challenge_record then -- find record file
  
    -- insert cur_score to table
    if challenge_record[k]==nil then challenge_record[k] = {} end
    table.insert( challenge_record[k], cur_score )
    file.save_data('challenge_record', challenge_record, "wb")
    
  else -- not have record file, create one & save it.
    challenge_record = {}
    challenge_record[k] = {}
    table.insert( challenge_record[k], cur_score )
    file.save_data('challenge_record', challenge_record, "wb")
  end
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
--WarningCondition
check_condition[parameter.WarningCondition_20] = function(demo)
  local garbage_left  = demo:get_map_garbage_left(parameter.player1)
  local warning_level = demo:get_map_warning_level(parameter.player1)
  if garbage_left==0 and warning_level==0 then
    endgame(demo, true)
  end
end
check_condition[parameter.WarningCondition_40] = function(demo)
  local garbage_left  = demo:get_map_garbage_left(parameter.player1)
  local warning_level = demo:get_map_warning_level(parameter.player1)
  if garbage_left==0 and warning_level==0 then
    endgame(demo, true)
  end
end
check_condition[parameter.WarningCondition_60] = function(demo)
  local garbage_left  = demo:get_map_garbage_left(parameter.player1)
  local warning_level = demo:get_map_warning_level(parameter.player1)
  if garbage_left==0 and warning_level==0 then
    endgame(demo, true)
  end
end
check_condition[parameter.WarningCondition_80] = function(demo)
  local garbage_left  = demo:get_map_garbage_left(parameter.player1)
  local warning_level = demo:get_map_warning_level(parameter.player1)
  if garbage_left==0 and warning_level==0 then
    endgame(demo, true)
  end
end
check_condition[parameter.WarningCondition_100] = function(demo)
  local garbage_left  = demo:get_map_garbage_left(parameter.player1)
  local warning_level = demo:get_map_warning_level(parameter.player1)
  if garbage_left==0 and warning_level==0 then
    endgame(demo, true)
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

  save_mode_clear_flag(demo, submode)
  
  if submode==parameter.OneShotClear then
    recordboard.set_title( win_ and 'SUCCESS' or 'FAIL' )
    save_retry_record(demo, submode)
    local k = 'retry_' .. tostring(get_puzzle_level())
    local challenge_record = file.load_data('challenge_record', "rb")
    if challenge_record then
      local retry = challenge_record[k]
      if win_ and retry then recordboard.set_retry(retry) end
    end
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
      demo:init_single(0, get_puzzle_level(), 'char/char1_new', 'stage/jungle1', true)
    end)
    recordboard.on_press_retry(function(self)
      recordboard.hide()
      puzzle_retry_ = puzzle_retry_ + 1
      demo:init_single(submode, get_puzzle_level(), 'char/char1_new', 'stage/jungle1', true)
    end)
    recordboard.on_press_quit(function(self)
      recordboard.hide() 
      demo:leave_and_cleanup()
    end)
    recordboard.show(submode, win_)
    
  elseif submode==parameter.UnLimited_Normal or submode==parameter.UnLimited_Countdown then
    save_score_record(demo, submode)
    local k = 'score_' .. tostring(submode)
    local challenge_record = file.load_data('challenge_record', "rb")
    if challenge_record and challenge_record[k] then
      local cur_score  = tostring( demo:get_map_score(parameter.player1) )
      scorelist.set_score(challenge_record[k], cur_score, demo:get_ui_scene())
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

  end
end

------------------------------------------------------
-- Cleanup
------------------------------------------------------
local function cleanup()
  set_puzzle_level(2)
  set_level_unlimited(false)
  puzzle_retry_ = 0
  scorelist.remove_score_list()
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
}