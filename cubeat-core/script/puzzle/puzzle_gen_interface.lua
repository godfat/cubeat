
local MapUtils = require 'script/puzzle/maputils'
local PuzzleGen = require 'script/puzzle/puzzle_gen'

function generate_to_file(chain_limit, w, h, de_bug)
  file = io.open("tmp/puzzle.zzml", "w")
  file:write([[
level:4,
color_amounts:4,
chain_amounts:3,
starting_line:4,
chaining_duration:300,
cube_dying_duration:300,
cube_sinking_duration:500,
cube_dropping_duration:150,
speed:80.0,
damage_factor:1.0,
width:6,
height:10,
dropping_creatable:0,
garbage_dumpable:0,
cube_colors:
]])
  local map = PuzzleGen:generate(chain_limit, w, h, de_bug)
  file:write("[\n")
  for y = map.height, 1, -1 do
    file:write("[")
    for x = 1, map.width do
      if x == map.width then
        file:write(string.format("%d", map[y][x]))
      else
        file:write(string.format("%d, ", map[y][x]))
      end
    end
    if y == 1 then file:write("]\n")
    else file:write("],\n") end
  end
  file:write("]\n")
  file:close()
end

