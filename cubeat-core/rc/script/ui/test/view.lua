
local ffi      = require 'ffi'
local helper   = require 'rc/script/helper'
local basepath = helper.basepath
local C        = ffi.C

ffi.cdef[[
typedef struct TestUI TestUI;
typedef struct pScene pScene;
typedef struct pSprite pSprite;
]]
ffi.cdef( io.open( basepath().."rc/script/ui/test/bindings.ffi", 'r'):read('*a') )

------------- Some utils rigged for ffi callback management ---------------

local function _tracked_cb(btn_table, b, func)
  if btn_table[b] == nil then
    btn_table[b] = ffi.cast("PSC_OBJCALLBACK", func)
  else
    btn_table[b]:set(func)
  end
end

local function tracked_cb(cb_table, o, b, func)
  if cb_table[o] == nil then
    cb_table[o] = {}
    print 'new table for button registry added'
    _tracked_cb(cb_table[o], b, func)
  else
    _tracked_cb(cb_table[o], b, func)
  end
  return cb_table[o][b]
end

local function tracked_cb_removal(cb_table, o)
  for k, v in pairs(cb_table) do 
    print(k, v)
  end
  if type(cb_table[o]) == "table" then 
    for _, v1 in pairs(cb_table[o]) do
      print(_, v1)
      v1:free()
      v1 = nil
    end
  end
end

------------- "Class" definitions -----------------------------------------

local Mt_TestUI = {}
Mt_TestUI.__index      = Mt_TestUI

Mt_TestUI.get_ui_scene = function(self)
  return ffi.gc(C.TestUI_get_ui_scene(self), C.Scene__gc)
end

ffi.metatype("TestUI", Mt_TestUI)

local Mt_Scene = {}
Mt_Scene.__index     = Mt_Scene

ffi.metatype("pScene",  Mt_Scene)

local Mt_Sprite = {}
Mt_Sprite.__index    = Mt_Sprite
Mt_Sprite.set_pos    = C.Sprite_set_pos

local weakkey = {__mode = "k"}
Mt_Sprite.__on_releases__ = setmetatable({}, weakkey)
Mt_Sprite.__on_presses__  = setmetatable({}, weakkey)
Mt_Sprite.__on_downs__    = setmetatable({}, weakkey)
Mt_Sprite.__on_ups__      = setmetatable({}, weakkey)

Mt_Sprite.on_release = function(self, btn, func)
  C.Sprite_on_release(self, tracked_cb(Mt_Sprite.__on_releases__, self, btn, func))
end

ffi.metatype("pSprite", Mt_Sprite)

local function new_sprite(name, scene, w, h, center)
  return ffi.gc(C.Sprite_create(name, scene, w, h, center), function(self)
    tracked_cb_removal (Mt_Sprite.__on_releases__, self)
    print '--------'
    tracked_cb_removal (Mt_Sprite.__on_presses__, self)
    tracked_cb_removal (Mt_Sprite.__on_downs__, self)
    tracked_cb_removal (Mt_Sprite.__on_ups__, self)
    C.Sprite__gc(self)
  end)
end

return {
  new_sprite = new_sprite
}
