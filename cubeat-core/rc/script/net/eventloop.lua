local enet     = require 'enet'
local socket   = require 'socket'
local gettime = require 'socket'.gettime
local sleep   = require 'socket'.sleep
local ffi      = require 'ffi'
local C        = ffi.C
local kit      = require 'kit'
local addr     = kit.addr
local addr_cmp = kit.addr_cmp
local addr_str = kit.addr_str
local msg      = kit.msg
local pmsg     = kit.pmsg
local dump     = kit.getDump('Lua')
local ns       = require 'net_strategy'
local prep     = require 'protocol_preproc'
local play     = require 'protocol_gameplay'

ffi.cdef[[
void on_connected();
void on_matched();
void on_disconnected();
int  poll_from_C();
bool check_quit();
]]

local PORT_A = 2501
local PORT_B = 2502
local PORT = 2501
local IP_LOCAL = socket.dns.toip( socket.dns.gethostname() )
dump( "Local IP: "..IP_LOCAL )
local SERVER=1
local CLIENT=2

local net  = {}
local game = {}
game.lobby_addr = {ip="173.255.254.41", port="54321"}
game.hasPlayerList = function()
  dump('has player list? '..tostring( game.ppl ~= nil and game.ppl[1] ~= nil ))
  return ( game.ppl ~= nil and game.ppl[1] ~= nil )
end

-- state controls
local Const = {
  OFFLINE = 0,
  CONN_TO_LOBBY  = 1,
  IN_LOBBY       = 2,
  CONN_TO_PLAYER = 3,
  READY_TO_PLAY  = 4,
  IN_GAME        = 5,
  GIVE_UP = 9
}
net.gotoOffline = function()
  dump('state=OFFLINE')
  net.state = Const.OFFLINE
  net.reset()
end
net.gotoLobby = function()
  dump('state=CONN_TO_LOBBY')
  net.state = Const.CONN_TO_LOBBY
  return net.server(game.lobby_addr.ip, game.lobby_addr.port)
end
net.gotoLobbyReady = function()
  dump('state=IN_LOBBY')
  net.state = Const.IN_LOBBY
end
net.gotoPlayer = function()
  dump('state=CONN_TO_PLAYER')
  net.state = Const.CONN_TO_PLAYER
  if game.hasPlayerList() then
    net.farside(game.ppl[1].addr)
  end
end
net.gotoPlayerReady = function()
  dump('state=READY_TO_PLAY')
  net.state = Const.READY_TO_PLAY
  net.working = true
  if net.asServer == true then
    dump('Pose as '..'Server')
  else
    dump('Pose as '..'Client')
  end

end
net.gotoGame   = function() net.state = Const.IN_GAME end
net.gotoGiveup = function() net.state = Const.GIVE_UP end

-- connection management
net.conn_server  = nil
net.conn_farside = nil
net.host     = nil
net.working  = false
net.state    = Const.OFFLINE
net.tar      = nil  -- farside information
net.tm       = 0
net.greeting = 0

function net:tarPriAddr(i)
  if i ~= nil then return addr_str(net.tar.prialt) end
  return addr_str(net.tar.pri)
end

function net:tarPubAddr(i)
  if i ~= nil and i > 0 and i < 6 then return addr_str(net.tar.pubs[i])
  elseif i ~= nil and i > 100     then return addr_str(net.tar.pubalt)
  end
  return addr_str(net.tar.pub)
end

net.init = function(ip, port)
  dump('create host '..ip..':'..port)
  net.reset()
  net.host = enet.host_create(ip..":"..port)
end

net.server = function(ip, port)
  dump('connect to server...')
  local function foo()
    net.conn_server = net.host:connect(ip..":"..port)
    -- net.conn_server = net.host:connect(IP_LOCAL..":54321")
  end

  local ok, err = pcall(foo)

  if not ok then dump(err) end

  return ok
end

net.farside = function(info)
  dump('connect to farside...')
  if info then return ns.connect(net, info) end
  return ns.connect_next()
end

net.setup = function(tar)
  net.reset()

  net.iam = {}
  net.iam.pri = {ip=IP_LOCAL, port=PORT}
  net.iam.prialt = {ip=IP_LOCAL, port=PORT+1000}

  net.tar = kit.addr_ext(tar)
end

net.reset = function()
  if net.conn_farside then net.conn_farside:disconnect() end
  if net.conn_server  then net.conn_server:disconnect() end
  net.greeting = 0
  net.working  = false
end

net.waitGreeting = function()
  dump('wait for greetings...'..tostring(net.greeting))
  net.greeting = net.greeting + 1

  if net.greeting >= 3 then
    dump('wait too long...')
    if game.hasPlayerList() and (not net.farside()) then
      net.gotoGiveup()
    else
      net.gotoLobbyReady()
    end
  end
end

net.gotGreeting = function(src)
  net.gotoPlayerReady()
  net.conn_farside = src
end

net.tick = function(cc)

  -- commands from terminal
  if cc ~= 0 and cc ~= 65 and cc ~= nil then

      if cc == 49 then
        net.gotoPlayer()
      elseif cc==50 then
        if net.state >= Const.READY_TO_PLAY then
          for i = 1, 100 do
            play.hit(net.conn_farside, 0, i)
            --net.host:flush()
          end
        end
      elseif cc==51 then
      elseif cc==52 then
        prep.chat_lobby(net.conn_server, string.random(6)..os.time())
      end

  end

  if (os.time() - net.tm > 0) then
    net.tm = os.time()

    if net.state == Const.CONN_TO_PLAYER then
      net.waitGreeting()
    end

    if net.tm % 90 == 0 and net.state == Const.IN_LOBBY then
      play.plist(net.conn_server)
    end

    -- keep-alive
    if net.tm % 10 == 0 and net.working then
      dump('poke server. tm='..net.tm..' state='..net.state)
      prep.poke_server(net.conn_server)
    end
  end
end

net.proc_farside = function(e)
  if e.type == "receive" then
    play.recv(e)
  elseif e.type == "connect" then
    print("Lua: farside connected, send greeting:", e.peer)
    prep.greeting(e.peer)
  elseif e.type == "disconnect" then
    dump("disconnected:"..tostring(e.peer))
    net.working = false
  else
    dump(e)
  end
end

net.proc_server = function(e)
  if e.type == "receive" then
    prep.recv(e)
  elseif e.type == "connect" then
    print("Lua: server connected:", e.peer)

    if not net.conn_server then
      net.conn_server = e.peer
      C.on_connected()
    end

    C.on_matched()
    net.working = true

    if net.state == Const.CONN_TO_LOBBY then
      prep.send_iam(IP_LOCAL, PORT, e.peer)

    elseif net.state == Const.IN_LOBBY then
      prep.greeting(e.peer)
    end

  elseif e.type == "disconnect" then
    print("Lua: disconnected:", e.peer)
    net.working = false
    C.on_disconnected()
  else
    dump(e)
  end
end



-- Entry point
-- global function so it can be called from C++
function init(sc_flag)
  if sc_flag == SERVER then
    PORT = PORT_A
    net.asServer = true
    net.asClient = false
    net.init(IP_LOCAL, PORT)
  elseif sc_flag == CLIENT then
    PORT = PORT_B
    net.asServer = false
    net.asClient = true
    net.init(IP_LOCAL, PORT)
  end

  prep.setup(net, game)
  play.setup(net, game)
end

function run()

  if not net.gotoLobby() then return false end

  local busy = false

  while not C.check_quit() do

    local c = C.poll_from_C()      -- commands from c

    if busy then t = 1 else t = 100 end

    local e = net.host:service(t)  -- network event

    if e then
      if net.state <= Const.IN_LOBBY then
        net.proc_server(e)
      else
        net.proc_farside(e)
      end
      busy = true
    else
      busy = false
    end

    net.tick(c)

  end

  net.reset()
  dump('event loop ended.')
end
