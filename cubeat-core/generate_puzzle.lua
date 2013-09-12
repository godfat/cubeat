local generate = require 'rc/script/puzzle/puzzle_gen_interface'

local function string_to_boolean(v)
  return (type(v)=="string" and v=="true")
end

local w_            = 6
local h_            = 11
local chain_limit_  = tonumber(arg[1])
local color_num_    = tonumber(arg[2])
local de_bug_       = string_to_boolean(arg[3])
local outfile_      = arg[4]

generate.gen(chain_limit_, w_, h_, de_bug_, color_num_, outfile_)