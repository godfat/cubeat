local event     = require 'rc/script/event/event'
local parameter = require 'rc/script/ui/demo/challengemenu/parameter'

local win_ = false  -- win state for SinglePlayer modes.

------------------------------------------------------
--
------------------------------------------------------
local function set_win(win)
  win_ = win
end

local function get_win()
  return win_
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
  
  -- WarningCondition_10
  if submode == parameter.WarningCondition_10 then
    demo:init_map_starting_line(0, 10)
    demo:set_map_garbage_amount(0, 10)
  end
  
  -- WarningCondition_20
  if submode == parameter.WarningCondition_20 then
    demo:init_map_starting_line(0, 10)
    demo:set_map_garbage_amount(0, 20)
  end
  
  -- WarningCondition_30
  if submode == parameter.WarningCondition_30 then
    demo:init_map_starting_line(0, 10)
    demo:set_map_garbage_amount(0, 30)
  end
  
  -- WarningCondition_40
  if submode == parameter.WarningCondition_40 then
    --demo:init_map_starting_line(0, 10)
    demo:set_map_garbage_amount(0, 40)
  end
  
  -- WarningCondition_50
  if submode == parameter.WarningCondition_50 then
    demo:init_map_starting_line(0, 10)
    demo:set_map_garbage_amount(0, 50)
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
  if highest_chain>3 or highest_chain==3 then
    endgame(demo, true)
  end
end
check_condition[parameter.Highest_4Chain] = function(demo)
  local highest_chain = demo:get_map_highest_chain(parameter.player1)
  if highest_chain>4 or highest_chain==4 then
    endgame(demo, true)
  end
end
check_condition[parameter.Highest_5Chain] = function(demo)
  local highest_chain = demo:get_map_highest_chain(parameter.player1)
  if highest_chain>5 or highest_chain==5 then
    endgame(demo, true)
  end
end
check_condition[parameter.Highest_3Chain_1Min] = function(demo)
  local cur_time = demo:get_time()
  local highest_chain = demo:get_map_highest_chain(parameter.player1)
  if cur_time<60 and (highest_chain>3 or highest_chain==3) then
    endgame(demo, true)
  elseif cur_time>60 or cur_time==60 then
    endgame(demo, false)
  end
end
check_condition[parameter.Highest_4Chain_2Min] = function(demo)
  local cur_time = demo:get_time()
  local highest_chain = demo:get_map_highest_chain(parameter.player1)
  if cur_time<120 and (highest_chain>4 or highest_chain==4) then
    endgame(demo, true)
  elseif cur_time>120 or cur_time==120 then
    endgame(demo, false)
  end
end
check_condition[parameter.Highest_5Chain_3Min] = function(demo)
  local cur_time = demo:get_time()
  local highest_chain = demo:get_map_highest_chain(parameter.player1)
  if cur_time<180 and (highest_chain>5 or highest_chain==5) then
    endgame(demo, true)
  elseif cur_time>180 or cur_time==180 then
    endgame(demo, false)
  end
end
----------------------------------
--WarningCondition
check_condition[parameter.WarningCondition_10] = function(demo)
  local garbage_left  = demo:get_map_garbage_left(0)
  local warning_level = demo:get_map_warning_level(0)
  if garbage_left==0 and warning_level==0 then
    endgame(demo, true)
  end
end
check_condition[parameter.WarningCondition_20] = function(demo)
  local garbage_left  = demo:get_map_garbage_left(0)
  local warning_level = demo:get_map_warning_level(0)
  if garbage_left==0 and warning_level==0 then
    endgame(demo, true)
  end
end
check_condition[parameter.WarningCondition_30] = function(demo)
  local garbage_left  = demo:get_map_garbage_left(0)
  local warning_level = demo:get_map_warning_level(0)
  if garbage_left==0 and warning_level==0 then
    endgame(demo, true)
  end
end
check_condition[parameter.WarningCondition_40] = function(demo)
  local garbage_left  = demo:get_map_garbage_left(0)
  local warning_level = demo:get_map_warning_level(0)
  --print('------ garbage_left: ' .. tostring(garbage_left) .. ' warning_level: ' .. tostring(warning_level) .. ' ------')
  if garbage_left==0 and warning_level==0 then
    print('-------------- garbage_40 end game --------------')
    endgame(demo, true)
  end
end
check_condition[parameter.WarningCondition_50] = function(demo)
  local garbage_left  = demo:get_map_garbage_left(0)
  local warning_level = demo:get_map_warning_level(0)
  if garbage_left==0 and warning_level==0 then
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


--
return {
  set_win = set_win,
  get_win = get_win,
  --
  init_override                   = init_override,
  check_ending_condition_by_frame = check_ending_condition_by_frame,
}