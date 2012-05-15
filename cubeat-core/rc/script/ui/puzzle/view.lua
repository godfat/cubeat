local ffi       = require 'ffi'
local C         = ffi.C
local helper    = require 'rc/script/helper'
local basepath  = helper.basepath

ffi.cdef[[
typedef struct Puzzle Puzzle;
]]
ffi.cdef( io.open( basepath().."rc/script/ui/puzzle/bindings.ffi", 'r'):read('*a') )

----------------------------------------------------------------------------
-- "Class" definitions
----------------------------------------------------------------------------

-- Puzzle
local Mt_Puzzle = {}
Mt_Puzzle.__index       = Mt_Puzzle
Mt_Puzzle.get_ui_scene  = function(self)
                            return ffi.gc(C.get_puzzle_scene(self), C.Scene__gc)
                          end

ffi.metatype("Puzzle", Mt_Puzzle)