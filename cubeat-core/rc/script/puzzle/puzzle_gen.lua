
local MapUtils = require 'rc/script/puzzle/maputils'
local Helper = require 'rc/script/puzzle/helpers'
local random, Stack = Helper.random, Helper.stack

math.randomseed(os.time())

local PuzzleGen = {}

function PuzzleGen:init(chain_limit, w, h)
  self.chain_limit = chain_limit
  self.w = w
  self.h = h
  self.all_combinations, self.starters = MapUtils.create_all_combinations(w, h)

  self:reinit()

  self.inited = true
end

function PuzzleGen:distribute_chain_lengths()
  local chain_lengths = nil
  if self.chain_limit > 15 then
    chain_lengths = {}
    for i = 1, self.chain_limit do table.insert(chain_lengths, 3) end
    local quota = random(20 - self.chain_limit + 1) + 1
    local i = 1
    while i <= self.chain_limit do
      local chance = random(10)
      if quota > 1 and chance <= 1 and i < self.chain_limit / 2 and chain_lengths[i-1] ~= 5 then
        chain_lengths[i] = 5
        quota = quota - 2
      elseif quota > 0 and chance <= 3 then
        chain_lengths[i] = 4
        quota = quota - 1
      else
        chain_lengths[i] = 3
      end
      i = i + 1
    end
  end
  return chain_lengths
end

function PuzzleGen:reinit()
  self.heights = {}
  self.chains = Stack()
  self.chain_lengths = self:distribute_chain_lengths()
  self.mapcache = MapUtils.create_map(self.w, self.h)

  for l = 1, self.chain_limit do
    self.heights[l] = {}
    for i = 1, self.w do
      self.heights[l][i] = 0
    end
  end

  for _,v in ipairs(self.all_combinations) do
    v.intersects_ptr = random(#v.intersects) + 1
    v.answers_ptr    = random(#v.answers) + 1
  end

  local c
  repeat
    c = self.starters[random(#self.starters)+1]
  until self:length_ok(c.len)
  self.chains:push(c:scopy())
  self.chains:top():update_heights( self.heights[1] )
  self.chains:top().color = 1
  self.chains:top():put_color_in(self.mapcache)
end

local function array_move(src, dest)
  for i,v in ipairs(src) do dest[i] = v end
end

function PuzzleGen:update_heights()
  array_move( self.heights[self.chains.size - 1], self.heights[self.chains.size] )
  self.chains:top():update_heights( self.heights[self.chains.size] )
end

function PuzzleGen:length_ok(len)
  if not self.chain_lengths then return true
  else
    return self.chain_lengths[self.chains.size + 1] == len
  end
end

function PuzzleGen:not_float(c)
  return c:not_float(self.heights[self.chains.size])
end

function PuzzleGen:not_too_high(c)
  return not c:too_high(self.heights[self.chains.size], self.h)
end

function PuzzleGen:add_final_answer(colored_map)
  local answers = self.chains:top().answers
  local ptr = self.chains:top().answers_ptr
  for i = 1, #answers do
    local ans = answers[ ((i+ptr) % #answers) + 1 ]
    if self:not_too_high(ans) then
      ans:push_up_blocks_of(colored_map)
      for color = 1, 4 do
        colored_map[ans.y][ans.x] = color
        if not MapUtils.find_chain(colored_map) then
          self.chains:push(ans:scopy())
          self.chains:top().color = color
          return true -- answer found. chain construction complete.
        end
      end
      ans:remove_from_map(colored_map) -- restore map to try next answer if all color failed.
    end
  end
  return false
end

function PuzzleGen:next_chain(level)
  local intersects = self.chains:top().intersects
  local ptr = self.chains:top().intersects_ptr
  for i = 1, #intersects do
    local c = intersects[ ((i+ptr) % #intersects) + 1 ]
    if self:length_ok(c.len) and self:not_float(c) and self:not_too_high(c) then
      local last_color = self.chains:top().color
      self.chains:push(c:scopy())
      self:update_heights()
      self.chains:top():push_up_blocks_of( self.mapcache )
      for k = 0, 3 do
        self.chains:top().color = ((last_color + k) % 4) + 1
        self.chains:top():put_color_in( self.mapcache ) -- important, only call this after color is assigned.
        local chained, possible_count = MapUtils.find_chain( self.mapcache )
        if possible_count == c.len then
          if self.chains.size >= self.chain_limit then
            if self:add_final_answer( self.mapcache ) then return true end
          else
            if self:next_chain( level + 1 ) then return true
            elseif level < self.chain_limit - 4 then return false end
          end
        end
      end
      self.chains:top():remove_from_map( self.mapcache )
      self.chains:pop() -- pop after we tested all colors of this combination
    end
  end
  return false -- if all possible answers for this level has been tried, return false
end

function PuzzleGen:generate(chain_limit, w, h, de_bug)
  w, h = w or 6, h or 10
  if not self.inited then self:init(chain_limit, w, h) end
  repeat
    self:reinit()
  until self:next_chain(2)

  local ans
  if de_bug then
    self.chains:display()
    print("Ans: ", self.chains:top())
    ans = tostring(self.chains:top())
  end
  local res = MapUtils.gen_map_from_exprs(w, h, self.chains)
  return res, ans
end

return PuzzleGen
