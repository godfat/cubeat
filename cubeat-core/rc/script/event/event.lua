local ffi       = require 'ffi'
local C         = ffi.C
local helper    = require 'rc/script/helper'
local basepath  = helper.basepath

ffi.cdef[[
typedef struct pHandle pHandle;
]]
ffi.cdef( io.open( basepath().."rc/script/event/bindings.ffi", 'r'):read('*a') )

----------------------------------------------------------------------------
-- FFI callback hackery
----------------------------------------------------------------------------

local Mt_Handle = {}
Mt_Handle.__index = Mt_Handle
Mt_Handle.__gc    = C.Handle__gc
Mt_Handle.remove  = function(self) C.Handle__gc(self); self = nil end -- make it explicit if needed

ffi.metatype("pHandle", Mt_Handle)

local event = {}

event.get_time_of = function(timer_name)
  timer_name = timer_name or "global"
  return C.Event_get_time_of(timer_name)
end

event.on_timer = function(timer_name, callback, dur, loop)
  loop = loop or 0
  return C.Event_on_timer(timer_name, callback, dur, loop)
end

return event
