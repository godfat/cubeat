local enet   = require 'enet'
local socket = require 'socket'
local gettime = require 'socket'.gettime
local ffi    = require 'ffi'
local C      = ffi.C
local kit    = require 'kit'
local msg    = kit.msg
local pmsg   = kit.pmsg
local dump   = kit.getDump('Protocol_Gameplay')

local EXPORT = {}
local net    = nil
local game   = nil

-- recv functions : gameplay receiver

local RECV = {}
RECV.MOV = function(m)
  --pmsg(m)
  -- ok now we assume it will always receive x value of cursor from C
  C.on_received(string.format("mov %d %d %d", m.tm, m.x, m.y)) 
end
RECV.HIT = function(m)
  --pmsg(m)
  if m.y%10 == 0 then print(m.y, gettime()) end
end
RECV.DIE = function(m)
  pmsg(m)
end
RECV.POKE = function(m)
  pmsg(m)
end
RECV.GREETING = function(m)
  pmsg(m)
  net.gotGreeting(m.src)
end
RECV.PLS_R = function(m)
  dump(m.T)
  table.foreach(m.ppl, function(k, v) v.addr = kit.addr_ext(v.addr) end)
  game.ppl = m.ppl
end

local recv = kit.getRecv(function (m)
  if RECV[m.T]==nil then
    dump('Incoming msg is not supported: '..m.T)
    return
  end
  RECV[m.T](m)
end)

-- outgoing messages
local function poke(peer)
  local m = msg('POKE')
  m.tm = os.time()
  kit.send(m, peer)
end

local function move(peer, x,y)
  local m = msg('MOV')
  m.x = x
  m.y = y
  m.tm = os.time()
  kit.send(m, peer)
end
local function hit(peer, x,y)
  local m = msg('HIT')
  m.x = x
  m.y = y
  m.tm = os.time()
  kit.send(m, peer)
end
local function plist(peer)
  local m = msg('PLS')
  m.pid = game.pid
  kit.send(m, peer)
end

EXPORT.recv = recv
EXPORT.poke = poke
EXPORT.move = move
EXPORT.hit = hit
EXPORT.plist = plist
EXPORT.setup = function(n, g)
  net = n
  game = g
end

return EXPORT
