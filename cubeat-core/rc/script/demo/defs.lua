local ffi      = require 'ffi'
local C        = ffi.C
local helper   = require 'rc/script/helper'
local basepath = helper.basepath

require 'rc/script/ui/view'

ffi.cdef[[
typedef struct Demo Demo;
]]
ffi.cdef( io.open( basepath().."rc/script/demo/bindings.ffi", 'r'):read('*a') )

----------------------------------------------------------------------------
-- "Class" definitions
----------------------------------------------------------------------------

-- Demo
local Mt_Demo = {}
Mt_Demo.__index                = Mt_Demo
Mt_Demo.init_vs_ppl            = C.Demo_init_vs_ppl
Mt_Demo.init_vs_cpu            = C.Demo_init_vs_cpu
Mt_Demo.init_cpudemo           = C.Demo_init_cpudemo
Mt_Demo.init_ai_logging        = C.Demo_init_ai_logging
Mt_Demo.init_single            = C.Demo_init_single
Mt_Demo.init_map_starting_line = C.Demo_init_map_starting_line
Mt_Demo.set_map_garbage_amount = C.Demo_set_map_garbage_amount
Mt_Demo.set_only_one_shot_for_puzzle = C.Demo_set_only_one_shot_for_puzzle
Mt_Demo.get_time               = C.Demo_get_time
Mt_Demo.get_map_score          = C.Demo_get_map_score
Mt_Demo.get_map_highest_chain  = C.Demo_get_map_highest_chain
Mt_Demo.get_map_garbage_left   = C.Demo_get_map_garbage_left
Mt_Demo.get_map_warning_level  = C.Demo_get_map_warning_level
Mt_Demo.get_map_cubes_cleared_data = C.Demo_get_map_cubes_cleared_data
Mt_Demo.is_map_all_waiting     = C.Demo_is_map_all_waiting
Mt_Demo.is_map_empty           = C.Demo_is_map_empty
Mt_Demo.is_puzzle_started      = C.Demo_is_puzzle_started
Mt_Demo.reinit                 = C.Demo_reinit
Mt_Demo.endgame                = C.Demo_endgame
Mt_Demo.eventual_pause         = C.Demo_eventual_pause
Mt_Demo.eventual_resume        = C.Demo_eventual_resume
Mt_Demo.leave_and_cleanup      = C.Demo_leave_and_cleanup
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
