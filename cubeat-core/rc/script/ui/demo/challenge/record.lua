local parameter   = require 'rc/script/ui/demo/challenge/parameter'
local file        = require 'rc/script/ui/file'
local stat_list    = require 'rc/script/ui/demo/challenge/stat_list'
local achieve_list= require 'rc/script/ui/demo/challenge/achievement_list'

local save_record_ = {}

------------------------------------------------------

local function save_raw(key, value)
  local challenge_record = file.load_data('challenge_record', "rb")
  if not challenge_record then -- didn't fine record file, create one
    challenge_record = {}
  end
  challenge_record[key] = value
  file.save_data('challenge_record', challenge_record, "wb")
end

local function load_raw(key)
  local challenge_record = file.load_data('challenge_record', "rb")
  if challenge_record==nil then return nil end
  
  return challenge_record[key]
end

------------------------------------------------------
-- Save mode clear flag
save_record_[parameter.clear] = function(demo, data)
  local submode     = data.submode
  local puzzle_level= data.puzzle_level
  local win         = data.win

  local k
  if submode==parameter.OneShotClear then
    k = 'clear_0_' .. tostring(puzzle_level)
  else
    k = 'clear_' .. tostring(submode)
  end

  -- only when win==true we should save clear flag.
  if win then
    save_raw(k, true)
  end
end

------------------------------------------------------
-- Save retry num record
save_record_[parameter.retry] = function(demo, data)
  local submode     = data.submode
  local puzzle_level= data.puzzle_level
  local win         = data.win
  local retry       = data.retry

  local k = 'retry_' .. tostring(puzzle_level)
  
  -- only when win==true we should save retry record
  -- only if there is not have retry record, we should save it.
  if win and not load_raw(k) then
    save_raw(k, retry)
  end
end

------------------------------------------------------
-- Save challenge mode score record
save_record_[parameter.score] = function(demo, data)
  local submode     = data.submode
  local puzzle_level= data.puzzle_level
  local win         = data.win
  local score       = data.score

  local k = 'score_' .. tostring(submode)
  
  local this_score_board = load_raw(k)
  if not this_score_board then 
    this_score_board = {}
  end
  
  table.insert( this_score_board, score )
  
  save_raw(k, this_score_board)
end

------------------------------------------------------
-- Save story mode clear record
save_record_[parameter.story] = function(demo, data)
  local win = data.win
  local ch  = data.character
  
  local k = 'story_' .. tostring(ch)
  
  save_raw(k, true)
end

-- Save story mode quicksave record
save_record_[parameter.quicksave] = function(demo, data)
  local ch    = data.character
  local stage = data.stage  -- clear stage
  
  local k = 'quicksave'
  
  save_raw(k, { ch=ch, stage=stage })
end

-- Save game stats not related to specific game modes
save_record_[parameter.stat] = function(demo, key, value)
  local challenge_record = file.load_data('challenge_record', "rb")

  local k = 'stat_' .. tostring(key)

  save_raw(k, value)  
end

-- Save game achievements
save_record_[parameter.achieve] = function(demo, key, value)
  local challenge_record = file.load_data('challenge_record', "rb")
  
  local k = "achieve_".. tostring(key)
  
  save_raw(k, value)
end

------------------------------------------------------
local function save(demo, save_type, data)
  if save_record_[save_type] then
    save_record_[save_type](demo, data)
  end
end

------------------------------------------------------
local function load(load_type, data)
  local challenge_record = file.load_data('challenge_record', "rb")
  if challenge_record==nil then return nil end
  
  if load_type==parameter.clear then
    if data.submode==nil then return nil end
    local k
    if data.submode==parameter.OneShotClear then
      if data.puzzle_level==nil then return nil end
      k = 'clear_0_' .. tostring(data.puzzle_level)
    else
      k = 'clear_' .. tostring(data.submode)
    end
    return challenge_record[k]
  end
  
  if load_type==parameter.retry then
    if data.puzzle_level==nil then return nil end
    local k = 'retry_' .. tostring(data.puzzle_level)
    return challenge_record[k]
  end
  
  if load_type==parameter.score then
    if data.submode==nil then return nil end
    local k = 'score_' .. tostring(data.submode)
    return challenge_record[k]
  end
  
  if load_type==parameter.story then
    if data.character==nil then return nil end
    local k = 'story_' .. tostring(data.character)
    return challenge_record[k]
  end
end

local function print_challenge_record_origin_data()
  local challenge_record = file.load_data('challenge_record', "rb")
  if challenge_record then -- find record file
    print("-------------- challenge_record origin data --------------")
    for k, v in pairs(challenge_record) do
      print(k, v)
    end
    print("------------------------ data end ------------------------")
  else -- not have record file, print "challenge_record file not found"
    print("challenge_record file not found")
  end
end

local function clear_all_stat_achievement()
  print("\n---------------- * NOTICE * ------------------")
  print("*** CLEAR ALL STATS AND ACHIEVENEMT called ***")
  print("----------------------------------------------\n")
  local challenge_record = file.load_data("challenge_record", "rb")
  if challenge_record then --
    print(" -------- DEBUG: all in game stats and achievements cleared ------- ")
    for k, v in pairs(challenge_record) do
      if string.sub(k, 0, 4) == "stat" or string.sub(k, 0, 7) == "achieve" then
        challenge_record[k] = nil
      end
    end
    file.save_data('challenge_record', challenge_record, "wb")
  end
end

local function populate_stat_achievement_init_value_in_file()
  local challenge_record = file.load_data("challenge_record", "rb")
  if not challenge_record then challenge_record = {} end
  
  -- populate init value based on achievement_list.lua
  for _, v in ipairs(achieve_list) do
    local key = "achieve_"..tostring(v)  
    if not challenge_record[key] then 
      print(" record "..key.." not found... populating default value")
      challenge_record[key] = 0
    end      
  end  
  
  -- populate init value based on stat_list.lua
  for _, v in ipairs(stat_list) do
    local key = "stat_"..tostring(v)
    if not challenge_record[key] then
      print(" record "..key.." not found... populating default value")
      challenge_record[key] = 0
    end
  end
  
  file.save_data("challenge_record", challenge_record, "wb")
end

local function load_stat_achievement_to_C_side(demo)
  print("\n------------- Loading stats from Lua to C ------------------")
  local challenge_record = file.load_data("challenge_record", "rb")
  if challenge_record then --
    for k, v in pairs(challenge_record) do
      if string.sub(k, 0, 4) == "stat" or string.sub(k, 0, 7) == "achieve" then
        print(k, v)
        demo:fill_statistics_from_lua(k, v)
      end
    end
  end
end

local function print_challenge_record_data()
  local challenge_record = file.load_data('challenge_record', "rb")
  if challenge_record then -- find record file
    print("------------ challenge_record data ------------")
  -- Story QuickSave Info
    local t = "QuickSave :"
    if challenge_record["quicksave"] then
      local ch    = challenge_record["quicksave"]["ch"]
      local stage = challenge_record["quicksave"]["stage"]
      t = t .. " ch : " .. tostring(ch) .. " stage : " .. tostring(stage)
    end
    print(t .. "\n")
  -- Story Clear Info
    local t = "Story Clear :"
    for i=1,6 do
      local ch_id = tostring(i)
      if challenge_record["story_" .. ch_id] then
        t = t .. " " .. ch_id
      end
    end
    print(t .. "\n")
  -- Puzzle Mode Clear Info
    local t = "Puzzle Mode Clear :"
    for i=2,parameter.OneShotClearStageNum+1 do -- puzzle mode stage level started from lv2.
      local stage_level   = tostring(i)
      local stage_number  = tostring(i-1) -- the stage number in Game is stage level - 1.
      if challenge_record["clear_0_" .. stage_level] then
        t = t .. "\n" .. "Stage " .. stage_number .. " : " .. "clear"
      else
        t = t .. "\n" .. "Stage " .. stage_number .. " : " .. "no"
      end
    end
    print(t .. "\n")
  -- Puzzle Mode Retry Info
    local t = "Puzzle Mode Retry :"
    for i=2,parameter.OneShotClearStageNum+1 do -- puzzle mode stage level started from lv2.
      local stage_level   = tostring(i)
      local stage_number  = tostring(i-1) -- the stage number in Game is stage level - 1.
      if challenge_record["retry_" .. stage_level] then
        local retry_times = tostring(challenge_record["retry_" .. stage_level])
        t = t .. "\n" .. "Stage " .. stage_number .. " : " .. retry_times
      else
        t = t .. "\n" .. "Stage " .. stage_number .. " : " .. "no"
      end
    end
    print(t .. "\n")
  -- Emergency Mode Clear Info
    local t = "Emergency Mode Clear :"
    for i=1,5 do
      local condition = "WarningCondition_" .. tostring(i*20)
      local stage_id  = parameter[condition]
      if challenge_record["clear_" .. tostring(stage_id)] then
        t = t .. "\n" .. condition .. " : clear"
      else
        t = t .. "\n" .. condition .. " : no"
      end
    end
    print(t .. "\n")
  -- Unlimited Mode Normal Score
    local t = "Unlimited Mode Normal Score :"
    local normal_score = challenge_record["score_" .. tostring(parameter.UnLimited_Normal)]
    if normal_score then
      local sortFunc = function(a, b) return b < a end
      table.sort(normal_score, sortFunc)
      for i=1,10 do
        local s = tostring(i) .. " - " .. tostring(normal_score[i])
        t = t .. "\n" .. s
      end
    else
      t = t .. "\nno record"
    end
    print(t .. "\n")
  -- Unlimited Mode Countdown Score
    local t = "Unlimited Mode Countdown Score :"
    local countdown_score = challenge_record["score_" .. tostring(parameter.UnLimited_Countdown)]
    if countdown_score then
      local sortFunc = function(a, b) return b < a end
      table.sort(countdown_score, sortFunc)
      for i=1,10 do
        local s = tostring(i) .. " - " .. tostring(countdown_score[i])
        t = t .. "\n" .. s
      end
    else
      t = t .. "\nno record"
    end
    print(t)
    print("------------------ data end  ------------------")
  else -- not have record file, print "challenge_record file not found"
    print("challenge_record file not found")
  end
end

return {
  save = save,
  load = load,
  save_raw = save_raw,
  load_raw = load_raw,
  print_challenge_record_origin_data  = print_challenge_record_origin_data,
  print_challenge_record_data         = print_challenge_record_data,
  clear_all_stat_achievement = clear_all_stat_achievement,
  load_stat_achievement_to_C_side = load_stat_achievement_to_C_side,
  populate_stat_achievement_init_value_in_file = populate_stat_achievement_init_value_in_file,
}