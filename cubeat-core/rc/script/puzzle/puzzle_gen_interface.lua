
local random = require 'rc/script/puzzle/helpers'.random
local reverse_i = require 'rc/script/puzzle/helpers'.reverse_i
local MapUtils = require 'rc/script/puzzle/maputils'
local PuzzleGen = require 'rc/script/puzzle/puzzle_gen'

function generate_to_file(chain_limit, w, h, de_bug)

  local ffi = require 'ffi'
  ffi.cdef[[
    char* _getcwd(char * buf, size_t size);
    char*  getcwd(char * buf, size_t size);
  ]]
  local getcwd;
  if jit.os == 'Windows' then
    getcwd = function(buf, size) return ffi.string( ffi.C._getcwd(buf, size) ) end
  else
    getcwd = function(buf, size) return ffi.string( ffi.C.getcwd(buf, size) ) end
  end
  local buf  = ffi.new("char[256]")
  local path = getcwd(buf, 256) -- get working directory
  path = path.."/"
  if de_bug then print("Debug: getcwd = "..path) end

  file = io.open( path.."rc/config/tmp/puzzle.zzml", "w")
  file:write([[
level:4,
color_amounts:4,
chain_amounts:3,
starting_line:4,
chaining_duration:300,
cube_dying_duration:200,
cube_sinking_duration:500,
cube_dropping_duration:300,
sink_speed:80.0,
drop_speed:450.0,
damage_factor:1.0,
width:6,
height:11,
dropping_creatable:0,
garbage_dumpable:0,
cube_colors:
]])
  local map, ans = PuzzleGen:generate(chain_limit, w, h, de_bug)
  file:write("[\n")

  file:write("[")
  for x = 1, map.width - 1 do file:write("0, ") end -- important, the real height must be one row higher than logic map
  file:write("0],\n")

  local flipped = false
  if random(2) == 0 then
    flipped = true
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

  if de_bug then
    if ans     then file:write("# note: "..ans.."\n") end
    if flipped then file:write("# note: answer flipped.\n") end
  end

  file:close()
end

