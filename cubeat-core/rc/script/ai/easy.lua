
local ffi      = require 'ffi'
local helper   = require 'rc/script/helper'
local basepath = helper.basepath
local random   = helper.random
local C        = ffi.C
ffi.cdef[[
typedef struct AIBrain AIBrain;
typedef struct pSimpleMap pSimpleMap;
typedef struct pSimpleCube pSimpleCube;
]]
ffi.cdef( io.open( basepath().."rc/script/ai/bindings.ffi", 'r'):read('*a') )

local Mt_AIBrain = {}
Mt_AIBrain.__index = Mt_AIBrain
Mt_AIBrain.push_command  = C.AIBrain_push_command
Mt_AIBrain.cmdqueue_size = C.AIBrain_cmdqueue_size

Mt_AIBrain.get_ally_map = function(self, index)
  return ffi.gc(C.AIBrain_get_ally_map(self, index), C.SimpleMap__gc)
end

Mt_AIBrain.get_enemy_map = function(self, index)
  return ffi.gc(C.AIBrain_get_enemy_map(self, index), C.SimpleMap__gc)
end

ffi.metatype("AIBrain", Mt_AIBrain)

local Mt_SimpleMap = {}
Mt_SimpleMap.__index = Mt_SimpleMap
Mt_SimpleMap.print_data_for_debug = C.SimpleMap_print_data_for_debug
Mt_SimpleMap.warning_level        = C.SimpleMap_warning_level
Mt_SimpleMap.garbage_left         = C.SimpleMap_garbage_left
Mt_SimpleMap.width                = C.SimpleMap_width
Mt_SimpleMap.height               = C.SimpleMap_height
Mt_SimpleMap.cube_exist_at        = C.SimpleMap_cube_exist_at
Mt_SimpleMap.grounded_cube_count  = C.SimpleMap_grounded_cube_count

Mt_SimpleMap.get_cube             = function(self, x, y) 
  return ffi.gc(C.SimpleMap_get_cube(self, x, y), C.SimpleCube__gc)
end

Mt_SimpleMap.get_grounded_cube    = function(self, x, y) 
  return ffi.gc(C.SimpleMap_get_grounded_cube(self, x, y), C.SimpleCube__gc)
end

Mt_SimpleMap.get_firepoint_cube   = function(map, lb, ub)
  return ffi.gc(C.SimpleMap_get_firepoint_cube(map, lb, ub), C.SimpleCube__gc)
end

Mt_SimpleMap.get_garbages         = function(self) 
  local size_out = ffi.new("unsigned int[1]")
  return ffi.gc(C.SimpleMap_get_garbages(self, size_out), function(self)
    C.SimpleCubeList__gc(self, size_out[0])
  end), size_out[0]
end

Mt_SimpleMap.get_brokens          = function(self)
  local size_out = ffi.new("unsigned int[1]") 
  return ffi.gc(C.SimpleMap_get_brokens(self, size_out), function(self)
    C.SimpleCubeList__gc(self, size_out[0])
  end), size_out[0]
end

Mt_SimpleMap.get_highcols         = function(self, threshold)
  local cols = ffi.new("int[?]", self:width())
  local size = 0
  for x = 0, self:width() - 1 do
    for y = 0, self:height() - 1 do 
      if not self:get_cube(x, y):exist() then
        if y >= threshold then cols[size] = x; size = size + 1 end 
        break
      end
    end
  end
  return cols, size
end

ffi.metatype("pSimpleMap", Mt_SimpleMap)

local Mt_SimpleCube = {}
Mt_SimpleCube.__index = Mt_SimpleCube
Mt_SimpleCube.exist      = C.SimpleCube_exist
Mt_SimpleCube.is_garbage = C.SimpleCube_is_garbage
Mt_SimpleCube.is_broken  = C.SimpleCube_is_broken
Mt_SimpleCube.x          = C.SimpleCube_x
Mt_SimpleCube.y          = C.SimpleCube_y
ffi.metatype("pSimpleCube", Mt_SimpleCube)

local ATTACK_PWR = 99

local function setcmd(buf, x, y, delay, type)
  buf.x, buf.y, buf.delay, buf.type = x, y, delay, type
  return buf
end

local function random_shuffle(array, size)
  for i = 0, size-1 do 
    local rnd = random(size)
    array[i], array[rnd] = array[rnd], array[i]
  end
end

function ai_entry(self)
  self = ffi.cast("AIBrain*", self)
  local my_map =    self:get_ally_map(0)
  local enemy_map = self:get_enemy_map(0)
  local cmdbuf    = ffi.new("LuaAICommand", {0, 0, 0, C.PSC_AI_NONE}) -- reuse this
  
  local attack_threshold = 8
  if ATTACK_PWR < 9 then      attack_threshold = 2
  elseif ATTACK_PWR < 20 then attack_threshold = 4
  end
  
  if my_map:warning_level() > 50 then     attack_threshold = 1
  elseif my_map:warning_level() > 25 then attack_threshold = 2
  elseif my_map:warning_level() > 0  then attack_threshold = 3
  elseif my_map:grounded_cube_count() + my_map:garbage_left() > 
         my_map:width() * (my_map:height() - 1) then attack_threshold = 1
  end
  
  local keycube = my_map:get_firepoint_cube(2, 99)
  if keycube:exist() and enemy_map:garbage_left() < ATTACK_PWR * 2 then -- why this logic?
    io.write( string.format("keycube at: %d, %d\n", keycube:x(), keycube:y()) )
    setcmd(cmdbuf, keycube:x(), keycube:y(), 0, C.PSC_AI_SHOOT)
    self:push_command(cmdbuf)
    if keycube:is_broken() then
      self:push_command(cmdbuf)
    elseif keycube:is_garbage() then
      self:push_command(cmdbuf)
      self:push_command(cmdbuf)
    end
  else
    io.write "No keycube for now.\n"
    local highcol_threshold = 9
    local highcols, hsize = my_map:get_highcols( highcol_threshold )
    local brokens,  bsize = my_map:get_brokens()
    
    if bsize > 0 then 
      random_shuffle(brokens, bsize) -- this should be ok? since garbages are actually "pointer to pointers"
      local rnd = random(bsize)
      setcmd(cmdbuf, brokens[rnd]:x(), brokens[rnd]:y(), 0, C.PSC_AI_SHOOT)
      self:push_command(cmdbuf)
    end

    if hsize > 0 then
      random_shuffle(highcols, hsize)
      local rnd_x, rnd_height = highcols[random(hsize)], random( highcol_threshold/2 )
      setcmd(cmdbuf, rnd_x, rnd_height, 0, C.PSC_AI_SHOOT)
      self:push_command(cmdbuf)
    end
    -- don't do garbages for now.
    
    if self:cmdqueue_size() < 1 then
      if my_map:grounded_cube_count() >= 48 then
        local x, y
        repeat
          x = random(my_map:width())
          y = random(my_map:height()/2)
        until my_map:get_grounded_cube(x, y):exist()
        setcmd(cmdbuf, x, y, 0, C.PSC_AI_SHOOT)
        self:push_command(cmdbuf) 
      else
        setcmd(cmdbuf, 0, 0, 0, C.PSC_AI_HASTE) -- we have to know player's heat level here! damn!
        self:push_command(cmdbuf) 
      end
    end
  end
  
  print(collectgarbage("count"))
  collectgarbage("collect")
end
