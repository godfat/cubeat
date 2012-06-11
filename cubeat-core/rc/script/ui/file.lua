local msgpack   = require 'rc/script/ui/luajit-msgpack-pure'
local helper    = require 'rc/script/helper'
local basepath  = helper.basepath

local function load_data(filename, data)
  if filename == nil then error('load_data error -- the filename is nil') end
  if data     == nil then data={} end
  
  local file = io.open(basepath().."rc/config/"..filename, "r")
  if file == nil then
    io.output("rc/config/"..filename)
    io.close()
  else
    local offset
    offset, data = msgpack.unpack( file:read() )
    file:close()
  end
  return data
end

local function save_data(filename, data)
  if filename == nil then error('save_data error -- the filename is nil') end
  if data     == nil then data={} end
  
  local s = msgpack.pack(data)
  local file = io.open(basepath().."rc/config/"..filename, "w")
  file:write(s)
  file:close()
end


----------------------------------------------------------------------------
-- Main functions
----------------------------------------------------------------------------
return {
  load_data = load_data,
  save_data = save_data
}