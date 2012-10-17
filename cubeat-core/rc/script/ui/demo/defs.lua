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
Mt_Demo.init_vs_ppl  = C.Demo_init_vs_ppl
Mt_Demo.init_vs_cpu  = C.Demo_init_vs_cpu
Mt_Demo.init_cpudemo = C.Demo_init_cpudemo
Mt_Demo.init_ai_logging = C.Demo_init_ai_logging
Mt_Demo.init_puzzle  = C.Demo_init_puzzle
Mt_Demo.init_tutorial = C.Demo_init_tutorial
Mt_Demo.init_mode    = function(self, mode, c1p, c2p, sconf, ai_level)
  if mode == 0 then self:init_vs_ppl(c1p, c2p, sconf)
  elseif mode == 1 then self:init_vs_cpu(c1p, c2p, sconf, ai_level)
  elseif mode == 2 then self:init_cpudemo(c1p, c2p, sconf)
  elseif mode == 3 then self:init_ai_logging(c1p, c2p, sconf)
  elseif mode == 4 then self:init_tutorial(c1p, c2p, sconf)
  end
end
Mt_Demo.quit         = C.Demo_quit
Mt_Demo.get_ui_scene = function(self)
  return ffi.gc(C.Demo_get_ui_scene(self), C.Scene__gc)
end

ffi.metatype("Demo", Mt_Demo)
