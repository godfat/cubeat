
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

return {
  basepath = basepath
}

