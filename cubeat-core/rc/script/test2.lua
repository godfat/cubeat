
local MapUtils = require 'rc/script/puzzle/maputils'
local PuzzleGen = require 'rc/script/puzzle/puzzle_gen'

MapUtils.display( PuzzleGen:generate(19, 6, 10, true) )
