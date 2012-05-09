local basepath = require 'rc/script/helper'.basepath
package.path = basepath()..[[rc/script/net/?.lua;]]..package.path
package.cpath= basepath()..[[rc/script/net/?.dll;]]..package.cpath

local enet     = require 'enet'
local socket   = require 'socket'
local gettime  = require 'socket'.gettime
local sleep    = require 'socket'.sleep
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
void on_connected(char const*);
void on_matched(char const*);
void on_received(char const*);
void on_disconnected(char const*);
char const* poll_from_C();
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
net.gotoPlayer = function(tar)
  dump('state=CONN_TO_PLAYER')
  net.state = Const.CONN_TO_PLAYER
  if tar then net.farside(tar.addr) end
end
net.gotoPlayerReady = function()
  dump('state=READY_TO_PLAY')
  net.state = Const.READY_TO_PLAY

  C.on_matched('') -- only call matched when Player is READY_TO_PLAY

  if net.asServer == true then
    dump('Pose as '..'Server')
  else
    dump('Pose as '..'Client')
  end
end
net.gotoGame   = function() net.state = Const.IN_GAME end
net.gotoGiveup = function() net.state = Const.GIVE_UP end

net.isInLobby = function() return (net.state == Const.IN_LOBBY) end
net.isPlayerReady = function() return (net.state == Const.READY_TO_PLAY) end

-- connection management
net.conn_server  = nil
net.conn_farside = nil
net.host     = nil
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
    -- net.conn_server = net.host:connect(IP_LOCAL..":"..port)
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
  net.greeting = 0
  -- net.state    = Const.OFFLINE

  net.iam = {}
  net.iam.pri = {ip=IP_LOCAL, port=PORT}
  net.iam.prialt = {ip=IP_LOCAL, port=PORT+1000}

  net.tar = kit.addr_ext(tar)
end

net.reset = function()
  if net.conn_farside then net.conn_farside:disconnect() end
  if net.conn_server  then net.conn_server:disconnect() end
  net.greeting = 0
  net.state    = Const.OFFLINE
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
  if not net.isPlayerReady() then
    net.gotoPlayerReady()
    net.conn_farside = src
  else
    dump('ignore duplicated player connection from '..tostring(src))
  end

end

local function old_tick_poll_core(cc)
  -- if cc == 49 then
    -- if game.hasPlayerList() then
      -- prep.play_one(net.conn_server, game.ppl[1].pid)
    -- end
  -- elseif cc==50 then
    -- if net.isPlayerReady() then
      -- for i = 1, 100 do
        -- play.hit(net.conn_farside, 0, i)
        -- --net.host:flush()
      -- end
    -- end
  -- elseif cc==51 then
  -- elseif cc==52 then
    -- prep.chat_lobby(net.conn_server, string.random(6)..os.time())
  -- end
end

net.tick = function()

  -- commands from terminal
  local cc = ffi.string(C.poll_from_C())
  while cc and cc ~= '' do
    -- tick_poll_core(cc)
    if not net.isPlayerReady() then
      if cc == '1' then
        if game.hasPlayerList() then
          prep.play_one(net.conn_server, game.ppl[1].pid)
        end
      end
    else
      local getT = loadstring(cc)
      local t = getT()
      t.tm = os.time() -- appenddum
      -- play.move(net.conn_farside, cc, 100)
      kit.send(t, net.conn_farside)
    end
    cc = ffi.string(C.poll_from_C())
  end

  if (os.time() - net.tm > 0) then
    net.tm = os.time()

    if net.state == Const.CONN_TO_PLAYER then
      net.waitGreeting()
    end

    if net.tm % 10 == 0 and net.state == Const.IN_LOBBY then
      play.plist(net.conn_server)
    end

    -- keep-alive
    if net.tm % 1 == 0 and net.state >= Const.IN_LOBBY then
      -- dump('poke server. tm='..net.tm..' state='..net.state)
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
    dump("Lua: disconnected to farside: "..tostring(e.peer))
    net.state = Const.OFFLINE
  else
    dump(e)
  end
end

net.proc_server = function(e)
  if e.type == "receive" then
    prep.recv(e)
  elseif e.type == "connect" then
    print("Lua: server connected:", e.peer)

    C.on_connected('')

    if net.state == Const.CONN_TO_LOBBY then
      prep.send_iam(IP_LOCAL, PORT, e.peer)
    end

  elseif e.type == "disconnect" then
    print("Lua: disconnected to server: ", e.peer)
    net.state = Const.OFFLINE
    C.on_disconnected('')
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
    net.init(IP_LOCAL, PORT)
  elseif sc_flag == CLIENT then
    PORT = PORT_B
    net.asServer = false
    net.init(IP_LOCAL, PORT)
  end

  prep.setup(net, game)
  play.setup(net, game)

  if not net.gotoLobby() then
    print('Lua: host:connect failed')
    return false
  end
  print('Lua: host:connect succeed, but not yet acked.')
  return true
end

function run()
  local e = net.host:service(0) -- network event
  while e do
    if net.state <= Const.IN_LOBBY then
      net.proc_server(e)
    else
      net.proc_farside(e)
    end
    e = net.host:service(0)
  end
  net.tick()
end

function dtor()
  net.reset()
  dump('event loop ended.')
end
