
local ffi = require 'ffi'
local C   = ffi.C

ffi.cdef[[
typedef struct pSimpleMap pSimpleMap;

void SimpleMap_print_data_for_debug(pSimpleMap*);
int  SimpleMap_warning_level(pSimpleMap*);
int  SimpleMap_garbage_left(pSimpleMap*);
int  SimpleMap_width(pSimpleMap*);
int  SimpleMap_height(pSimpleMap*);


void SimpleMap__gc(pSimpleMap*);
]]

local mt = {}
mt.__index = mt
mt.print_data_for_debug = C.SimpleMap_print_data_for_debug
ffi.metatype("pSimpleMap", mt)

function ai_entry(my_map, enemy_map)
  my_map    = ffi.gc(ffi.cast("pSimpleMap*", my_map),    C.SimpleMap__gc) 
  enemy_map = ffi.gc(ffi.cast("pSimpleMap*", enemy_map), C.SimpleMap__gc) 
  
  my_map:print_data_for_debug()
  print(collectgarbage("count"))
  collectgarbage("collect")
end
