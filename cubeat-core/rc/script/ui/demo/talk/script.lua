local ffi   = require 'ffi'
local C    = ffi.C
local view = require 'rc/script/ui/view'


--[[

[index]
1 = left actor
2 = right actor

[actor_effect]
show
hide
slide_in
slide_out
fade_in
fade_out
shake

[word_effect]
size_L
size_M
size_S
shake

--]]


--
local act1_act1 = {
  { index=1, img="char1_new/glad", text="Hello" },
  { index=1, text="This is a\ntest page" },
  { index=1, text="Let's test\ntalking" },
  { index=2, img="char1_new/glad", text="1~" },
  { index=2, text="1~\n2~" },
  { index=2, text="1~\n2~\n3!" },
  
  { index=1, text="ok" },
  { index=1, text="Let's test\nvisible" },
  { index=2, text="Ha", actor_effect="hide" },
  { index=2, text="Ha", actor_effect="show" },
  
  { index=1, text="ok" },
  { index=1, text="Let's test\nslide" },
  { index=2, text="yahoo~~~~", actor_effect="slide_out" },
  { index=2, text="yahoo~~~~", actor_effect="slide_in" },
    
  { index=1, text="ok" },
  { index=1, text="Let's test\nword size"},
  { index=2, text="HAHAHA", word_effect="size_L" },
  { index=2, text="HAHAHA", word_effect="size_S" },
  { index=2, text="HAHAHA", word_effect="size_M" },
    
  { index=1, text="ok" },
  { index=1, text="Let's test\nshake actor" },
  { index=2, text="Unnnnn", actor_effect="shake" },
  
  { index=1, text="ok" },
  { index=1, text="Let's test\nshake word" },
  { index=2, text="Noooo!!", word_effect="shake" },
  { index=2, text="Remember\nturn back to\nmiddle size", word_effect="size_M" },
    
  { index=1, text="ok" },
  { index=1, text="Let's test\nfade" },
  { index=2, text="bye~~~", actor_effect="fade_out" },
  { index=2, text="hi~~~", actor_effect="fade_in" },
  
  { index=1, text="ok" },
  { index=1, text="test end" },
  
  { index=1, text="" }
}

local act2_act1 = {
  { index=1, text="Test talk 2-1" },
  { index=2, text="HAHAHA", actor_effect="hide", word_effect="shake" },
  { index=1, text="!?" },
  { index=2, text="Show time!!", actor_effect="slide_in", word_effect="size_M" },
  { index=2, text="Rock!!", actor_effect="shake" },
  { index=2, text="ok" },
  { index=2, text="test end" },
  
  { index=1, text="" }
}


local rundown = {
  r1_1 = act1_act1,
  r2_1 = act2_act1,
}


local function get_rundown(ch1, ch2)
  local k = 'r'..tostring(ch1)..'_'..tostring(ch2)
  
  return rundown[k]
end


return {
  get_rundown = get_rundown
}