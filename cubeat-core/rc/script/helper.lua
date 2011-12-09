
local ffi = require 'ffi'

ffi.cdef[[
  char* _getcwd(char * buf, size_t size);
  char*  getcwd(char * buf, size_t size);
]]

local getcwd;
if jit.os == 'Windows' then
  getcwd = function(buf, size) return ffi.string( ffi.C._getcwd(buf, size) ) end
else
  getcwd = function(buf, size) return ffi.string( ffi.C.getcwd(buf, size) ) end
end

local function basepath()
  local buf = ffi.new("char[256]")
  return (getcwd(buf, 256).."/") -- get working directory with trailing / 
end

local function random(n) return math.floor(math.random()*n) end

local function C_random_shuffle(array, size)
  for i = 0, size-1 do 
    local rnd = random(size)
    array[i], array[rnd] = array[rnd], array[i]
  end
end

return {
  basepath = basepath,
  random   = random,
  C_random_shuffle = C_random_shuffle
}

