local parameter   = require 'rc/script/ui/demo/challenge/parameter'
local file        = require 'rc/script/ui/file'
local scoreblock  = require 'rc/script/ui/demo/challenge/scoreblock'
local recordboard = require 'rc/script/ui/demo/challenge/recordboard'

local win_              = false  -- win state for SinglePlayer modes.
local puzzle_level_     = 2
local level_unlimited_  = false


------------------------------------------------------
--
------------------------------------------------------
-- win_
local function set_win(win)
  win_ = win
end
local function get_win()
  return win_
end

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

-- remove score block
local function remove_score_block()
  scoreblock.remove_score_block()
end


-- Get current used time
local function get_cur_time(demo, submode)
  -- Submode that limit time is 1 min
  if submode==parameter.Highest_3Chain_1Min
  or submode==parameter.TimeLimit_30Cube_1Min
  or submode==parameter.TimeLimit_20CubeR_1Min
  or submode==parameter.TimeLimit_15CubeR_15CubeB_1Min then
    local t = 60 - demo:get_time()
    return t
  -- Submode that limit time is 2 min
  elseif submode==parameter.Highest_4Chain_2Min
  or submode==parameter.TimeLimit_70Cube_2Min
  or submode==parameter.TimeLimit_50CubeR_2Min
  or submode==parameter.TimeLimit_30CubeR_30CubeB_2Min then
    local t = 120 - demo:get_time()
    return t
  -- Submode that limit time is 3 min
  elseif submode==parameter.Highest_5Chain_3Min then
    local t = 180 - demo:get_time()
    return t
  -- no limit time
  else
    return demo:get_time()
  end
end

------------------------------------------------------
-- Save challenge mode score record
------------------------------------------------------
local function save_challenge_record(demo, submode)

  local cur_record = get_cur_time(demo, submode) -- current record
  
  local challenge_record = file.load_data('challenge_record')
  if challenge_record then -- find record file    
  
    -- if this submode not have record or get new best record, save it.
    local best_record = challenge_record[tostring(submode)]
    if (best_record==nil) or (cur_record<best_record) then
      challenge_record[tostring(submode)] = cur_record
      file.save_data('challenge_record', challenge_record)
    end
    
  elseif challenge_record==nil then -- not have record file, create one & save it.
    challenge_record = {}
    challenge_record[tostring(submode)] = cur_record
    file.save_data('challenge_record', challenge_record)
  end
end

------------------------------------------------------
--
------------------------------------------------------
local function endgame(demo, win)
  set_win(win)
  demo:endgame(parameter.player1)
  print('---------- endgame, win: ' .. tostring(win) .. ' ----------')
end

------------------------------------------------------
--
------------------------------------------------------
local function init_override(demo, in_place, submode, scene)
  -- OneShotClear
  if submode == parameter.OneShotClear then
    --print(demo:get_map_score(1)) 
    --print(demo:get_map_highest_chain(1))
    --print(demo:get_map_cubes_cleared_data(1)[1])
  end

  -- Set time countdown 60 second
  if submode == parameter.Highest_3Chain_1Min
  or submode == parameter.TimeLimit_30Cube_1Min
  or submode == parameter.TimeLimit_20CubeR_1Min
  or submode == parameter.TimeLimit_15CubeR_15CubeB_1Min then
    demo:set_time(60)
    demo:set_countdown(true)
  end
  
  -- Set time countdown 120 second
  if submode == parameter.Highest_4Chain_2Min
  or submode == parameter.TimeLimit_70Cube_2Min
  or submode == parameter.TimeLimit_50CubeR_2Min
  or submode == parameter.TimeLimit_30CubeR_30CubeB_2Min then
    demo:set_time(120)
    demo:set_countdown(true)  
  end
  
  -- Set time countdown 180 second
  if submode == parameter.Highest_5Chain_3Min then
    demo:set_time(180)
    demo:set_countdown(true)
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
  
  -- Create 1 score block
  if submode == parameter.TimeLimit_30Cube_1Min 
  or submode == parameter.TimeLimit_70Cube_2Min
  or submode == parameter.TimeLimit_20CubeR_1Min
  or submode == parameter.TimeLimit_50CubeR_2Min then
    if in_place==false then scoreblock.create_score_block(scene)
    else set_score(0) -- init score
    end
  end
  
  -- Create 2 score block (Red & Blue)
  if submode == parameter.TimeLimit_15CubeR_15CubeB_1Min
  or submode == parameter.TimeLimit_30CubeR_30CubeB_2Min then
    if in_place==false then scoreblock.create_score_block_double(scene)
    else set_double_score(0, 0) -- init score
    end
  end
end

------------------------------------------------------
-- All Check Condition Function
------------------------------------------------------
local check_condition = {}
----------------------------------
-- HighestChain
check_condition[parameter.Highest_3Chain] = function(demo)
  local highest_chain = demo:get_map_highest_chain(parameter.player1)
  if highest_chain>=3 then
    save_challenge_record(demo, parameter.Highest_3Chain)
    endgame(demo, true)
  end
end
check_condition[parameter.Highest_4Chain] = function(demo)
  local highest_chain = demo:get_map_highest_chain(parameter.player1)
  if highest_chain>=4 then
    save_challenge_record(demo, parameter.Highest_4Chain)
    endgame(demo, true)
  end
end
check_condition[parameter.Highest_5Chain] = function(demo)
  local highest_chain = demo:get_map_highest_chain(parameter.player1)
  if highest_chain>=5 then
    save_challenge_record(demo, parameter.Highest_5Chain)
    endgame(demo, true)
  end
end
check_condition[parameter.Highest_3Chain_1Min] = function(demo)
  local cur_time = demo:get_time()
  local highest_chain = demo:get_map_highest_chain(parameter.player1)
  if cur_time>0 and highest_chain>=3 then
    save_challenge_record(demo, parameter.Highest_3Chain_1Min)
    endgame(demo, true)
  elseif cur_time<=0 then
    endgame(demo, false)
  end
end
check_condition[parameter.Highest_4Chain_2Min] = function(demo)
  local cur_time = demo:get_time()
  local highest_chain = demo:get_map_highest_chain(parameter.player1)
  if cur_time>0 and highest_chain>=4 then
    save_challenge_record(demo, parameter.Highest_4Chain_2Min)
    endgame(demo, true)
  elseif cur_time<=0 then
    endgame(demo, false)
  end
end
check_condition[parameter.Highest_5Chain_3Min] = function(demo)
  local cur_time = demo:get_time()
  local highest_chain = demo:get_map_highest_chain(parameter.player1)
  if cur_time>0 and highest_chain>=5 then
    save_challenge_record(demo, parameter.Highest_5Chain_3Min)
    endgame(demo, true)
  elseif cur_time<=0 then
    endgame(demo, false)
  end
end
----------------------------------
--WarningCondition
check_condition[parameter.WarningCondition_20] = function(demo)
  local garbage_left  = demo:get_map_garbage_left(parameter.player1)
  local warning_level = demo:get_map_warning_level(parameter.player1)
  if garbage_left==0 and warning_level==0 then
    save_challenge_record(demo, parameter.WarningCondition_20)
    endgame(demo, true)
  end
end
check_condition[parameter.WarningCondition_40] = function(demo)
  local garbage_left  = demo:get_map_garbage_left(parameter.player1)
  local warning_level = demo:get_map_warning_level(parameter.player1)
  if garbage_left==0 and warning_level==0 then
    save_challenge_record(demo, parameter.WarningCondition_40)
    endgame(demo, true)
  end
end
check_condition[parameter.WarningCondition_60] = function(demo)
  local garbage_left  = demo:get_map_garbage_left(parameter.player1)
  local warning_level = demo:get_map_warning_level(parameter.player1)
  if garbage_left==0 and warning_level==0 then
    save_challenge_record(demo, parameter.WarningCondition_60)
    endgame(demo, true)
  end
end
check_condition[parameter.WarningCondition_80] = function(demo)
  local garbage_left  = demo:get_map_garbage_left(parameter.player1)
  local warning_level = demo:get_map_warning_level(parameter.player1)
  if garbage_left==0 and warning_level==0 then
    save_challenge_record(demo, parameter.WarningCondition_80)
    endgame(demo, true)
  end
end
check_condition[parameter.WarningCondition_100] = function(demo)
  local garbage_left  = demo:get_map_garbage_left(parameter.player1)
  local warning_level = demo:get_map_warning_level(parameter.player1)
  if garbage_left==0 and warning_level==0 then
    save_challenge_record(demo, parameter.WarningCondition_100)
    endgame(demo, true)
  end
end
----------------------------------
--TimeLimit
check_condition[parameter.TimeLimit_30Cube_1Min] = function(demo)
  local cur_time  = demo:get_time()
  local cube_b    = demo:get_map_cubes_cleared_data(parameter.player1)[parameter.cube_b]
  local cube_g    = demo:get_map_cubes_cleared_data(parameter.player1)[parameter.cube_g]
  local cube_r    = demo:get_map_cubes_cleared_data(parameter.player1)[parameter.cube_r]
  local cube_y    = demo:get_map_cubes_cleared_data(parameter.player1)[parameter.cube_y]
  local cube = cube_b + cube_g + cube_r + cube_y
  scoreblock.set_score(cube)
  if cur_time>0 and cube>=30 then
    save_challenge_record(demo, parameter.TimeLimit_30Cube_1Min)
    endgame(demo, true)
  elseif cur_time<=0 then
    endgame(demo, false)
  end
end
check_condition[parameter.TimeLimit_70Cube_2Min] = function(demo)
  local cur_time  = demo:get_time()
  local cube_b    = demo:get_map_cubes_cleared_data(parameter.player1)[parameter.cube_b]
  local cube_g    = demo:get_map_cubes_cleared_data(parameter.player1)[parameter.cube_g]
  local cube_r    = demo:get_map_cubes_cleared_data(parameter.player1)[parameter.cube_r]
  local cube_y    = demo:get_map_cubes_cleared_data(parameter.player1)[parameter.cube_y]
  local cube = cube_b + cube_g + cube_r + cube_y
  scoreblock.set_score(cube)
  if cur_time>0 and cube>=70 then
    save_challenge_record(demo, parameter.TimeLimit_70Cube_2Min)
    endgame(demo, true)
  elseif cur_time<=0 then
    endgame(demo, false)
  end
end
check_condition[parameter.TimeLimit_20CubeR_1Min] = function(demo)
  local cur_time  = demo:get_time()
  local cube_r    = demo:get_map_cubes_cleared_data(parameter.player1)[parameter.cube_r]
  scoreblock.set_score(cube_r)
  if cur_time>0 and cube_r>=20 then
    save_challenge_record(demo, parameter.TimeLimit_20CubeR_1Min)
    endgame(demo, true)
  elseif cur_time<=0 then
    endgame(demo, false)
  end
end
check_condition[parameter.TimeLimit_50CubeR_2Min] = function(demo)
  local cur_time  = demo:get_time()
  local cube_r    = demo:get_map_cubes_cleared_data(parameter.player1)[parameter.cube_r]
  scoreblock.set_score(cube_r)
  if cur_time>0 and cube_r>=50 then
    save_challenge_record(demo, parameter.TimeLimit_50CubeR_2Min)
    endgame(demo, true)
  elseif cur_time<=0 then
    endgame(demo, false)
  end
end
check_condition[parameter.TimeLimit_15CubeR_15CubeB_1Min] = function(demo)
  local cur_time  = demo:get_time()
  local cube_b    = demo:get_map_cubes_cleared_data(parameter.player1)[parameter.cube_b]
  local cube_r    = demo:get_map_cubes_cleared_data(parameter.player1)[parameter.cube_r]
  scoreblock.set_score_double(cube_r, cube_b)
  if cur_time>0 and cube_b>=15 and cube_r>=15 then
    save_challenge_record(demo, parameter.TimeLimit_15CubeR_15CubeB_1Min)
    endgame(demo, true)
  elseif cur_time<=0 then
    endgame(demo, false)
  end
end
check_condition[parameter.TimeLimit_30CubeR_30CubeB_2Min] = function(demo)
  local cur_time  = demo:get_time()
  local cube_b    = demo:get_map_cubes_cleared_data(parameter.player1)[parameter.cube_b]
  local cube_r    = demo:get_map_cubes_cleared_data(parameter.player1)[parameter.cube_r]
  scoreblock.set_score_double(cube_r, cube_b)
  if cur_time>0 and cube_b>=30 and cube_r>=30 then
    save_challenge_record(demo, parameter.TimeLimit_30CubeR_30CubeB_2Min)
    endgame(demo, true)
  elseif cur_time<=0 then
    endgame(demo, false)
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
  recordboard.set_title( win_ and 'SUCCESS' or 'FAIL' )
  
  if submode==parameter.OneShotClear then
  else
    local challenge_record = file.load_data('challenge_record')
    if challenge_record then
      local cur_time  = get_cur_time(demo, submode)
      local best_time = challenge_record[tostring(submode)]
      recordboard.set_cur_time( tostring(cur_time) .. ' sec' )
      recordboard.set_best_time( tostring(best_time) .. ' sec' )
    end
    
    recordboard.on_press_retry(function(self)
      recordboard.hide()
      demo:init_single(submode, 1, 'char/char1_new', 'stage/jungle1', true)
    end)
    recordboard.on_press_quit(function(self)
      recordboard.hide(false) 
      demo:leave_and_cleanup()
    end)
    
    recordboard.show(submode)
  end
end

------------------------------------------------------
-- Cleanup
------------------------------------------------------
local function cleanup()
  set_puzzle_level(2)
  set_level_unlimited(false)
  scoreblock.remove_score_block()
end


--
return {
  set_win             = set_win,
  get_win             = get_win,
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