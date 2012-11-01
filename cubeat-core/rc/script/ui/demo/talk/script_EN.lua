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

--]]


--
local act1_act1_ = {
  { ch=1, img="char1_new/glad", text="Hello" },
  { ch=1, text="This is a\ntest page" },
  { ch=1, text="Let's test\ntalking" },
  { ch=2, img="char1_new/glad", text="1~" },
  { ch=2, text="1~\n2~" },
  { ch=2, text="1~\n2~\n3!" },
  
  { ch=1, text="ok" },
  { ch=1, text="Let's test\nset position" },
  { ch=2, text="300, 0", pos={x=300, y=0} },
  { ch=2, text="700, 300", pos={x=700, y=300} },
  { ch=2, text="450, 400", pos={x=450, y=400} },
  
  { ch=1, text="ok" },
  { ch=1, text="Let's test\nvisible" },
  { ch=2, text="Ha", effect_a="hide" },
  { ch=2, text="Ha", effect_a="show" },
  
  { ch=1, text="ok" },
  { ch=1, text="Let's test\nslide" },
  { ch=2, text="yahoo~~~~", effect_a="slide_out" },
  { ch=2, text="yahoo~~~~", effect_a="slide_in" },
    
  { ch=1, text="ok" },
  { ch=1, text="Let's test\nword size"},
  { ch=2, text="HAHAHA", effect_w="size_L" },
  { ch=2, text="HAHAHA", effect_w="size_S" },
  { ch=2, text="HAHAHA", effect_w="size_M" },
    
  { ch=1, text="ok" },
  { ch=1, text="Let's test\nshake actor" },
  { ch=2, text="Unnnnn", effect_a="shake" },
  
  { ch=1, text="ok" },
  { ch=1, text="Let's test\nshake word" },
  { ch=2, text="Noooo!!", effect_w="shake" },
  { ch=2, text="Remember\nturn back to\nmiddle size", effect_w="size_M" },
    
  { ch=1, text="ok" },
  { ch=1, text="Let's test\nfade" },
  { ch=2, text="bye~~~", effect_a="fade_out" },
  { ch=2, text="hi~~~", effect_a="fade_in" },
  
  { ch=1, text="ok" },
  { ch=1, text="Let's test\nchange board" },
  { ch=2, text="Cubeat Style!!", board="cubeat_800x200" },
  --{ ch=2, text="Origin Style!!", board="area_rect" },
  
  { ch=1, text="ok" },
  { ch=1, text="Let's text\nboard_flipH" },
  { ch=2, text="Fliiiiiiiip!!", board_flip="H" },
  
  { ch=1, text="ok" },
  { ch=1, text="Let's text\nboard_flipV" },
  { ch=2, text="Fliiiiiiiip!!", board_flip="V" },
  
  { ch=1, text="ok" },
  { ch=1, text="Let's text\nboard_flipHV" },
  { ch=2, text="Fliiiiiiiip!!", board_flip="HV" },
  
  { ch=1, text="ok" },
  { ch=1, text="test end" },
  
  { ch=1, text="" }
}

local act2_act1_ = {
  { ch=1, text="Test talk 2-1" },
  { ch=2, text="HAHAHA", effect_a="hide", effect_w="shake" },
  { ch=1, text="!?" },
  { ch=2, text="Show time!!", effect_a="slide_in", effect_w="size_M" },
  { ch=2, text="Rock!!", effect_a="shake" },
  { ch=2, text="ok" },
  { ch=2, text="test end" },
  
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