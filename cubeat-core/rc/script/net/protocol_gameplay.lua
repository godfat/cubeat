local enet   = require 'enet'
local socket = require 'socket'
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
  pmsg(m)
end
RECV.HIT = function(m)
  pmsg(m)
end
RECV.DIE = function(m)
  pmsg(m)
end
RECV.POKE = function(m)
  pmsg(m)
end
RECV.GREETING = function()
  -- do nothing for duplicated greetings
end
RECV.PLS_R = function(m)
  dump(m.T)

  table.foreach(m.ppls, function(k, v) v.addr = kit.addr_ext(v.addr) end)
  game.ppls = m.ppls
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
  m.t = os.time()
  kit.send(m, peer)
end

local function move(peer, x,y)
  local m = msg('MOV')
  m.x = x
  m.y = y
  m.t = os.time()
  kit.send(m, peer)
end
local function hit(peer, x,y)
  local m = msg('HIT')
  m.x = x
  m.y = y
  m.t = os.time()
  kit.send(m, peer)
end
local function plist(peer)
  local m = msg('PLS')
  m.pid = pid
  kit.send(m, peer)
end

EXPORT.recv = recv
EXPORT.poke = poke
EXPORT.move = move
EXPORT.hit = hit
EXPORT.setup = function(n, g)
  net = n
  game = g
end
EXPORT.test = function()
  dump(game, 'game=')
end

return EXPORT
