local ffi  = require 'ffi'
local C    = ffi.C
local view = require 'rc/script/ui/view'


local complete_rundown = 0
local actor_flag  = false
local word_flag   = false


local function get_count()
  return complete_rundown
end

local function add_count()
  complete_rundown = complete_rundown + 1
end

local function reset_count()
  complete_rundown = 0
end

local function set_actor_flag(flag)
  actor_flag = flag
end

local function set_word_flag(flag)
  word_flag = flag
end

--

local function actor_is_ready()
  actor_flag = true
  print(tostring(actor_flag))
  print(tostring(word_flag))
  if word_flag==true then complete_rundown = complete_rundown+1 print("complete_rundown: "..tostring(complete_rundown)) end
end

local function word_is_ready()
  word_flag = true
  if actor_flag==true then complete_rundown = complete_rundown+1 end
end

local function actor_cb()
  local function cb()
    actor_flag = true
    if word_flag==true then complete_rundown = complete_rundown+1 end
  end
  return cb
end

local function word_cb()
  local function cb()
    word_flag = true
    if actor_flag==true then complete_rundown = complete_rundown+1 end
  end
  return cb
end



return {
  complete_rundown  = complete_rundown,
  actor_flag        = actor_flag,
  word_flag         = word_flag,
  --
  get_count       = get_count,
  add_count       = add_count,
  reset_count     = reset_count,
  set_actor_flag  = set_actor_flag,
  set_word_flag   = set_word_flag,
  --
  actor_is_ready  = actor_is_ready,
  word_is_ready   = word_is_ready,
  actor_cb        = actor_cb,
  word_cb         = word_cb
}