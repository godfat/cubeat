
local mp      = require "luajit-msgpack-pure"
local gettime = require 'socket'.gettime

local EXPORT  = {}

local function curry(f)
  return function (x)
    return function (y) return f(x,y) end
  end
end

local Chars = {}
for Loop = 0, 255 do
   Chars[Loop+1] = string.char(Loop)
end
local String = table.concat(Chars)

local Built = {['.'] = Chars}

local AddLookup = function(CharSet)
   local Substitute = string.gsub(String, '[^'..CharSet..']', '')
   local Lookup = {}
   for Loop = 1, string.len(Substitute) do
       Lookup[Loop] = string.sub(Substitute, Loop, Loop)
   end
   Built[CharSet] = Lookup

   return Lookup
end

function string.random(Length, CharSet)
   -- Length (number)
   -- CharSet (string, optional); e.g. %l%d for lower case letters and digits

   local CharSet = CharSet or '.'

   if CharSet == '' then
      return ''
   else
      local Result = {}
      local Lookup = Built[CharSet] or AddLookup(CharSet)
      local Range = table.getn(Lookup)

      for Loop = 1,Length do
         Result[Loop] = Lookup[math.random(1, Range)]
      end

      return table.concat(Result)
   end
end

-- table printer
local function table_print (tt, indent, done)
  done = done or {}
  indent = indent or 0
  if type(tt) == "table" then
    local sb = {}
    for key, value in pairs (tt) do
      table.insert(sb, string.rep (" ", indent)) -- indent it
      if type (value) == "table" and not done [value] then
        done [value] = true
        table.insert(sb, "{\n");
        table.insert(sb, table_print (value, indent + 2, done))
        table.insert(sb, string.rep (" ", indent)) -- indent it
        table.insert(sb, "}\n");
      elseif "number" == type(key) then
        table.insert(sb, string.format("\"%s\"\n", tostring(value)))
      else
        table.insert(sb, string.format(
            "%s = \"%s\"\n", tostring (key), tostring(value)))
       end
    end
    return table.concat(sb)
  else
    return tt .. "\n"
  end
end

local function _strtab ( tbl )
    if "nil" == type( tbl ) then
        return tostring(nil)
    elseif "table" == type( tbl ) then
        return table_print(tbl)
    elseif "string" == type( tbl ) then
        return tbl
    else
        return tostring(tbl)
    end
end

-- construct a message object
-- T = message type
-- C = returning code (optional)
EXPORT.msg = function (t, c)
  local r = {T=t}
  if c ~= nil then r.C = c end
  return r
end

-- print table
EXPORT.ptab = function(tb)
  print(_strtab(tb))
end

-- pring message
EXPORT.pmsg = function(m)
  print('***** '..m.T..' *****')
  print(_strtab(m))
end


local function _dump(header, text)
  print(header..': '.._strtab(text))
end

EXPORT.getDump = function(h)
  return curry(_dump)(h)
end

-- tool functions for address
EXPORT.addr = function (peer)
  local i,p = string.gmatch(tostring(peer), "(.+):(%w+)")()
  return {ip=i, port=p}
end

EXPORT.addr_str = function(addr)
  return (''..addr.ip)..':'..addr.port
end

EXPORT.addr_cmp = function (a,b)
  if a ==nil or b==nil then return false end
  return a.ip==b.ip and a.port==b.port
end

local function _parse(hnd, e)
  -- print('inside _parse peer=', e.peer)
  local off, m = mp.unpack(e.data)
  if (type(m)=='table') then
    m.src = e.peer
    hnd(m)
  else
    dump('received command is not an object: '..m)
  end
end

EXPORT.getRecv = function (hnd)
  return curry(_parse)(hnd)
end

EXPORT.send = function (obj, peer)
  peer:send(mp.pack(obj))
end

-- prepare additional info for connection
EXPORT.addr_ext = function(tar)
  -- for ns method 3
  tar.pubs = {}
  for i = 1, 5, 1 do
    tar.pubs[i] = {ip=tar.pub.ip, port=tar.pub.port+i}
  end

  -- for ns method 4
  tar.pubalt = {ip=tar.pub.ip, port=tar.pub.port+1000}
  tar.prialt = {ip=tar.pri.ip, port=tar.pri.port+1000}
  return tar
end

EXPORT.str_test1 = string.random(1024)
EXPORT.str_test2 = string.random(10240)

return EXPORT
