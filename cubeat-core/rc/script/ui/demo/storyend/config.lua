local is_story_end_   = false
local story_win_      = 1
local story_lose_     = 2
local tutorial_win_   = 3
local tutorial_lose_  = 4

local story_stage = 1

local data = {}

data[1] = {
  {ch=1, lv=0},
  {ch=2, lv=0},
  {ch=6, lv=0},
  {ch=3, lv=1},
  {ch=4, lv=1},
  {ch=5, lv=2},
}

data[2] = {
  {ch=2, lv=0},
  {ch=1, lv=0},
  {ch=3, lv=0},
  {ch=4, lv=1},
  {ch=5, lv=1},
  {ch=6, lv=2},
}

data[3] = {
  {ch=3, lv=0},
  {ch=4, lv=0},
  {ch=2, lv=0},
  {ch=1, lv=1},
  {ch=5, lv=1},
  {ch=6, lv=2},
}

data[4] = {
  {ch=4, lv=0},
  {ch=1, lv=0},
  {ch=3, lv=0},
  {ch=6, lv=1},
  {ch=1, lv=1},
  {ch=5, lv=2},
}

data[5] = {
  {ch=5, lv=0},
  {ch=1, lv=0},
  {ch=2, lv=0},
  {ch=3, lv=1},
  {ch=6, lv=1},
  {ch=2, lv=2},
}

data[6] = {
  {ch=6, lv=0},
  {ch=4, lv=0},
  {ch=2, lv=0},
  {ch=3, lv=1},
  {ch=5, lv=1},
  {ch=1, lv=2},
}

local function set_stage(v)
  story_stage = v
end

local function next_stage()
  story_stage = story_stage + 1
  if story_stage > 6 then story_stage=6 end
end

local function get_data(n)
  return data[n][story_stage]
end

local function get_stage()
  return story_stage
end

return{
  is_story_end  = is_story_end_,
  story_win     = story_win_,
  story_lose    = story_lose_,
  tutorial_win  = tutorial_win_,
  tutorial_lose = tutorial_lose_,
  --
  set_stage     = set_stage,
  next_stage    = next_stage,
  get_data      = get_data,
  get_stage     = get_stage
}