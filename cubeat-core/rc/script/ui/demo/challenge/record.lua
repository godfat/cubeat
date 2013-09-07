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
local function save(demo, save_type, data)
  if save_record_[save_type] then
    save_record_[save_type](demo, data)
  end
end

return {
  save = save
}