local ffi   = require 'ffi'
local C    = ffi.C
local view = require 'rc/script/ui/view'


--[[

[ch]
1 = left actor
2 = right actor

[effect_a]
show
hide
slide_in
slide_out
fade_in
fade_out
shake

[effect_w]
size_L
size_M
size_S
shake

--]]


--
local act1_act1_ = {
  { ch=1, img="char1_new/glad", text="中文" },
  { ch=1, text="這是中文測試" },
  { ch=1, text="END" },
  
  { ch=1, text="" }
}

local act2_act1_ = {
  { ch=1, img="char1_new/glad", text="中文" },
  { ch=1, text="這是中文測試" },
  { ch=1, text="END" },
  
  { ch=1, text="" }
}


local rundown_ = {
  r1_1 = act1_act1_,
  r2_1 = act2_act1_,
}


local function get_rundown(ch1, ch2)
  local k = 'r'..tostring(ch1)..'_'..tostring(ch2)
  
  return rundown_[k]
end


return {
  get_rundown = get_rundown
}