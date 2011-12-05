
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

bool          SimpleMap_exist(pSimpleMap*, int, int);
pSimpleCube*  SimpleMap_get_cube(pSimpleMap*, int, int);
pSimpleCube*  SimpleMap_get_grounded_cube(pSimpleMap*, int, int);
pSimpleCube** SimpleMap_find_garbages(pSimpleMap*);
pSimpleCube** SimpleMap_find_garbages(pSimpleMap*);
int           SimpleMap_grounded_cube_count(pSimpleMap*);

bool SimpleCube_exist(pSimpleCube*);
bool SimpleCube_is_garbage(pSimpleCube*);
bool SimpleCube_is_broken(pSimpleCube*);
int  SimpleCube_x(pSimpleCube*);
int  SimpleCube_y(pSimpleCube*);
void SimpleCube__gc(pSimpleCube*);
]]

local Mt_SimpleMap = {}
Mt_SimpleMap.__index = Mt_SimpleMap
Mt_SimpleMap.print_data_for_debug = C.SimpleMap_print_data_for_debug
Mt_SimpleMap.warning_level        = C.SimpleMap_warning_level
Mt_SimpleMap.garbage_left         = C.SimpleMap_garbage_left
Mt_SimpleMap.width                = C.SimpleMap_width
Mt_SimpleMap.height               = C.SimpleMap_height
Mt_SimpleMap.is_below_empty       = C.SimpleMap_is_below_empty
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
  
  my_map:print_data_for_debug()
  print(collectgarbage("count"))
  collectgarbage("collect")
end
