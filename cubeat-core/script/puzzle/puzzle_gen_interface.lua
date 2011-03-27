
local MapUtils = require 'script/puzzle/maputils'
local PuzzleGen = require 'script/puzzle/puzzle_gen'

function generate_to_file(chain_limit, w, h, de_bug)
  MapUtils.display( PuzzleGen:generate(chain_limit, w, h, de_bug) )
end

