local ffi   = require 'ffi'

--
local act1_act1 = {
  {index=1, img='char1_new/glad', text='Hello'},
  {index=1, img='char2_new/glad', text='Mic test'},
  {index=2, img='char3_new/glad', text='1~'},
  {index=1, img='char4_new/glad', text='2~'},
  {index=2, img='char5_new/glad', text='3!'},
  {index=1, img='char6_new/glad', text='Ok perfect'}
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