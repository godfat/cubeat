local ffi   = require 'ffi'
local C    = ffi.C
local view = require 'rc/script/ui/view'

local script_ = {
  { img="char1_new/anger", x=250, y=1200, w=256, h=512 },
  { text="Starring :", x=780, y=1300, size=60 },
  { text="Shu Qi", x=780, y=1420 },
  { text="Vicki Zhao", x=780, y=1480 },
  { text="Ethan Ruan", x=780, y=1540 },
  { text="Mark Jau", x=780, y=1600 },
  { text="Amber Kuo", x=780, y=1660 },
  
  { img="char2_new/glad", x=750, y=1800, w=256, h=512, flip=true },
  { text="Co-starring :", x=250, y=1800, size=60 },
  { text="Long Shao Hua", x=250, y=1920 },
  { text="Belle Yu", x=250, y=1980 },
  { text="Kan Kan", x=250, y=2040 },
  { text="Wang Jingchun", x=250, y=2100 },
  { text="Lin Muran", x=250, y=2160 },
  
  { text="Special Appearance :", x=320, y=2500, size=60 },
  { text="Yang Kuei Mei", x=500, y=2620 },
  { text="John Ting", x=500, y=2680 },
  { text="Li Hsuan", x=500, y=2740 },
  { text="Hsiao-yeh", x=500, y=2800 },
  { text="Rhydian Vaughan", x=500, y=2860 },
  { text="Chen Han Dian", x=500, y=2920 },
  { text="Emerson Tsai", x=500, y=2980 },
  { text="Frankie Huang", x=500, y=3040 },
  { text="Zhang Haiyan", x=500, y=3100 },
  { text="Du Jiayi", x=500, y=3160 },
  { text="Joelle  Lu", x=500, y=3220 },
  { text="Zhi-Ying Zhu", x=500, y=3280 },
  { text="Bright Pu", x=500, y=3340 },
  { text="Aho Huang", x=500, y=3400 },
  { text="Billy Wei", x=500, y=3460 },
  { text="Chen Yu-Hsun", x=500, y=3520 },
  { text="Li Jun", x=500, y=3580 },
  
  { text="TEAM PSC", x=500, y=4000, size=60 },
}
local roll_time_      = 15000
local roll_distance_  = -3700
local default_size_   = 40
local default_depth_  = -500

--

local function get_staffroll()
  return script_
end

local function get_roll_time()
  return roll_time_
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