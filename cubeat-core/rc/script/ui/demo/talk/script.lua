local ffi   = require 'ffi'
local C    = ffi.C
local view = require 'rc/script/ui/view'


--[[

[index]
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
  { index=1, img="char1_new/glad", text="Hello" },
  { index=1, text="This is a\ntest page" },
  { index=1, text="Let's test\ntalking" },
  { index=2, img="char1_new/glad", text="1~" },
  { index=2, text="1~\n2~" },
  { index=2, text="1~\n2~\n3!" },
  
  { index=1, text="ok" },
  { index=1, text="Let's test\nset position" },
  { index=2, text="300, 0", pos={x=300, y=0} },
  { index=2, text="700, 300", pos={x=700, y=300} },
  { index=2, text="450, 400", pos={x=450, y=400} },
  
  { index=1, text="ok" },
  { index=1, text="Let's test\nvisible" },
  { index=2, text="Ha", effect_a="hide" },
  { index=2, text="Ha", effect_a="show" },
  
  { index=1, text="ok" },
  { index=1, text="Let's test\nslide" },
  { index=2, text="yahoo~~~~", effect_a="slide_out" },
  { index=2, text="yahoo~~~~", effect_a="slide_in" },
    
  { index=1, text="ok" },
  { index=1, text="Let's test\nword size"},
  { index=2, text="HAHAHA", effect_w="size_L" },
  { index=2, text="HAHAHA", effect_w="size_S" },
  { index=2, text="HAHAHA", effect_w="size_M" },
    
  { index=1, text="ok" },
  { index=1, text="Let's test\nshake actor" },
  { index=2, text="Unnnnn", effect_a="shake" },
  
  { index=1, text="ok" },
  { index=1, text="Let's test\nshake word" },
  { index=2, text="Noooo!!", effect_w="shake" },
  { index=2, text="Remember\nturn back to\nmiddle size", effect_w="size_M" },
    
  { index=1, text="ok" },
  { index=1, text="Let's test\nfade" },
  { index=2, text="bye~~~", effect_a="fade_out" },
  { index=2, text="hi~~~", effect_a="fade_in" },
  
  { index=1, text="ok" },
  { index=1, text="test end" },
  
  { index=1, text="" }
}

local act2_act1_ = {
  { index=1, text="Test talk 2-1" },
  { index=2, text="HAHAHA", effect_a="hide", effect_w="shake" },
  { index=1, text="!?" },
  { index=2, text="Show time!!", effect_a="slide_in", effect_w="size_M" },
  { index=2, text="Rock!!", effect_a="shake" },
  { index=2, text="ok" },
  { index=2, text="test end" },
  
  { index=1, text="" }
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