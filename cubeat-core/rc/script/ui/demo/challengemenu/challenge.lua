local event     = require 'rc/script/event/event'
local parameter = require 'rc/script/ui/demo/challengemenu/parameter'
local file      = require 'rc/script/ui/file'

local win_              = false  -- win state for SinglePlayer modes.
local puzzle_level_     = 2
local level_unlimited_  = false

------------------------------------------------------
--
------------------------------------------------------
local function set_win(win)
  win_ = win
end

local function get_win()
  return win_
end

local function add_puzzle_level(v)
  puzzle_level_ = puzzle_level_ + v
end

local function set_puzzle_level(lv)
  puzzle_level_ = lv
end

local function get_puzzle_level()
  return puzzle_level_
end

local function set_level_unlimited(b)
  level_unlimited_ = b
end

local function get_level_unlimited()
  return level_unlimited_
end

------------------------------------------------------
-- Save challenge mode score record
------------------------------------------------------
local function save_challenge_record(demo, mode)
  local challenge_record = file.load_data('challenge_record')
  if challenge_record then
    local cur_record  = demo:get_time()
    local best_record = challenge_record[mode]
    if (best_record==nil) or (best_record and best_record>cur_record) then
      challenge_record[mode] = cur_record
      file.save_data('challenge_record', challenge_record)
    end
  elseif challenge_record==nil then
    challenge_record = {}
    local record = demo:get_time()
    challenge_record[mode] = record
    file.save_data('challenge_record', challenge_record)
  end
end

------------------------------------------------------
--
------------------------------------------------------
local function endgame(demo, win)
  set_win(win)
  demo:endgame(0)
  print('---------- endgame, win: ' .. tostring(win) .. ' ----------')
end

------------------------------------------------------
--
------------------------------------------------------
local function init_override(demo, in_place, submode)
  -- OneShotClear
  if submode == parameter.OneShotClear then
    --print(demo:get_map_score(1)) 
    --print(demo:get_map_highest_chain(1))
    --print(demo:get_map_cubes_cleared_data(1)[1])
  end
  
  -- WarningCondition_20
  if submode == parameter.WarningCondition_20 then
    demo:init_map_starting_line(0, 10)
    demo:set_map_garbage_amount(0, 20)
  end
  
  -- WarningCondition_40
  if submode == parameter.WarningCondition_40 then
    demo:init_map_starting_line(0, 10)
    demo:set_map_garbage_amount(0, 40)
  end
  
  -- WarningCondition_60
  if submode == parameter.WarningCondition_60 then
    demo:init_map_starting_line(0, 10)
    demo:set_map_garbage_amount(0, 60)
  end
  
  -- WarningCondition_80
  if submode == parameter.WarningCondition_80 then
    demo:init_map_starting_line(0, 10)
    demo:set_map_garbage_amount(0, 80)
  end
  
  -- WarningCondition_100
  if submode == parameter.WarningCondition_100 then
    demo:init_map_starting_line(0, 10)
    demo:set_map_garbage_amount(0, 100)
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
    endgame(demo, true)
  end
end
check_condition[parameter.Highest_4Chain] = function(demo)
  local highest_chain = demo:get_map_highest_chain(parameter.player1)
  if highest_chain>=4 then
    endgame(demo, true)
  end
end
check_condition[parameter.Highest_5Chain] = function(demo)
  local highest_chain = demo:get_map_highest_chain(parameter.player1)
  if highest_chain>=5 then
    endgame(demo, true)
  end
end
check_condition[parameter.Highest_3Chain_1Min] = function(demo)
  local cur_time = demo:get_time()
  local highest_chain = demo:get_map_highest_chain(parameter.player1)
  if cur_time<60 and highest_chain>=3 then
    endgame(demo, true)
  elseif cur_time>=60 then
    endgame(demo, false)
  end
end
check_condition[parameter.Highest_4Chain_2Min] = function(demo)
  local cur_time = demo:get_time()
  local highest_chain = demo:get_map_highest_chain(parameter.player1)
  if cur_time<120 and highest_chain>=4 then
    endgame(demo, true)
  elseif cur_time>=120 then
    endgame(demo, false)
  end
end
check_condition[parameter.Highest_5Chain_3Min] = function(demo)
  local cur_time = demo:get_time()
  local highest_chain = demo:get_map_highest_chain(parameter.player1)
  if cur_time<180 and highest_chain>=5 then
    endgame(demo, true)
  elseif cur_time>=180 then
    endgame(demo, false)
  end
end
----------------------------------
--WarningCondition
check_condition[parameter.WarningCondition_20] = function(demo)
  local garbage_left  = demo:get_map_garbage_left(0)
  local warning_level = demo:get_map_warning_level(0)
  if garbage_left==0 and warning_level==0 then
    endgame(demo, true)
  end
end
check_condition[parameter.WarningCondition_40] = function(demo)
  local garbage_left  = demo:get_map_garbage_left(0)
  local warning_level = demo:get_map_warning_level(0)
  if garbage_left==0 and warning_level==0 then
    endgame(demo, true)
  end
end
check_condition[parameter.WarningCondition_60] = function(demo)
  local garbage_left  = demo:get_map_garbage_left(0)
  local warning_level = demo:get_map_warning_level(0)
  if garbage_left==0 and warning_level==0 then
    endgame(demo, true)
  end
end
check_condition[parameter.WarningCondition_80] = function(demo)
  local garbage_left  = demo:get_map_garbage_left(0)
  local warning_level = demo:get_map_warning_level(0)
  if garbage_left==0 and warning_level==0 then
    endgame(demo, true)
  end
end
check_condition[parameter.WarningCondition_100] = function(demo)
  local garbage_left  = demo:get_map_garbage_left(0)
  local warning_level = demo:get_map_warning_level(0)
  if garbage_left==0 and warning_level==0 then
    endgame(demo, true)
  end
end
----------------------------------
--TimeLimit
check_condition[parameter.TimeLimit_30Cube_1Min] = function(demo)
  local cur_time  = demo:get_time()
  local cube_b    = demo:get_map_cubes_cleared_data(0)[1]
  local cube_g    = demo:get_map_cubes_cleared_data(0)[2]
  local cube_r    = demo:get_map_cubes_cleared_data(0)[3]
  local cube_y    = demo:get_map_cubes_cleared_data(0)[4]
  local cube = cube_b + cube_g + cube_r + cube_y
  if cur_time<60 and cube>=30 then
    endgame(demo, true)
  elseif cur_time>=60 then
    endgame(demo, false)
  end
end
check_condition[parameter.TimeLimit_70Cube_2Min] = function(demo)
  local cur_time  = demo:get_time()
  local cube_b    = demo:get_map_cubes_cleared_data(0)[1]
  local cube_g    = demo:get_map_cubes_cleared_data(0)[2]
  local cube_r    = demo:get_map_cubes_cleared_data(0)[3]
  local cube_y    = demo:get_map_cubes_cleared_data(0)[4]
  local cube = cube_b + cube_g + cube_r + cube_y
  if cur_time<120 and cube>=70 then
    endgame(demo, true)
  elseif cur_time>=120 then
    endgame(demo, false)
  end
end
check_condition[parameter.TimeLimit_20CubeR_1Min] = function(demo)
  local cur_time  = demo:get_time()
  local cube_r    = demo:get_map_cubes_cleared_data(0)[3]
  if cur_time<60 and cube_r>=20 then
    endgame(demo, true)
  elseif cur_time>=60 then
    endgame(demo, false)
  end
end
check_condition[parameter.TimeLimit_50CubeR_2Min] = function(demo)
  local cur_time  = demo:get_time()
  local cube_r    = demo:get_map_cubes_cleared_data(0)[3]
  if cur_time<120 and cube_r>=50 then
    endgame(demo, true)
  elseif cur_time>=120 then
    endgame(demo, false)
  end
end
check_condition[parameter.TimeLimit_15CubeR_15CubeB_1Min] = function(demo)
  local cur_time  = demo:get_time()
  local cube_b    = demo:get_map_cubes_cleared_data(0)[1]
  local cube_r    = demo:get_map_cubes_cleared_data(0)[3]
  if cur_time<60 and cube_b>=15 and cube_r>=15 then
    endgame(demo, true)
  elseif cur_time>=60 then
    endgame(demo, false)
  end
end
check_condition[parameter.TimeLimit_30CubeR_30CubeB_2Min] = function(demo)
  local cur_time  = demo:get_time()
  local cube_b    = demo:get_map_cubes_cleared_data(0)[1]
  local cube_r    = demo:get_map_cubes_cleared_data(0)[3]
  if cur_time<120 and cube_b>=30 and cube_r>=30 then
    endgame(demo, true)
  elseif cur_time>=120 then
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
}