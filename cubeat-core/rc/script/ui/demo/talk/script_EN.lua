local ffi   = require 'ffi'
local C    = ffi.C
local view = require 'rc/script/ui/view'


--[[
[pos]
change text & panel position
example=> pos={x=700, y=300}

[board]
change panel texture
example=> board="area_rect"

[board_flip]
"H"  = flip panel Horizon
"V"  = flip panel Vertical
"HV" = flip panel Horizon+Vertical

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

[special]
cube: x1,y1,x2,y2,x3,y3,path,w,h,dur

--]]


--
local act1_act1_ = {
  { ch=1, img="char1_new/glad", text="Let's go" },
  { ch=2, img="char1_new/glad", text="Go!" },
  
  { ch=1, text="" }
}
local act1_act2_ = {
  { ch=1, img="char1_new/glad", text="Let's go" },
  { ch=2, img="char2_new/glad", text="Go!" },
  
  { ch=1, text="" }
}
local act1_act3_ = {
  { ch=1, img="char1_new/glad", text="Let's go" },
  { ch=2, img="char3_new/glad", text="Go!" },
  
  { ch=1, text="" }
}
local act1_act4_ = {
  { ch=1, img="char1_new/glad", text="Let's go" },
  { ch=2, img="char4_new/glad", text="Go!" },
  
  { ch=1, text="" }
}
local act1_act5_ = {
  { ch=1, img="char1_new/glad", text="Let's go" },
  { ch=2, img="char5_new/glad", text="Go!" },
  
  { ch=1, text="" }
}
local act1_act6_ = {
  { ch=1, img="char1_new/glad", text="Let's go" },
  { ch=2, img="char6_new/glad", text="Go!" },
  
  { ch=1, text="" }
}

local act2_act1_ = {
  { ch=1, img="char2_new/glad", text="Let's go" },
  { ch=2, img="char1_new/glad", text="Go!" },
  
  { ch=1, text="" }
}
local act2_act2_ = {
  { ch=1, img="char2_new/glad", text="Let's go" },
  { ch=2, img="char2_new/glad", text="Go!" },
  
  { ch=1, text="" }
}
local act2_act3_ = {
  { ch=1, img="char2_new/glad", text="Let's go" },
  { ch=2, img="char3_new/glad", text="Go!" },
  
  { ch=1, text="" }
}
local act2_act4_ = {
  { ch=1, img="char2_new/glad", text="Let's go" },
  { ch=2, img="char4_new/glad", text="Go!" },
  
  { ch=1, text="" }
}
local act2_act5_ = {
  { ch=1, img="char2_new/glad", text="Let's go" },
  { ch=2, img="char5_new/glad", text="Go!" },
  
  { ch=1, text="" }
}
local act2_act6_ = {
  { ch=1, img="char2_new/glad", text="Let's go" },
  { ch=2, img="char6_new/glad", text="Go!" },
  
  { ch=1, text="" }
}

local act3_act1_ = {
  { ch=1, img="char3_new/glad", text="Let's go" },
  { ch=2, img="char1_new/glad", text="Go!" },
  
  { ch=1, text="" }
}
local act3_act2_ = {
  { ch=1, img="char3_new/glad", text="Let's go" },
  { ch=2, img="char2_new/glad", text="Go!" },
  
  { ch=1, text="" }
}
local act3_act3_ = {
  { ch=1, img="char3_new/glad", text="Let's go" },
  { ch=2, img="char3_new/glad", text="Go!" },
  
  { ch=1, text="" }
}
local act3_act4_ = {
  { ch=1, img="char3_new/glad", text="Let's go" },
  { ch=2, img="char4_new/glad", text="Go!" },
  
  { ch=1, text="" }
}
local act3_act5_ = {
  { ch=1, img="char3_new/glad", text="Let's go" },
  { ch=2, img="char5_new/glad", text="Go!" },
  
  { ch=1, text="" }
}
local act3_act6_ = {
  { ch=1, img="char3_new/glad", text="Let's go" },
  { ch=2, img="char6_new/glad", text="Go!" },
  
  { ch=1, text="" }
}

local act4_act1_ = {
  { ch=1, img="char4_new/glad", text="Let's go" },
  { ch=2, img="char1_new/glad", text="Go!" },
  
  { ch=1, text="" }
}
local act4_act2_ = {
  { ch=1, img="char4_new/glad", text="Let's go" },
  { ch=2, img="char2_new/glad", text="Go!" },
  
  { ch=1, text="" }
}
local act4_act3_ = {
  { ch=1, img="char4_new/glad", text="Let's go" },
  { ch=2, img="char3_new/glad", text="Go!" },
  
  { ch=1, text="" }
}
local act4_act4_ = {
  { ch=1, img="char4_new/glad", text="Let's go" },
  { ch=2, img="char4_new/glad", text="Go!" },
  
  { ch=1, text="" }
}
local act4_act5_ = {
  { ch=1, img="char4_new/glad", text="Let's go" },
  { ch=2, img="char5_new/glad", text="Go!" },
  
  { ch=1, text="" }
}
local act4_act6_ = {
  { ch=1, img="char4_new/glad", text="Let's go" },
  { ch=2, img="char6_new/glad", text="Go!" },
  
  { ch=1, text="" }
}

local act5_act1_ = {
  { ch=1, img="char5_new/glad", text="Let's go" },
  { ch=2, img="char1_new/glad", text="Go!" },
  
  { ch=1, text="" }
}
local act5_act2_ = {
  { ch=1, img="char5_new/glad", text="Let's go" },
  { ch=2, img="char2_new/glad", text="Go!" },
  
  { ch=1, text="" }
}
local act5_act3_ = {
  { ch=1, img="char5_new/glad", text="Let's go" },
  { ch=2, img="char3_new/glad", text="Go!" },
  
  { ch=1, text="" }
}
local act5_act4_ = {
  { ch=1, img="char5_new/glad", text="Let's go" },
  { ch=2, img="char4_new/glad", text="Go!" },
  
  { ch=1, text="" }
}
local act5_act5_ = {
  { ch=1, img="char5_new/glad", text="Let's go" },
  { ch=2, img="char5_new/glad", text="Go!" },
  
  { ch=1, text="" }
}
local act5_act6_ = {
  { ch=1, img="char5_new/glad", text="Let's go" },
  { ch=2, img="char6_new/glad", text="Go!" },
  
  { ch=1, text="" }
}

local act6_act1_ = {
  { ch=1, img="char6_new/glad", text="Let's go" },
  { ch=2, img="char1_new/glad", text="Go!" },
  
  { ch=1, text="" }
}
local act6_act2_ = {
  { ch=1, img="char6_new/glad", text="Let's go" },
  { ch=2, img="char2_new/glad", text="Go!" },
  
  { ch=1, text="" }
}
local act6_act3_ = {
  { ch=1, img="char6_new/glad", text="Let's go" },
  { ch=2, img="char3_new/glad", text="Go!" },
  
  { ch=1, text="" }
}
local act6_act4_ = {
  { ch=1, img="char6_new/glad", text="Let's go" },
  { ch=2, img="char4_new/glad", text="Go!" },
  
  { ch=1, text="" }
}
local act6_act5_ = {
  { ch=1, img="char6_new/glad", text="Let's go" },
  { ch=2, img="char5_new/glad", text="Go!" },
  
  { ch=1, text="" }
}
local act6_act6_ = {
  { ch=1, img="char6_new/glad", text="Let's go" },
  { ch=2, img="char6_new/glad", text="Go!" },
  
  { ch=1, text="" }
}

local rundown_ = {
  r1_1 = act1_act1_,
  r1_2 = act1_act2_,
  r1_3 = act1_act3_,
  r1_4 = act1_act4_,
  r1_5 = act1_act5_,
  r1_6 = act1_act6_,
  
  r2_1 = act2_act1_,
  r2_2 = act2_act2_,
  r2_3 = act2_act3_,
  r2_4 = act2_act4_,
  r2_5 = act2_act5_,
  r2_6 = act2_act6_,
  
  r3_1 = act3_act1_,
  r3_2 = act3_act2_,
  r3_3 = act3_act3_,
  r3_4 = act3_act4_,
  r3_5 = act3_act5_,
  r3_6 = act3_act6_,
  
  r4_1 = act4_act1_,
  r4_2 = act4_act2_,
  r4_3 = act4_act3_,
  r4_4 = act4_act4_,
  r4_5 = act4_act5_,
  r4_6 = act4_act6_,
  
  r5_1 = act5_act1_,
  r5_2 = act5_act2_,
  r5_3 = act5_act3_,
  r5_4 = act5_act4_,
  r5_5 = act5_act5_,
  r5_6 = act5_act6_,
  
  r6_1 = act6_act1_,
  r6_2 = act6_act2_,
  r6_3 = act6_act3_,
  r6_4 = act6_act4_,
  r6_5 = act6_act5_,
  r6_6 = act6_act6_,
}


local function get_rundown(ch1, ch2)
  local k = 'r'..tostring(ch1)..'_'..tostring(ch2)
  
  return rundown_[k]
end


return {
  get_rundown = get_rundown
}