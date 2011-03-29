
local random = require 'script/puzzle/helpers'.random
local reverse_i = require 'script/puzzle/helpers'.reverse_i
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
cube_dying_duration:200,
cube_sinking_duration:500,
cube_dropping_duration:300,
speed:80.0,
damage_factor:1.0,
width:6,
height:11,
dropping_creatable:0,
garbage_dumpable:0,
cube_colors:
]])
  local map = PuzzleGen:generate(chain_limit, w, h, de_bug)
  file:write("[\n")

  file:write("[")
  for x = 1, map.width - 1 do file:write("0, ") end -- important, the real height must be one row higher than logic map
  file:write("0],\n")

  if random(2) == 0 then
    if de_bug then print(" -- puzzle flipped (50% chance) -- ") end
    for y = 1, map.height do reverse_i(map[y]) end
  end

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

