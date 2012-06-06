local msgpack   = require 'rc/script/ui/luajit-msgpack-pure'

local function load_data(data, filename)
  local file = io.open("rc/config/"..filename, "r")
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

local function save_data(data, filename)
  local s = msgpack.pack(data)
  local file = io.open("rc/config/"..filename, "w")
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