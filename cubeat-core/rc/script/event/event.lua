local ffi       = require 'ffi'
local C         = ffi.C
local helper    = require 'rc/script/helper'
local basepath  = helper.basepath
local cdata_addr= helper.cdata_addr

ffi.cdef[[
typedef struct pHandle pHandle;
]]
ffi.cdef( io.open( basepath().."rc/script/event/bindings.ffi", 'r'):read('*a') )

----------------------------------------------------------------------------
-- FFI callback hackery
----------------------------------------------------------------------------

local __tracked_timer__ = {}
local CallbackT = ffi.typeof("PSC_CALLBACK")

local function tracked_timer(func, h)
  if __tracked_timer__[ cdata_addr(h) ] == nil then
    __tracked_timer__[ cdata_addr(h) ] = ffi.cast(CallbackT, func)
  else
    __tracked_timer__[ cdata_addr(h) ]:set(func)
  end
  return __tracked_timer__[ cdata_addr(h) ]
end

local function handle_dtor(h)
  if __tracked_timer__[ cdata_addr(h) ] ~= nil then
    __tracked_timer__[ cdata_addr(h) ]:free()
    __tracked_timer__[ cdata_addr(h) ] = nil
    C.Handle__gc(h)
  end
end

-- Mt_Handle ==================

local Mt_Handle = {}
Mt_Handle.__index = Mt_Handle
Mt_Handle.__gc    = handle_dtor
Mt_Handle.remove  = handle_dtor -- make it explicit if needed

ffi.metatype("pHandle", Mt_Handle)

--=============================

local event = {}

event.get_time_of = function(timer_name)
  timer_name = timer_name or "global"
  return C.Event_get_time_of(timer_name)
end

event.clear_timers_of = function(timer_name)
  timer_name = timer_name or "global"
  C.Event_clear_timers_of(timer_name)
end

event.on_timer = function(timer_name, callback, dur, loop)
  loop = loop or 0
  local handle = C.Handle_create()
  callback = tracked_timer(callback, handle)
  C.Event_on_timer(timer_name, callback, handle, dur, loop)
  return handle
end

return event
