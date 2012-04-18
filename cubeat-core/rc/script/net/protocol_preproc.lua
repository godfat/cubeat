local enet   = require 'enet'
local socket = require 'socket'
local ffi    = require 'ffi'
local C      = ffi.C
local kit    = require 'kit'
local msg    = kit.msg
local pmsg   = kit.pmsg
local dump   = kit.getDump('Protocol_Preproc')

local EXPORT = {}
local net    = nil
local game   = nil


-- recv functions : connection receiver
local RECV = {}

-- replaced by plist()
-- RECV.TAR = function(m)
--   pmsg(m)
--   C.on_matched()
--   net.reset()     -- say goodbye to matcher
--   net.farside(m)  -- say hello to player
-- end

RECV.URE = function(m)
  dump(m)
  game.pid = m.pid

  table.foreach(m.ppl, function(k, v) v.addr = kit.addr_ext(v.addr) end)
  game.ppl = m.ppl

  EXPORT.plist(m.src) -- test
end

RECV.GREETING = function(m)
  pmsg(m)
  net.gotGreeting(m.src)
  net.readyToPlay()         -- state=3
end

RECV.PLS_R = function(m)
  dump(m)
  if m.C==0 then
    table.foreach(m.ppl, function(k, v) v.addr = kit.addr_ext(v.addr) end)
    game.ppl = m.ppl
  end
end

RECV.CHAT = function(m)
  dump(m)
  local sid = m.sid
  local txt = m.txt
  local type= m.type
  if type=='b' then
    dump(sid..' says '..txt..' at Lobby')
  end
end

-- receiver
local recv = kit.getRecv(function (m)
  if RECV[m.T]==nil then
    dump('Incoming msg is not supported: '..m.T)
    return
  end

  RECV[m.T](m)
end)


-- outgoing messages
local function greeting(peer)
  local m = msg('GREETING')
  kit.send(m, peer)
end
local function send_iam(ip, port, peer)
  local m = msg('IAM')
  m.ip    = ip
  m.port  = port
  m.nick  = 'nick '..string.random(4, '%d')
  kit.send(m, peer)
end
local function plist(peer)
  local m = msg('PLS')
  m.pid = game.pid
  kit.send(m, peer)
end
local function poke_server(peer)
  local m = msg('POKE')
  m.pid = game.pid
  kit.send(m, peer)
end
local function chat_lobby(peer, txt)
  local m = msg('CHAT')
  m.pid  = game.pid
  m.txt  = txt
  m.type = 'b' -- lobby
  kit.send(m, peer)
end


EXPORT.recv = recv
EXPORT.chat_lobby  = chat_lobby
EXPORT.poke_server = poke_server
EXPORT.send_iam = send_iam
EXPORT.greeting = greeting
EXPORT.plist = plist
EXPORT.setup = function(n, g)
  net  = n
  game = g
end

return EXPORT
