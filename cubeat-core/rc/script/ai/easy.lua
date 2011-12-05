
local ffi = require 'ffi'
local C   = ffi.C

ffi.cdef[[
typedef struct pSimpleMap  pSimpleMap;
typedef struct pSimpleCube pSimpleCube;

pSimpleCube* find_keycube_for_highest_chain_power(pSimpleMap*, int, int);

void SimpleMap_print_data_for_debug(pSimpleMap*);
int  SimpleMap_warning_level(pSimpleMap*);
int  SimpleMap_garbage_left(pSimpleMap*);
int  SimpleMap_width(pSimpleMap*);
int  SimpleMap_height(pSimpleMap*);
void SimpleMap__gc(pSimpleMap*);
bool SimpleMap_cube_exist_at(pSimpleMap*, int, int);
int  SimpleMap_grounded_cube_count(pSimpleMap*);

pSimpleCube*  SimpleMap_get_cube(pSimpleMap*, int, int);
pSimpleCube*  SimpleMap_get_grounded_cube(pSimpleMap*, int, int);
pSimpleCube** SimpleMap_get_garbages(pSimpleMap*, unsigned int* size_out);
pSimpleCube** SimpleMap_get_brokens(pSimpleMap*, unsigned int* size_out);

bool SimpleCube_exist(pSimpleCube*);
bool SimpleCube_is_garbage(pSimpleCube*);
bool SimpleCube_is_broken(pSimpleCube*);
int  SimpleCube_x(pSimpleCube*);
int  SimpleCube_y(pSimpleCube*);
void SimpleCube__gc(pSimpleCube*);

void SimpleCubeList__gc(pSimpleCube** list, int size);
]]

local function find_keycube_for_highest_chain_power(map, lb, ub)
  return ffi.gc(C.find_keycube_for_highest_chain_power(map, lb, ub), C.SimpleCube__gc)
end

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

Mt_SimpleMap.get_garbages         = function(self, size_out) 
  return ffi.gc(C.SimpleMap_get_garbages(self, size_out), function(self)
    C.SimpleCubeList__gc(self, size_out[0])
  end)
end

Mt_SimpleMap.get_brokens          = function(self, size_out) 
  return ffi.gc(C.SimpleMap_get_brokens(self, size_out), function(self)
    C.SimpleCubeList__gc(self, size_out[0])
  end)
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
  
  local keycube = find_keycube_for_highest_chain_power(my_map, 2, 99)
  if keycube:exist() then
    io.write( string.format("keycube at: %d, %d\n", keycube:x(), keycube:y()) )
  else
    io.write "No keycube for now.\n"
  end
  
  local size = ffi.new("unsigned int[1]")
  local list = my_map:get_brokens(size)
  for i = 0, size[0]-1 do
    io.write( string.format("broken cube at: %d, %d\n", list[i]:x(), list[i]:y()) )
  end
  
  print(collectgarbage("count"))
  collectgarbage("collect")
end
