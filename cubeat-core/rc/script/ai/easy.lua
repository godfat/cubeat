
local ffi      = require 'ffi'
local basepath = require 'rc/script/helper'.basepath
local C        = ffi.C
ffi.cdef[[
typedef struct pSimpleMap pSimpleMap;
typedef struct pSimpleCube pSimpleCube;
]]
ffi.cdef( io.open( basepath().."rc/script/ai/lua_AI_bindings.ffi", 'r'):read('*a') )

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

ffi.metatype("pSimpleMap", Mt_SimpleMap)

local Mt_SimpleCube = {}
Mt_SimpleCube.__index = Mt_SimpleCube
Mt_SimpleCube.exist      = C.SimpleCube_exist
Mt_SimpleCube.is_garbage = C.SimpleCube_is_garbage
Mt_SimpleCube.is_broken  = C.SimpleCube_is_broken
Mt_SimpleCube.x          = C.SimpleCube_x
Mt_SimpleCube.y          = C.SimpleCube_y
ffi.metatype("pSimpleCube", Mt_SimpleCube)

function ai_entry(my_map, enemy_map)
  my_map    = ffi.gc(ffi.cast("pSimpleMap*", my_map),    C.SimpleMap__gc) 
  enemy_map = ffi.gc(ffi.cast("pSimpleMap*", enemy_map), C.SimpleMap__gc) 
  
  local keycube = my_map:get_firepoint_cube(2, 99)
  if keycube:exist() then
    io.write( string.format("keycube at: %d, %d\n", keycube:x(), keycube:y()) )
  else
    io.write "No keycube for now.\n"
  end
  
  local list, size = my_map:get_brokens()
  for i = 0, size-1 do
    io.write( string.format("broken cube at: %d, %d\n", list[i]:x(), list[i]:y()) )
  end
  
  print(collectgarbage("count"))
  collectgarbage("collect")
end
