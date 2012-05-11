local ffi       = require 'ffi'
local C         = ffi.C
local helper    = require 'rc/script/helper'
local basepath  = helper.basepath

ffi.cdef[[
typedef struct pObject pObject;
typedef struct pScene pScene;
typedef struct pSprite pSprite;
typedef struct pSpriteText pSpriteText;
typedef struct Input Input;
typedef struct Button Button;
]]
ffi.cdef( io.open( basepath().."rc/script/ui/bindings.ffi", 'r'):read('*a') )

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
Mt_Sprite.set_size                = C.Sprite_set_size
Mt_Sprite.set_depth               = C.Sprite_set_depth
Mt_Sprite.set_alpha               = C.Sprite_set_alpha
Mt_Sprite.set_visible             = C.Sprite_set_visible
Mt_Sprite.set_center_aligned      = C.Sprite_set_center_aligned
Mt_Sprite.on_tween_line_alpha     = C.Sprite_on_tween_line_alpha
Mt_Sprite.on_release              = function(p, b, cb) C.Sprite_on_release(ffi.cast("pSprite*", p), b, cb) end
Mt_Sprite.on_press                = function(p, b, cb) C.Sprite_on_press(ffi.cast("pSprite*", p), b, cb) end

ffi.metatype("pSprite", Mt_Sprite)


-- pSpriteText
local Mt_SpriteText = setmetatable({}, {__index = Mt_Sprite})
Mt_SpriteText.__index             = Mt_SpriteText
Mt_SpriteText.set_pos             = C.SpriteText_set_pos
Mt_SpriteText.set_depth           = C.SpriteText_set_depth
Mt_SpriteText.set_alpha           = C.SpriteText_set_alpha
Mt_SpriteText.set_visible         = C.SpriteText_set_visible
Mt_SpriteText.set_center_aligned  = C.SpriteText_set_center_aligned
Mt_SpriteText.on_tween_line_alpha = C.SpriteText_on_tween_line_alpha

ffi.metatype("pSpriteText", Mt_SpriteText)


--
local function new_sprite(name, parent, w, h, center)
  return ffi.gc(C.Sprite_create(name, ffi.cast("pObject*", parent), w, h, center), C.Sprite__gc)
end


--
local function new_sprite_text(text, parent, font, size, center, r, g, b)
  return ffi.gc(C.SpriteText_create(text, ffi.cast("pObject*",parent), font, size, center, r, g, b), C.SpriteText__gc)
end

----------------------------------------------------------------------------
-- Main functions
----------------------------------------------------------------------------
return {
  new_sprite        = new_sprite,
  new_sprite_text   = new_sprite_text
}