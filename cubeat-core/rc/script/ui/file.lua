local msgpack   = require 'rc/script/ui/luajit-msgpack-pure'
local helper    = require 'rc/script/helper'
local basepath  = helper.basepath

local function check_filename(filename)
  if filename==nil then
    error('error -- the filename is nil') end
  if string.len(filename)<1 then
    error('error -- the filename length is 0') end
end
local function check_data(data)
  if data==nil then
    error('error -- the data is nil') end
end

local function load_data(filename, m)
  print('load_data')
  check_filename(filename)
  
  local mode = m or "r"
  
  local offset
  local data
  local file = io.open(basepath().."rc/config/"..filename, mode)
  if file == nil then
    print('Could not open this file: '..filename)
  else
    offset, data = msgpack.unpack( file:read("*all") )
    if data=="invalid argument" then data=nil end
    file:close()
  end

  return data
end

local function save_data(filename, data, m)
  check_filename(filename)
  check_data(data)
  
  local mode = m or "w"
  
  local s = msgpack.pack(data)
  local file = io.open(basepath().."rc/config/"..filename, mode)
  if file==nil then
    io.output(basepath().."rc/config/"..filename)
    io.close()
    file = io.open(basepath().."rc/config/"..filename, mode)
  end
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