local parameter   = require 'rc/script/ui/demo/challenge/parameter'
local file        = require 'rc/script/ui/file'


local save_record_ = {}

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
  
  local challenge_record = file.load_data('challenge_record', "rb")

  -- only when win==true we should save clear flag.
  if win then
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
-- Save retry num record
save_record_[parameter.retry] = function(demo, data)
  local submode     = data.submode
  local puzzle_level= data.puzzle_level
  local win         = data.win
  local retry       = data.retry

  local k = 'retry_' .. tostring(puzzle_level)
  
  local challenge_record = file.load_data('challenge_record', "rb")
  
  -- only when win==true we should save retry record
  if win then
    if challenge_record then -- find record file
      -- only if there is not have retry record, we should save it.
      if challenge_record[k]==nil then
        challenge_record[k] = retry
        file.save_data('challenge_record', challenge_record, "wb")
      end
    else -- not have record file, create one & save it.
      challenge_record = {}
      challenge_record[k] = retry
      file.save_data('challenge_record', challenge_record, "wb")
    end
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
  
  local challenge_record = file.load_data('challenge_record', "rb")
  if challenge_record then -- find record file
  
    -- insert score to table
    if challenge_record[k]==nil then challenge_record[k] = {} end
    table.insert( challenge_record[k], score )
    file.save_data('challenge_record', challenge_record, "wb")
    
  else -- not have record file, create one & save it.
    challenge_record = {}
    challenge_record[k] = {}
    table.insert( challenge_record[k], score )
    file.save_data('challenge_record', challenge_record, "wb")
  end
end

------------------------------------------------------
-- Save story mode clear record
save_record_[parameter.story] = function(demo, data)
  local win = data.win
  local ch  = data.character
  
  local k = 'story_' .. tostring(ch)
  
  local challenge_record = file.load_data('challenge_record', "rb")
  if challenge_record then -- find record file
  
    if challenge_record[k]==nil then
      challenge_record[k]=true
      file.save_data('challenge_record', challenge_record, "wb")
    end
    
  else -- not have record file, create one & save it.
    challenge_record = {}
    challenge_record[k]=true
    file.save_data('challenge_record', challenge_record, "wb")
  end
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

local function print_challenge_record_data()
  local challenge_record = file.load_data('challenge_record', "rb")
  if challenge_record then -- find record file
    print("------------ challenge_record data ------------")
  -- Story Clear Info
    local story_clear_info = "Story Clear :"
    for i=1,6 do
      local character_number = tostring(i)
      if challenge_record["story_" .. character_number] then
        story_clear_info = story_clear_info .. " " .. character_number
      end
    end
    print(story_clear_info)
  -- Puzzle Mode Clear Info
    local puzzle_mode_clear_info = "Puzzle Mode Clear :"
    for i=2,parameter.OneShotClearStageNum+1 do -- puzzle mode stage level started from lv2.
      local stage_level = tostring(i)
      if challenge_record["clear_0_" .. stage_level] then
        local stage_number = tostring(i-1) -- the stage number in Game is stage level - 1.
        puzzle_mode_clear_info = puzzle_mode_clear_info .. " " .. stage_number
      end
    end
    print(puzzle_mode_clear_info)
  -- Puzzle Mode Retry Info
    local puzzle_mode_retry_info = "Puzzle Mode Retry :"
    for i=2,parameter.OneShotClearStageNum+1 do -- puzzle mode stage level started from lv2.
      local stage_level = tostring(i)
      if challenge_record["retry_" .. stage_level] then
        local stage_number  = tostring(i-1) -- the stage number in Game is stage level - 1.
        local retry_times   = tostring(challenge_record["retry_" .. stage_level])
        puzzle_mode_retry_info = puzzle_mode_retry_info .. "\n" .. stage_number .. " - " .. retry_times
      end
    end
    print(puzzle_mode_retry_info)
  -- Emergency Mode Clear Info
    local emergency_mode_clear_info = "Emergency Mode Clear :"
    for i=1,5 do
      local condition = "WarningCondition_" .. tostring(i*20)
      local stage_id  = parameter[condition]
      if challenge_record["clear_" .. tostring(stage_id)] then
        emergency_mode_clear_info = emergency_mode_clear_info .. "\n" .. condition
      end
    end
    print(emergency_mode_clear_info)
  -- Unlimited Mode Normal Score
    local unlimited_mode_normal_score = "Unlimited Mode Normal Score :"
    local normal_score = challenge_record["score_" .. tostring(parameter.UnLimited_Normal)]
    if normal_score then
      local sortFunc = function(a, b) return b < a end
      table.sort(normal_score, sortFunc)
      for i=1,10 do
        local s = tostring(i) .. " - " .. tostring(normal_score[i])
        unlimited_mode_normal_score = unlimited_mode_normal_score .. "\n" .. s
      end
    else
      unlimited_mode_normal_score = unlimited_mode_normal_score .. "\nno record"
    end
    print(unlimited_mode_normal_score)
  -- Unlimited Mode Countdown Score
    local unlimited_mode_countdown_score = "Unlimited Mode Countdown Score :"
    local countdown_score = challenge_record["score_" .. tostring(parameter.UnLimited_Countdown)]
    if countdown_score then
      local sortFunc = function(a, b) return b < a end
      table.sort(countdown_score, sortFunc)
      for i=1,10 do
        local s = tostring(i) .. " - " .. tostring(countdown_score[i])
        unlimited_mode_countdown_score = unlimited_mode_countdown_score .. "\n" .. s
      end
    else
      unlimited_mode_countdown_score = unlimited_mode_countdown_score .. "\nno record"
    end
    print(unlimited_mode_countdown_score)
    print("------------------ data end  ------------------")
  else -- not have record file, print "challenge_record file not found"
    print("challenge_record file not found")
  end
end

return {
  save = save,
  load = load,
  print_challenge_record_origin_data  = print_challenge_record_origin_data,
  print_challenge_record_data         = print_challenge_record_data,
}