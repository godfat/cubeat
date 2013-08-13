local event     = require 'rc/script/event/event'
local parameter = require 'rc/script/ui/demo/challengemenu/parameter'

local challenge_start_, challenge_callend_, challenge_end_ = 1,2,3
local status_ = nil
local handle_ = nil


local function handle_remove()
  if handle_ then
    handle_:remove()
    handle_ = nil
    print("----------- handle remove -----------")
  end
end

local function setup(demo, in_place, submode)
  -- OneShotClear
  if submode == parameter.OneShotClear then
    --print(demo:get_map_score(1)) 
    --print(demo:get_map_highest_chain(1))
    --print(demo:get_map_cubes_cleared_data(1)[1])
  end
  
  -- HighestChain
  if submode == parameter.HighestChain then
    status_ = challenge_start_
    
    handle_ = event.on_timer("global", function()
      if status_ == challenge_start_ then
        local chain = demo:get_map_highest_chain(parameter.player1)
        print("highest chain: " .. tostring(chain))
        if chain>2 or chain==2 then
          if status_ ~= challenge_end_ then status_ = challenge_callend_ end
        end
      end
      
      if status_ == challenge_callend_ then
        demo:endgame(parameter.player2)
        status_ = challenge_end_
      end
    end, 1000, -1 )
  end
  
  -- WarningCondition
  if submode == parameter.WarningCondition then
  end
  
  -- TimeLimit
  if submode == parameter.TimeLimit then
  end
end

return {
  handle_remove = handle_remove,
  setup         = setup
}