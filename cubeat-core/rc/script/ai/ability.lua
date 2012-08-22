
local C = require 'ffi'.C
require 'rc/script/ai/ai'

local ability_preconditions_ = {
  
  function(self, my_map, enemy_map) -- #1 NOTE: ability #1's passive mods have to be considered in AI strategy as well
    local capacity = my_map:width() * (my_map:height()-1)
    local ground_cube_num = my_map:grounded_cube_count()
    local _, broken_num   = my_map:get_brokens()
    local _, garbage_num  = my_map:get_garbages()
    
    if ground_cube_num > capacity * 0.9 and broken_num+garbage_num > capacity * 0.3 then return true end
    if ground_cube_num > capacity * 0.6 and broken_num+garbage_num > capacity * 0.45 then return true end
    return false
  end,
  
  function(self, my_map, enemy_map) -- #2
    local capacity = my_map:width() * (my_map:height()-1)
    local ground_cube_num = my_map:grounded_cube_count()
    local garbage_left    = my_map:garbage_left()
    
    if garbage_left + ground_cube_num >= capacity then return true end 
    return false
  end,
  
  function(self, my_map, enemy_map) -- #3
    local capacity = my_map:width() * (my_map:height()-1)
    if my_map:garbage_left() > capacity * 0.5 then return true end
    return false
  end,
  
  function(self, my_map, enemy_map) -- #4
    local capacity = my_map:width() * (my_map:height()-1)
    local ground_cube_num = my_map:grounded_cube_count()
    local _, broken_num   = my_map:get_brokens()
    local _, garbage_num  = my_map:get_garbages()
    
    if C.psc_get_game_time() < 60000 then return false end -- don't use it in the first minute.
    if ground_cube_num > capacity * 0.9 and broken_num+garbage_num < ground_cube_num * 0.4 then return true end
    if ground_cube_num > capacity * 0.6 and broken_num+garbage_num < ground_cube_num * 0.2 then return true end
    return false
  end,
  
  function(self, my_map, enemy_map) -- #5
    local capacity = my_map:width() * (my_map:height()-1)
    local ground_cube_num = my_map:grounded_cube_count()
    local _, broken_num   = my_map:get_brokens()
    local _, garbage_num  = my_map:get_garbages()
    
    if ground_cube_num > capacity * 0.9 and broken_num+garbage_num > capacity * 0.45 then return true end
    return false
  end,
  
  function(self, my_map, enemy_map) -- #6
    local my_capacity = my_map:width() * (my_map:height()-1)
    local en_capacity = enemy_map:width() * (enemy_map:height()-1)
    local my_ground_cube_num = my_map:grounded_cube_count()
    local en_ground_cube_num = enemy_map:grounded_cube_count()
    local _, broken_num   = my_map:get_brokens()
    local _, garbage_num  = my_map:get_garbages()
    
    if my_ground_cube_num > my_capacity * 0.9 and broken_num+garbage_num > my_capacity * 0.4 then return true end
    if my_ground_cube_num > my_capacity * 0.6 and 
       broken_num+garbage_num > my_ground_cube_num * 0.6 and 
       en_ground_cube_num < en_capacity * 0.7 then return true end 
       -- it is pretty important that we don't transfer cubes to enemy when they are already too full.
    return false
  end,
  
  function(self, my_map, enemy_map) -- #7
    -- write it later
  end,
  
  function(self, my_map, enemy_map) -- #8
    -- write it later
  end
}

return ability_preconditions_
