local generate = require 'rc/script/puzzle/puzzle_gen_interface'

local function string_to_boolean(v)
  return (type(v)=="string" and v=="true")
end

local w_            = 6
local h_            = 11
local chain_limit_  = tonumber(arg[1])
local color_num_    = tonumber(arg[2])
local num_          = tonumber(arg[3]) if num_>10 then num_ = 10 end
local de_bug_       = string_to_boolean(arg[4])
local outfile_      = arg[5] or "rc/config/tmp/puzzle"

for i=1,num_ do
  filepath = outfile_ .. tostring(i) .. '.zzml'
  generate.gen(chain_limit_, w_, h_, de_bug_, color_num_, filepath)
end