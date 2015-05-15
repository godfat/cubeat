local ffi   = require 'ffi'
local C    = ffi.C
local view = require 'rc/script/ui/view'

local script_ = {
  { img="char1_new/anger", x=250, y=1160, w=256, h=512 },
  { text="Credit", x=740, y=1300, size=60 },
  { text="Core Team -", x=740, y=1420 },
  { text="Jung-Sheng Lin", x=740, y=1480 },
  { text="Kun-Wei Lin", x=740, y=1540 },
  { text="Maxwell Lai", x=740, y=1600 },
  
  { img="char2_new/glad", x=750, y=1840, w=256, h=512, flip=true },
  { text="Character & Background", x=250, y=1800, size=60 },
  { text="B.c.N.y", x=250, y=1920 },
  { text="Music", x=250, y=2100, size=60 },
  { text="\"Waiting\" Shen", x=250, y=2220 },
  
  { text="Original Prototype", x=400, y=2500, size=60 },
  { text="Jung-Sheng Lin", x=500, y=2620 },
  { text="Kun-Wei Lin", x=500, y=2680 },
  { text="Jen-Shin Lin", x=500, y=2740 },
  { text="You-Ren Lin", x=500, y=2800 },
  { text="Akito Lin", x=500, y=2860 },
  { text="Tadashi Chiu", x=500, y=2920 },
  { text="Special Thanks", x=400, y=3120, size=60 },
  { text="Faust Li", x=500, y=3240 },
  { text="Masamitsu Ishikawa", x=500, y=3300 },
  { text="Agarie Ryo", x=500, y=3360 },
  { text="Aaron Isaksen", x=500, y=3420 },
  { text="Alexander Bruce", x=500, y=3480 },
  { text="Douglas Wilson", x=500, y=3540 },
  { text="Chi Wei Lee", x=500, y=3600 },
  { text="... and you!", x=500, y=3660 },
  
  { text="TEAM PSC", x=500, y=4000, size=60 },
}
local roll_time_      = {45000,
                         45000,
                         45000,
                         45000,
                         45000,
                         45000,}
local roll_distance_  = -3700
local default_size_   = 40
local default_depth_  = -600

--

local function get_staffroll()
  return script_
end

local function get_roll_time(i)
  return roll_time_[i]
end

local function get_roll_distance()
  return roll_distance_
end

local function get_default_size()
  return default_size_
end

local function get_default_depth()
  return default_depth_
end

--

return {
  get_staffroll     = get_staffroll,
  get_roll_time     = get_roll_time,
  get_roll_distance = get_roll_distance,
  get_default_size  = get_default_size,
  get_default_depth = get_default_depth,
}