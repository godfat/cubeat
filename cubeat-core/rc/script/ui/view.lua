local ffi       = require 'ffi'
local C         = ffi.C
local helper    = require 'rc/script/helper'
local basepath  = helper.basepath
local copy_cdata_mt = helper.copy_cdata_mt

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

local weakkey = {__mode = "k"}

-- Ok, the problem here is we now have to different signature for callbacks,
-- But, ffi.cast to C functions seemed to have the magic to treat them all the same!
-- I assume there's another internal check for argument amounts.... 
-- so I don't care about PSC_CALLBACK_WITH_PARAM now

local CallbackT            = ffi.typeof("PSC_OBJCALLBACK")
local Callback_with_paramT = ffi.typeof("PSC_OBJCALLBACK_WITH_PARA")

local function _tracked_cb(btn_table, T, b, func)
  if btn_table[b] == nil then
    btn_table[b] = ffi.cast(T, func)
  else
    btn_table[b]:set(func)
  end
end

local function tracked_cb(cb_table, T, obj, btn, func)
  if cb_table[obj] == nil then
    cb_table[obj] = {}
  end
  _tracked_cb(cb_table[obj], T, btn, func)
  return cb_table[obj][btn]
end

local function tracked_cb_removal(cb_table, obj)
  if cb_table[obj] ~= nil then
    for _, v1 in pairs(cb_table[obj]) do
      io.write("callback collected.\n")
      v1:free()
    end
  end
end

local __on_press__   = setmetatable({}, weakkey) -- use object (cdata) as the weak key
local __on_release__ = setmetatable({}, weakkey) -- use object (cdata) as the weak key
local __on_down__    = setmetatable({}, weakkey) -- use object (cdata) as the weak key
local __on_up__      = setmetatable({}, weakkey) -- use object (cdata) as the weak key
local __on_enter_focus__ = setmetatable({}, weakkey) -- use object (cdata) as the weak key
local __on_leave_focus__ = setmetatable({}, weakkey) -- use object (cdata) as the weak key

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
Mt_Sprite.set_size                = C.Sprite_set_size
Mt_Sprite.set_depth               = C.Sprite_set_depth
Mt_Sprite.set_color               = C.Sprite_set_color
Mt_Sprite.set_red                 = C.Sprite_set_red
Mt_Sprite.set_green               = C.Sprite_set_green
Mt_Sprite.set_blue                = C.Sprite_set_blue
Mt_Sprite.set_alpha               = C.Sprite_set_alpha
Mt_Sprite.set_visible             = C.Sprite_set_visible
Mt_Sprite.set_center_aligned      = C.Sprite_set_center_aligned
Mt_Sprite.tween_elastic_pos       = function(self, s, e, dur, l, cb, d) 
  C.Sprite_tween_elastic_pos(self, s, e, dur, l or 0, cb or nil, d or 0)
end
Mt_Sprite.tween_isine_pos         = function(self, s, e, dur, l, cb, d) 
  C.Sprite_tween_isine_pos(self, s, e, dur, l or 0, cb or nil, d or 0)
end
Mt_Sprite.tween_osine_pos         = function(self, s, e, dur, l, cb, d) 
  C.Sprite_tween_osine_pos(self, s, e, dur, l or 0, cb or nil, d or 0)
end
Mt_Sprite.tween_linear_alpha      = function(self, s, e, dur, l, cb, d)
  C.Sprite_tween_linear_alpha(self, s, e, dur, l or 0, cb or nil, d or 0)
end
Mt_Sprite.tween                   = function(self, Eq, Accessor, s, e, dur, l, cb, d)
  if type(s)=='number' and type(e)=='number' then
    C.Sprite_tween_1d(self, Eq, Accessor, s, e, dur, l or 0, cb or nil, d or 0)
  else
    C.Sprite_tween(self, Eq, Accessor, s, e, dur, l or 0, cb or nil, d or 0)
  end
end
Mt_Sprite.texture_flipH           = C.Sprite_texture_flipH
Mt_Sprite.texture_flipV           = C.Sprite_texture_flipV
-- Mt_Sprite.on_release              = function(p, b, func) C.Sprite_on_release(ffi.cast("pSprite*", p), b, func) end
-- Mt_Sprite.on_press                = function(p, b, func) C.Sprite_on_press(ffi.cast("pSprite*", p), b, func) end
-- Mt_Sprite.on_up                   = function(p, b, func) C.Sprite_on_up(ffi.cast("pSprite*", p), b, func) end
-- Mt_Sprite.on_down                 = function(p, b, func) C.Sprite_on_down(ffi.cast("pSprite*", p), b, func) end
-- Mt_Sprite.on_enter_focus          = function(p, input, func) C.Sprite_on_enter_focus(ffi.cast("pSprite*", p), input, func) end
-- Mt_Sprite.on_leave_focus          = function(p, input, func) C.Sprite_on_leave_focus(ffi.cast("pSprite*", p), input, func) end
Mt_Sprite.get_pos_x               = C.Sprite_get_pos_x
Mt_Sprite.get_pos_y               = C.Sprite_get_pos_y
Mt_Sprite.get_size_x              = C.Sprite_get_size_x
Mt_Sprite.get_size_y              = C.Sprite_get_size_y
Mt_Sprite.get_screen_pos_x        = C.Sprite_get_screen_pos_x
Mt_Sprite.get_screen_pos_y        = C.Sprite_get_screen_pos_y

Mt_Sprite.on_release              = function(p, b, func) 
  C.Sprite_on_release(ffi.cast("pSprite*", p), b, tracked_cb(__on_release__, CallbackT, p, b, func)) 
end
 
Mt_Sprite.on_press                = function(p, b, func) 
  C.Sprite_on_press(ffi.cast("pSprite*", p), b, tracked_cb(__on_press__, CallbackT, p, b, func)) 
end

Mt_Sprite.on_up                   = function(p, b, func) 
  C.Sprite_on_up(ffi.cast("pSprite*", p), b, tracked_cb(__on_up__, CallbackT, p, b, func)) 
end

Mt_Sprite.on_down                 = function(p, b, func) 
  C.Sprite_on_down(ffi.cast("pSprite*", p), b, tracked_cb(__on_down__, CallbackT, p, b, func)) 
end

Mt_Sprite.on_enter_focus          = function(p, input, func) 
  C.Sprite_on_enter_focus(ffi.cast("pSprite*", p), input, tracked_cb(__on_enter_focus__, Callback_with_paramT, p, input, func)) 
end

Mt_Sprite.on_leave_focus          = function(p, input, func) 
  C.Sprite_on_leave_focus(ffi.cast("pSprite*", p), input, tracked_cb(__on_leave_focus__, Callback_with_paramT, p, input, func)) 
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
Mt_SpriteText.tween_linear_alpha  = C.SpriteText_tween_linear_alpha

ffi.metatype("pSpriteText", Mt_SpriteText)

-- Extended (or for extending) metatables will be exported

local Mt_Sprite_Ex     = copy_cdata_mt(Mt_Sprite)

Mt_Sprite_Ex.set_fade = function(self, setting)
  self:set_visible(true)
  self:tween_linear_alpha(setting.s or 255, setting.e or 0, 500, 0, setting.cb or nil, 0)
end

local Mt_SpriteText_Ex = copy_cdata_mt(Mt_SpriteText, Mt_Sprite_Ex)

-- Constructors & Finalizers

local function __finalizer__(actual_finalizer)
  return function(self)
    tracked_cb_removal(__on_press__, self)
    tracked_cb_removal(__on_release__, self)
    tracked_cb_removal(__on_down__, self)
    tracked_cb_removal(__on_up__, self)
    tracked_cb_removal(__on_enter_focus__, self)
    tracked_cb_removal(__on_leave_focus__, self)
    actual_finalizer(self)
  end
end

local sprite_dtor_      = __finalizer__(C.Sprite__gc)
local sprite_text_dtor_ = __finalizer__(C.Sprite__gc)

local function new_sprite(name, parent, w, h, center)
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
  Input2_right      = Input2_right
}