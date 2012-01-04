
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
Mt_Sprite.__index                 = Mt_Sprite
Mt_Sprite.set_texture             = C.Sprite_set_texture
Mt_Sprite.set_center_aligned      = C.Sprite_set_center_aligned
Mt_Sprite.move_to                 = C.Sprite_move_to
Mt_Sprite.move_tween              = C.Sprite_move_tween
Mt_Sprite.texture_flipH           = C.Sprite_texture_flipH
Mt_Sprite.texture_flipV           = C.Sprite_texture_flipV

Mt_Sprite.set_pos                 = C.Sprite_set_pos
Mt_Sprite.set_rotate              = C.Sprite_set_rotate
Mt_Sprite.set_scale               = C.Sprite_set_scale
Mt_Sprite.set_color_diffuse       = C.Sprite_set_color_diffuse
Mt_Sprite.set_gradient_diffuse    = C.Sprite_set_gradient_diffuse
Mt_Sprite.set_gradient_emissive   = C.Sprite_set_gradient_emissive
Mt_Sprite.set_red                 = C.Sprite_set_red
Mt_Sprite.set_green               = C.Sprite_set_green
Mt_Sprite.set_blue                = C.Sprite_set_blue
Mt_Sprite.set_redE                = C.Sprite_set_redE
Mt_Sprite.set_greenE              = C.Sprite_set_greenE
Mt_Sprite.set_blueE               = C.Sprite_set_blueE
Mt_Sprite.set_alpha               = C.Sprite_set_alpha
Mt_Sprite.set_frame               = C.Sprite_set_frame
Mt_Sprite.set_visible             = C.Sprite_set_visible
Mt_Sprite.set_size                = C.Sprite_set_size

local function newSprite(name, scene, w, h, center)
  return ffi.gc(C.Sprite_create(name, scene, w, h, center), C.Sprite__gc)
end

ffi.metatype("pSprite", Mt_Sprite)

return {
    newSprite = newSprite
}
