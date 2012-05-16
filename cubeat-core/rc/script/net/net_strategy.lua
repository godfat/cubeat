local kit      = require 'kit'
local dump     = kit.getDump('Lua-NetStt')
local sleep    = require 'socket'.sleep
local addr_str = kit.addr_str

local EXPORT = {}
local net    = nil
local step   = 0
local inc    = 1
local proc   = {}

local function _step1() -- connect to private ip
  dump('connect to '..net:tarPriAddr())
  net.conn_farside = net.host:connect(net:tarPriAddr())
end

local function _step2() -- connect to public ip
  dump('connect to '..net:tarPubAddr())
  net.conn_farside = net.host:connect(net:tarPubAddr())
end

local function _step3() -- connect to public ip by increasing port number
  dump('connect to '..net:tarPubAddr(inc))
  net.conn_farside = net.host:connect(net:tarPubAddr(inc))
end

EXPORT.reset = function (s)
  if s == nil then s = 1 end
  step = s
  if s < 3 then inc = 1 end
end

EXPORT.connect = function (n, tar, s)
  -- init farside information
  net = n
  net.setup(tar)

  proc[1] = _step1
  proc[2] = _step2
  proc[3] = _step3

  EXPORT.reset(s)
  return EXPORT.connect_next()
end

EXPORT.connect_next = function ()

  if step == nil then
    EXPORT.reset(1)
  elseif step > table.getn(proc) then
    dump('stop trying to connect to player')
    return false
  end

  dump('try method '..step)
  local ok, err = pcall(proc[step])

  if ok==false then
    dump(err)
  end

  if step == 3 and inc < 5 then
    inc = inc + 1
  else
    step = step + 1
  end

  return true
end

return EXPORT
