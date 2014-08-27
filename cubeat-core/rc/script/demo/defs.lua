local ffi      = require 'ffi'
local C        = ffi.C
local helper   = require 'rc/script/helper'
local basepath = helper.basepath

require 'rc/script/ui/view'

ffi.cdef[[
typedef struct Demo Demo;
typedef struct AIPlayer AIPlayer;
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
Mt_Demo.init_story             = C.Demo_init_story
Mt_Demo.init_tutorial          = C.Demo_init_tutorial
Mt_Demo.init_cpudemo           = C.Demo_init_cpudemo
Mt_Demo.init_ai_logging        = C.Demo_init_ai_logging
Mt_Demo.init_single            = C.Demo_init_single
Mt_Demo.init_map_starting_line = C.Demo_init_map_starting_line
Mt_Demo.init_map_with_config   = C.Demo_init_map_with_config
Mt_Demo.reset_map_record       = C.Demo_reset_map_record
Mt_Demo.hide_map_warning       = C.Demo_hide_map_warning
Mt_Demo.set_map_garbage_amount = C.Demo_set_map_garbage_amount
Mt_Demo.set_map_dropping       = C.Demo_set_map_dropping
Mt_Demo.set_only_one_shot_for_puzzle = C.Demo_set_only_one_shot_for_puzzle
Mt_Demo.set_stage_name         = C.Demo_set_stage_name
Mt_Demo.set_countdown          = C.Demo_set_countdown
Mt_Demo.set_time               = C.Demo_set_time
Mt_Demo.play_sound             = C.Demo_play_sound
Mt_Demo.hide_character_animations = C.Demo_hide_character_animations
Mt_Demo.get_time               = C.Demo_get_time
Mt_Demo.get_map_score          = C.Demo_get_map_score
Mt_Demo.get_map_highest_chain  = C.Demo_get_map_highest_chain
Mt_Demo.get_map_garbage_left   = C.Demo_get_map_garbage_left
Mt_Demo.get_map_warning_level  = C.Demo_get_map_warning_level
Mt_Demo.get_map_broken_num     = C.Demo_get_map_broken_num
Mt_Demo.get_map_cubes_cleared_data = C.Demo_get_map_cubes_cleared_data
Mt_Demo.get_map_cubes_matched_data = C.Demo_get_map_cubes_matched_data
Mt_Demo.get_ai_player          = C.Demo_get_ai_player
Mt_Demo.is_map_all_waiting     = C.Demo_is_map_all_waiting
Mt_Demo.is_map_empty           = C.Demo_is_map_empty
Mt_Demo.is_puzzle_started      = C.Demo_is_puzzle_started
Mt_Demo.reinit                 = C.Demo_reinit
Mt_Demo.endgame                = C.Demo_endgame
Mt_Demo.eventual_pause         = C.Demo_eventual_pause
Mt_Demo.eventual_resume        = C.Demo_eventual_resume
Mt_Demo.leave_and_cleanup      = C.Demo_leave_and_cleanup
Mt_Demo.load_stage             = C.Demo_load_stage
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
Mt_Demo.get_game_scene = function(self)
  return ffi.gc(C.Demo_get_game_scene(self), C.Scene__gc)
end  

ffi.metatype("Demo", Mt_Demo)
