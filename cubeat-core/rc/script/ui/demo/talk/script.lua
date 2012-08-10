local ffi   = require 'ffi'
local C    = ffi.C
local view = require 'rc/script/ui/view'

local bg_w  = view.GET_SCREEN_W()
local bg_h  = view.GET_SCREEN_H()

local function v2(x, y)
  return ffi.new("value2", x, y)
end
local function v3(x, y, z)
  return ffi.new("value3", x, y, z)
end

--
local act1_act1 = {
  { index=1, img="char1_new/glad", text="Hello" },
  { index=1, text="This is a\ntest page" },
  { index=1, text="Let's test\ntalking" },
  { index=2, img="char2_new/glad", text="1~" },
  { index=2, text="1~\n2~" },
  { index=2, text="1~\n2~\n3!" },
  { index=1, text="ok" },
  { index=1, text="Let's test\nslide in" },
  { index=2, text="yahoo~~~~",
    target="actor", Eq="OElastic", Accessor="Pos2D", s=v2(bg_w,0), e=v2(bg_w-425,0), dur=1000 },
  { index=1, text="ok" },
  { index=1, text="Let's test\nscale"},
  { index=2, text="yo~~~",
    target="content", Eq="Linear", Accessor="Scale", s=v3(1,1,0), e=v3(2,2,0), dur=200 },
  { index=2, text="yo~~~",
    target="content", Eq="Linear", Accessor="Scale", s=v3(2,2,0), e=v3(1,1,0), dur=1 },
  { index=1, text="ok" },
  { index=1, text="Let's test\nshake" },
  { index=2, text="shake",
    target="actor", Eq="OElastic", Accessor="Pos2D", s=v2(bg_w-425-10,0), e=v2(bg_w-425,0), dur=1000},
}


local rundown = {
  c1_1 = act1_act1
}


local function get_rundown(ch1, ch2)
  local k = 'c'..tostring(ch1)..'_'..tostring(ch2)
  
  return rundown[k]
end


return {
  get_rundown = get_rundown
}