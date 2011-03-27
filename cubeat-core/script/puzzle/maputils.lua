
local random = require 'script/puzzle/helpers'.random
local C3H, C4H, C5H, C3V, C4V = unpack(require 'script/puzzle/chain')

local MapUtils = {}
local floor = math.floor

function MapUtils.gen_map_from_exprs(w, h, exprs)
  local map = MapUtils.create_map(w, h)
  for chain,v in ipairs(exprs) do
    v:push_up_blocks_of(map)
    v:put_color_in(map, chain)
  end
  return map
end

function MapUtils.create_map(w, h)
  local map = {}
  map.width = w
  map.height = h
  for y = 1, h do
    map[y] = {}; for x = 1, w do map[y][x] = 0 end
  end
  return map
end

function MapUtils.display(map)
  for y = map.height, 1, -1 do
    for x = 1, map.width do
      io.write(string.format("%3d", map[y][x]))
    end
    print()
  end
  print()
end

local function gen_combinationsH_(c, w, h, len, ctor, starters)
  for y = 1, h do
    for x = 1, w - len + 1 do
      local temp = ctor(x*10 + y)
      temp.intersects = {}
      temp.answers = {}
      temp:answer_add() -- generate all possible answers in the c'tor
      table.insert(c, temp)
      if y == 1 then table.insert(starters, temp) end
    end
  end
end

local function gen_combinationsV_(c, w, h, len, ctor)
  for y = 1, h - len do -- don't +1 here, leave last row empty
    for x = 1, w do
      local temp = ctor(x*10 + y)
      temp.intersects = {}
      temp.answers = {}
      temp:answer_add() -- generate all possible answers in the c'tor
      table.insert(c, temp)
      -- don't use vertical combinations as starters.
    end
  end
end

local function gen_combinationsx(w, h)
  local c = {}
  local starters = {} -- combinations that can be the "last-invoked" chain.
  gen_combinationsH_(c, w, h, 3, C3H, starters)
  gen_combinationsH_(c, w, h, 4, C4H, starters)
  gen_combinationsH_(c, w, h, 5, C5H, starters)
  gen_combinationsV_(c, w, h, 3, C3V)
  gen_combinationsV_(c, w, h, 4, C4V)
  return c, starters
end

local function populate_intersects_of(key, combinations, height_limit) -- combinations should be immutable
  for _,v in ipairs(combinations) do
    key:intersect_add(v, height_limit) -- it will just simply insert it if passed
  end
end

function MapUtils.create_all_combinations(w, h)
  w = (w > 9  and 9  or w) or 6
  h = (h > 10 and 10 or h) or 10
  local all_combinations, starters = gen_combinationsx(w, h-1)
  local counter = 0
  for _, v in ipairs(all_combinations) do
    populate_intersects_of(v, all_combinations, h)
    counter = counter + 1
  end
  return all_combinations, starters, counter
end

local function do_check_chain_h(row, x)
  local i = x + 1
  while row[i] == row[i-1] do i = i + 1 end
  local len = i - x
  return (len >= 3), len
end

local function do_check_chain_v(map, x, y)
  local i = y + 1
  while map[i] and map[i][x] == map[i-1][x] do i = i + 1 end
  local len = i - y
  return (len >= 3), len
end

local function mark_for_delete_h(delete_mark, x, y, len)
  for i = 1, len do
    delete_mark[y][x+i-1] = 1
  end
end

local function mark_for_delete_v(delete_mark, x, y, len)
  for i = 1, len do
    delete_mark[y+i-1][x] = 1
  end
end

function MapUtils.find_chain(map)
  local chained, count = false, 0
  for y = 1, map.height do
    for x = 1, map.width do
      if map[y][x] > 0 then
        local res, len = do_check_chain_v(map, x, y)
        if res then chained = true; count = count + len end
        res, len = do_check_chain_h(map[y], x)
        if res then chained = true; count = count + len end
      end
    end
  end
  -- if we count len here, it sometimes will count duplication. but for verification purpose it should be ok.
  return chained, count
end

function MapUtils.destroy_chain(map)
  local delete_mark = MapUtils.create_map(map.width, map.height)
  local chained, count = false, 0
  for y = 1, map.height do
    for x = 1, map.width do
      if map[y][x] > 0 then
        local res, len = do_check_chain_v(map, x, y)
        if res then
          mark_for_delete_v(delete_mark, x, y, len)
          chained = true
        end
        res, len = do_check_chain_h(map[y], x)
        if res then
          mark_for_delete_h(delete_mark, x, y, len)
          chained = true
        end
      end
    end
  end
  for y = 1, map.height do
    for x = 1, map.width do
      if delete_mark[y][x] > 0 then
        map[y][x] = 0
        count = count + 1
      end
    end
  end
  return chained, count
end

function MapUtils.drop_blocks(map)
  for x = 1, map.width do
    local compact_col = {}
    for y = 1, map.height do
      if map[y][x] > 0 then
        table.insert(compact_col, map[y][x])
        map[y][x] = 0
      end
    end
    for y = 1, #compact_col do
      map[y][x] = compact_col[y]
    end
  end
end

local function specialized_deepcopy(map)
  local clone = MapUtils.create_map(map.width, map.height)
  for y = 1, map.height do
    for x = 1, map.width do
      clone[y][x] = map[y][x]
    end
  end
end

function MapUtils.check_puzzle_correctness(map, level)
  local clone = specialized_deepcopy(map)
  local chained, chain_count, destroy_count = true, -1, 3
  while chained and destroy_count >= 3 and destroy_count <= 5 do
    chained, destroy_count = MapUtils.destroy_chain(clone)
    MapUtils.drop_blocks(clone)
    chain_count = chain_count + 1
  end
  for y = 1, clone.height do
    for x = 1, clone.width do
      if clone[y][x] > 0 then
        return false
      end
    end
  end
  return true and chain_count == level
end

return MapUtils
