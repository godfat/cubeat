local story_stage = 1

local data = {}

data[1] = {
  {ch=1, lv=0},
  {ch=2, lv=0},
  {ch=3, lv=0},
  {ch=4, lv=1},
  {ch=5, lv=1},
  {ch=6, lv=2},
}

data[2] = {
  {ch=6, lv=0},
  {ch=5, lv=0},
  {ch=4, lv=0},
  {ch=3, lv=1},
  {ch=2, lv=1},
  {ch=1, lv=2},
}

data[3] = {
  {ch=1, lv=0},
  {ch=2, lv=0},
  {ch=3, lv=0},
  {ch=4, lv=1},
  {ch=5, lv=1},
  {ch=6, lv=2},
}

data[4] = {
  {ch=1, lv=0},
  {ch=2, lv=0},
  {ch=3, lv=0},
  {ch=4, lv=1},
  {ch=5, lv=1},
  {ch=6, lv=2},
}

data[5] = {
  {ch=1, lv=0},
  {ch=2, lv=0},
  {ch=3, lv=0},
  {ch=4, lv=1},
  {ch=5, lv=1},
  {ch=6, lv=2},
}

data[6] = {
  {ch=1, lv=0},
  {ch=2, lv=0},
  {ch=3, lv=0},
  {ch=4, lv=1},
  {ch=5, lv=1},
  {ch=6, lv=2},
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
  set_stage   = set_stage,
  next_stage  = next_stage,
  get_data    = get_data,
  get_stage   = get_stage
}