local ffi       = require 'ffi'
local C         = ffi.C
local helper    = require 'rc/script/helper'
local basepath  = helper.basepath
local copy_cdata_mt = helper.copy_cdata_mt
local cdata_addr= helper.cdata_addr

ffi.cdef[[
typedef struct pObject pObject;
typedef struct pScene pScene;
typedef struct pSprite pSprite;
typedef struct pSpriteText pSpriteText;
typedef struct Input Input;
typedef struct Button Button;
typedef struct { double x, y; } value2;
typedef struct { double x, y, z; } value3;
]]
ffi.cdef( io.open( basepath().."rc/script/ui/bindings.ffi", 'r'):read('*a') )

----------------------------------------------------------------------------
-- FFI callback hackery
----------------------------------------------------------------------------

local ObjCallbackT      = ffi.typeof("PSC_OBJCALLBACK")
local ObjCallback_Int2T = ffi.typeof("PSC_OBJCALLBACK_INT2")

local function tracked_cb(cb_table, T, obj, subkey, func)
  if cb_table[ cdata_addr(obj) ] == nil then
    cb_table[ cdata_addr(obj) ] = {}
  end
  if cb_table[ cdata_addr(obj) ][subkey] == nil then
    cb_table[ cdata_addr(obj) ][subkey] = ffi.cast(T, func)
  else
    cb_table[ cdata_addr(obj) ][subkey]:set(func)
  end
  return cb_table[ cdata_addr(obj) ][subkey]
end

local function tracked_cb_removal(cb_table, obj)
  if cb_table[ cdata_addr(obj) ] ~= nil then
    for _, v1 in pairs(cb_table[ cdata_addr(obj) ]) do
      v1:free()
    end
    cb_table[ cdata_addr(obj) ] = nil -- have to remove the record ourselves.
  end
end

local __on_press__   = {} -- we use cdata address number as key, it doesn't have to be weak
local __on_release__ = {}
local __on_down__    = {}
local __on_up__      = {}
local __on_enter_focus__ = {}
local __on_leave_focus__ = {}
local __tween_of__ = {}

local function remove_callbacks(p)
  tracked_cb_removal(__on_press__, p)
  tracked_cb_removal(__on_release__, p)
  tracked_cb_removal(__on_down__, p)
  tracked_cb_removal(__on_up__, p)
  tracked_cb_removal(__on_enter_focus__, p)
  tracked_cb_removal(__on_leave_focus__, p)
  tracked_cb_removal(__tween_of__, p)
end

local function debug_hack()
  local c = 0
  for k, v in pairs(__tween_of__) do
    c = c + 1
  end
  print("total obj count for tweening callback table: ", c)
end

----------------------------------------------------------------------------
-- "Class" definitions
----------------------------------------------------------------------------

-- pScene
local Mt_Scene  = {}
Mt_Scene.__index  = Mt_Scene

ffi.metatype("pScene",  Mt_Scene)


-- pSprite
local Mt_Sprite = {}
Mt_Sprite.__index                 = Mt_Sprite
Mt_Sprite.set_texture             = C.Sprite_set_texture
Mt_Sprite.set_pos                 = C.Sprite_set_pos
Mt_Sprite.set_rotation            = C.Sprite_set_rotation
Mt_Sprite.set_scale               = C.Sprite_set_scale
Mt_Sprite.set_size                = C.Sprite_set_size
Mt_Sprite.set_depth               = C.Sprite_set_depth
Mt_Sprite.set_color               = C.Sprite_set_color
Mt_Sprite.set_emissive            = C.Sprite_set_emissive
Mt_Sprite.set_red                 = C.Sprite_set_red
Mt_Sprite.set_green               = C.Sprite_set_green
Mt_Sprite.set_blue                = C.Sprite_set_blue
Mt_Sprite.set_alpha               = C.Sprite_set_alpha
Mt_Sprite.set_visible             = C.Sprite_set_visible
Mt_Sprite.set_center_aligned      = C.Sprite_set_center_aligned

Mt_Sprite.tween                   = function(self, Eq, Accessor, s, e, dur, l, cb, d)
  cb = cb and tracked_cb(__tween_of__, ObjCallbackT, self, Accessor, cb) or nil
  if type(s)=='number' and type(e)=='number' then
    C.Sprite_tween_1d(ffi.cast("pSprite*", self), Eq, Accessor, s, e, dur, l or 0, cb, d or 0)
  else
    C.Sprite_tween(ffi.cast("pSprite*", self), Eq, Accessor, s, e, dur, l or 0, cb, d or 0)
  end
end

Mt_Sprite.texture_flipH           = C.Sprite_texture_flipH
Mt_Sprite.texture_flipV           = C.Sprite_texture_flipV
Mt_Sprite.get_pos_x               = C.Sprite_get_pos_x
Mt_Sprite.get_pos_y               = C.Sprite_get_pos_y
Mt_Sprite.get_size_x              = C.Sprite_get_size_x
Mt_Sprite.get_size_y              = C.Sprite_get_size_y
Mt_Sprite.get_screen_pos_x        = C.Sprite_get_screen_pos_x
Mt_Sprite.get_screen_pos_y        = C.Sprite_get_screen_pos_y
Mt_Sprite.lazy_fix_alpha_artifact = C.Sprite_lazy_fix_alpha_artifact
Mt_Sprite.remove_texture          = C.Sprite_remove_texture
Mt_Sprite.set_blending            = C.Sprite_set_blending

Mt_Sprite.on_release              = function(p, b, func)
  C.Sprite_on_release(ffi.cast("pSprite*", p), b, tracked_cb(__on_release__, ObjCallbackT, p, b, func))
end

Mt_Sprite.on_press                = function(p, b, func)
  C.Sprite_on_press(ffi.cast("pSprite*", p), b, tracked_cb(__on_press__, ObjCallbackT, p, b, func))
end

Mt_Sprite.on_up                   = function(p, b, func)
  C.Sprite_on_up(ffi.cast("pSprite*", p), b, tracked_cb(__on_up__, ObjCallbackT, p, b, func))
end

Mt_Sprite.on_down                 = function(p, b, func)
  C.Sprite_on_down(ffi.cast("pSprite*", p), b, tracked_cb(__on_down__, ObjCallbackT, p, b, func))
end

Mt_Sprite.on_enter_focus          = function(p, input, func)
  C.Sprite_on_enter_focus(ffi.cast("pSprite*", p), input, tracked_cb(__on_enter_focus__, ObjCallback_Int2T, p, input, func))
end

Mt_Sprite.on_leave_focus          = function(p, input, func)
  C.Sprite_on_leave_focus(ffi.cast("pSprite*", p), input, tracked_cb(__on_leave_focus__, ObjCallback_Int2T, p, input, func))
end

Mt_Sprite.remove                  = function(p, texture_release)
  remove_callbacks(p)
  p:set_visible(false)
  if texture_release then p:remove_texture() end
end

ffi.metatype("pSprite", Mt_Sprite)


-- pSpriteText
local Mt_SpriteText = setmetatable({}, {__index = Mt_Sprite})
Mt_SpriteText.__index             = Mt_SpriteText
Mt_SpriteText.change_text         = C.SpriteText_change_text
Mt_SpriteText.set_pos             = C.SpriteText_set_pos
Mt_SpriteText.set_scale           = C.SpriteText_set_scale
Mt_SpriteText.set_depth           = C.SpriteText_set_depth
Mt_SpriteText.set_color           = C.SpriteText_set_color
Mt_SpriteText.set_red             = C.SpriteText_set_red
Mt_SpriteText.set_green           = C.SpriteText_set_green
Mt_SpriteText.set_blue            = C.SpriteText_set_blue
Mt_SpriteText.set_alpha           = C.SpriteText_set_alpha
Mt_SpriteText.set_visible         = C.SpriteText_set_visible
Mt_SpriteText.set_center_aligned  = C.SpriteText_set_center_aligned

Mt_SpriteText.remove              = function(p)
  remove_callbacks(p)
  p:set_visible(false)
end

ffi.metatype("pSpriteText", Mt_SpriteText)

-- Extended (or for extending) metatables will be exported

local Mt_Sprite_Ex     = copy_cdata_mt(Mt_Sprite)

Mt_Sprite_Ex.set_fade = function(self, setting)
  self:set_visible(true)
  self:tween("Linear", "Alpha", setting.s or 255, setting.e or 0, 500, 0, setting.cb or nil, 0)
end

local Mt_SpriteText_Ex = copy_cdata_mt(Mt_SpriteText, Mt_Sprite_Ex)

-- Constructors & Finalizers

local function __finalizer__(actual_finalizer)
  return function(self)
    remove_callbacks(self)
    actual_finalizer(self)
  end
end

local sprite_dtor_      = __finalizer__(C.Sprite__gc)
local sprite_text_dtor_ = __finalizer__(C.SpriteText__gc)

local function new_sprite(name, parent, w, h, center)
  w = w or -1
  h = h or -1
  center = center or false
  return ffi.gc(C.Sprite_create(name, ffi.cast("pObject*", parent), w, h, center), sprite_dtor_)
end

--

local function new_sprite_text(text, parent, font, size, center, r, g, b)
  return ffi.gc(C.SpriteText_create(text, ffi.cast("pObject*", parent), font, size, center, r, g, b), sprite_text_dtor_)
end

------------------------------------------------------------------------

local function GET_SCREEN_W()
  return C.Get_SCREEN_W()
end
local function GET_SCREEN_H()
  return C.Get_SCREEN_H()
end

--
local Input1      = C.Input_get_input1()
local Input2      = C.Input_get_input2()
local Input1_left = C.Input_get_trig1(C.Input_get_input1())
local Input2_left = C.Input_get_trig1(C.Input_get_input2())
local Input1_right= C.Input_get_trig2(C.Input_get_input1())
local Input2_right= C.Input_get_trig2(C.Input_get_input2())

----------------------------------------------------------------------------
-- Main functions
----------------------------------------------------------------------------
return {
  new_sprite        = new_sprite,
  new_sprite_text   = new_sprite_text,
  Mt_Sprite_Ex      = Mt_Sprite_Ex,
  Mt_SpriteText_Ex  = Mt_SpriteText_Ex,
  --
  GET_SCREEN_W      = GET_SCREEN_W,
  GET_SCREEN_H      = GET_SCREEN_H,
  --
  Input1            = Input1,
  Input2            = Input2,
  Input1_left       = Input1_left,
  Input2_left       = Input2_left,
  Input1_right      = Input1_right,
  Input2_right      = Input2_right,

  debug_hack        = debug_hack
}
