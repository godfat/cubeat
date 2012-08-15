local ffi   = require 'ffi'
local C    = ffi.C
local view = require 'rc/script/ui/view'


--
local act1_act1 = {
  { index=1, img="char1_new/glad", text="Hello" },
  { index=1, text="This is a\ntest page" },
  { index=1, text="Let's test\ntalking" },
  
  { index=2, img="char1_new/glad", text="1~" },
  { index=2, text="1~\n2~" },
  { index=2, text="1~\n2~\n3!" },
  
  { index=1, text="ok" },
  { index=1, text="Let's test\nslide" },
  
  { index=2, text="yahoo~~~~", effect="slide_out" },
  { index=2, text="yahoo~~~~", effect="slide_in" },
    
  { index=1, text="ok" },
  { index=1, text="Let's test\nword scale"},
  
  { index=2, text="HAHAHA", effect="word_L" },
  { index=2, text="HAHAHA", effect="word_S" },
    
  { index=1, text="ok" },
  { index=1, text="Let's test\nshake" },
  
  { index=2, text="shake", effect="shake" },
    
  { index=1, text="ok" },
  { index=1, text="Let's test\nfade" },
  
  { index=2, text="bye~~~", effect="fade_out" },
  { index=2, text="hi~~~", effect="fade_in" },
  
  { index=1, text="ok" },
  { index=1, text="test end" },
  
  { index=1, text="" }
}


local rundown = {
  c1_1 = act1_act1,
}


local function get_rundown(ch1, ch2)
  local k = 'c'..tostring(ch1)..'_'..tostring(ch2)
  
  return rundown[k]
end


return {
  get_rundown = get_rundown
}