local ffi      = require 'ffi'
local C        = ffi.C
local helper   = require 'rc/script/helper'
local basepath = helper.basepath

require 'rc/script/ui/view'

ffi.cdef[[
typedef struct Demo Demo;
]]
ffi.cdef( io.open( basepath().."rc/script/ui/demo/bindings.ffi", 'r'):read('*a') )

----------------------------------------------------------------------------
-- "Class" definitions
----------------------------------------------------------------------------

-- Demo
local Mt_Demo = {}
Mt_Demo.__index      = Mt_Demo
Mt_Demo.get_ui_scene = function(self)
  return ffi.gc(C.Demo_get_ui_scene(self), C.Scene__gc)
end

ffi.metatype("Demo", Mt_Demo)
