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

-- recv functions
local RECV = {}

RECV.URE = function(m)
  dump(m.T)

  game.pid = m.pid
  table.foreach(m.ppl, function(k, v) v.addr = kit.addr_ext(v.addr) end)
  game.ppl = m.ppl

  net.gotoLobbyReady()

  EXPORT.plist(m.src)  -- test
end

RECV.GREETING = function(m)
  pmsg(m)

  net.gotGreeting(m.src)
end

RECV.PLS_R = function(m)
  dump(m.T)
  if m.C==0 then
    table.foreach(m.ppl, function(k, v) v.addr = kit.addr_ext(v.addr) end)
    game.ppl = m.ppl
    game.ppl_map = {}
    table.foreach(game.ppl, function(k, v)
      game.ppl_map[v.pid] = v
    end)
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

RECV.PLAY_W = function(m)
  pmsg(m)
  if m.C == 0 then
    m.tar.addr = kit.addr_ext(m.tar.addr)
    game.ppl_map[m.tar.pid] = m.tar

    net.gotoPlayer(m.tar)
  else
    dump('no such player: '..m.tar.pid)
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
local function send_iam(ip, port, pserv)
  local m = msg('IAM')
  m.ip    = ip
  m.port  = port
  m.nick  = 'nick '..string.random(4, '%d')
  kit.send(m, pserv)
end
local function plist(pserv)
  local m = msg('PLS')
  m.pid   = game.pid
  kit.send(m, pserv)
end
local function poke_server(pserv)
  local m = msg('POKE')
  m.pid   = game.pid
  kit.send(m, pserv)
end
local function chat_lobby(pserv, txt)
  local m = msg('CHAT')
  m.pid   = game.pid
  m.txt   = txt
  m.type  = 'b' -- lobby
  kit.send(m, pserv)
end
local function play_one(pserv, pid) -- tell server who I want to play with
  local m = msg('PLAY_1')
  m.pid_me  = game.pid
  m.pid_tar = pid
  kit.send(m, pserv)
end

EXPORT.on = function(k,f) ON[k] = f end
EXPORT.chat_lobby  = chat_lobby
EXPORT.poke_server = poke_server
EXPORT.play_one = play_one
EXPORT.send_iam = send_iam
EXPORT.greeting = greeting
EXPORT.recv  = recv
EXPORT.plist = plist
EXPORT.setup = function(n, g)
  net  = n
  game = g
end

return EXPORT
