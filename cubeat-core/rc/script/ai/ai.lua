
local ffi      = require 'ffi'
local helper   = require 'rc/script/helper'
local basepath = helper.basepath
local C        = ffi.C

ffi.cdef[[
typedef struct AIPlayer AIPlayer;
typedef struct pSimpleMap pSimpleMap;
typedef struct pSimpleCube pSimpleCube;
]]
ffi.cdef( io.open( basepath().."rc/script/ai/bindings.ffi", 'r'):read('*a') )

local Mt_AIPlayer = {}
Mt_AIPlayer.__index       = Mt_AIPlayer
Mt_AIPlayer.push_command  = C.AIPlayer_push_command
Mt_AIPlayer.cmdqueue_size = C.AIPlayer_cmdqueue_size

Mt_AIPlayer.get_ally_map = function(self, index)
  return ffi.gc(C.AIPlayer_get_ally_map(self, index), C.SimpleMap__gc)
end

Mt_AIPlayer.get_enemy_map = function(self, index)
  return ffi.gc(C.AIPlayer_get_enemy_map(self, index), C.SimpleMap__gc)
end

ffi.metatype("AIPlayer", Mt_AIPlayer)

local Mt_SimpleMap = {}
Mt_SimpleMap.__index = Mt_SimpleMap
Mt_SimpleMap.print_data_for_debug = C.SimpleMap_print_data_for_debug
Mt_SimpleMap.warning_level        = C.SimpleMap_warning_level
Mt_SimpleMap.garbage_left         = C.SimpleMap_garbage_left
Mt_SimpleMap.width                = C.SimpleMap_width
Mt_SimpleMap.height               = C.SimpleMap_height
Mt_SimpleMap.cube_exist_at        = C.SimpleMap_cube_exist_at
Mt_SimpleMap.grounded_cube_count  = C.SimpleMap_grounded_cube_count

Mt_SimpleMap.get_cube             = function(self, x, y) 
  return ffi.gc(C.SimpleMap_get_cube(self, x, y), C.SimpleCube__gc)
end

Mt_SimpleMap.get_grounded_cube    = function(self, x, y) 
  return ffi.gc(C.SimpleMap_get_grounded_cube(self, x, y), C.SimpleCube__gc)
end

Mt_SimpleMap.get_firepoint_cube   = function(map, lb, ub)
  return ffi.gc(C.SimpleMap_get_firepoint_cube(map, lb, ub), C.SimpleCube__gc)
end

Mt_SimpleMap.get_garbages         = function(self) 
  local size_out = ffi.new("unsigned int[1]")
  return ffi.gc(C.SimpleMap_get_garbages(self, size_out), function(self)
    C.SimpleCubeList__gc(self, size_out[0])
  end), size_out[0]
end

Mt_SimpleMap.get_brokens          = function(self)
  local size_out = ffi.new("unsigned int[1]") 
  return ffi.gc(C.SimpleMap_get_brokens(self, size_out), function(self)
    C.SimpleCubeList__gc(self, size_out[0])
  end), size_out[0]
end

Mt_SimpleMap.get_highcols         = function(self, threshold)
  local cols = ffi.new("int[?]", self:width())
  local size = 0
  for x = 0, self:width() - 1 do
    for y = 0, self:height() - 1 do 
      if not self:get_cube(x, y):exist() then
        if y >= threshold then cols[size] = x; size = size + 1 end 
        break
      end
    end
  end
  return cols, size
end

local Mt_SimpleCube = {}
Mt_SimpleCube.__index    = Mt_SimpleCube
Mt_SimpleCube.exist      = C.SimpleCube_exist
Mt_SimpleCube.is_garbage = C.SimpleCube_is_garbage
Mt_SimpleCube.is_broken  = C.SimpleCube_is_broken
Mt_SimpleCube.x          = C.SimpleCube_x
Mt_SimpleCube.y          = C.SimpleCube_y

ffi.metatype("pSimpleMap", Mt_SimpleMap)
ffi.metatype("pSimpleCube", Mt_SimpleCube)
