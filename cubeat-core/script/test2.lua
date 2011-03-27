
local MapUtils = require 'script/puzzle/maputils'
local PuzzleGen = require 'script/puzzle/puzzle_gen'

MapUtils.display( PuzzleGen:generate(19, 6, 10, true) )
