local kit      = require 'kit'
local dump     = kit.getDump('Lua-NS')
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

local function _step4() -- connect to farside by opening new port
  local addr = net.iam.prialt
  net.init(addr.ip, addr.port)
  sleep(2)  -- wait for farside's initialization
  -- dump('connect to '..net:tarPriAddr(1000))
  net.conn_farside = net.host:connect(net:tarPriAddr(1000))
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
  --proc[4] = _step4

  EXPORT.reset(s)
  return EXPORT.connect_next()
end

EXPORT.connect_next = function ()

  if step == nil then
    EXPORT.reset(1)
  elseif step > table.getn(proc) then
    dump('Out of methods')
    return false
  end

  dump('try method '..step)
  local ok, err = pcall(proc[step])

  if ok==false then
    dump(err)
  else
    net.state = 1
  end

  if step == 3 and inc < 5 then
    inc = inc + 1
  else
    step = step + 1
  end
  return true
end

return EXPORT
