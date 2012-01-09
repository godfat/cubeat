
local ffi      = require 'ffi'
local helper   = require 'rc/script/helper'
local basepath = helper.basepath
local C        = ffi.C

ffi.cdef[[
typedef struct TestUI TestUI;
typedef struct pScene pScene;
typedef struct pSprite pSprite;
typedef struct pSpriteText pSpriteText;
typedef struct InputButton InputButton;
]]
ffi.cdef( io.open( basepath().."rc/script/ui/test/bindings.ffi", 'r'):read('*a') )

------------- Some utils rigged for ffi callback management ---------------
--[[ Important notes:
  I am just going to let ffi callbacks' resources go loose for now, 
  Since it is mainly for UI use only, it will not duplicate like in-game
  objects and keep creating new callbacks; moreover, lua_close(L) surely 
  will reclaim everything that was in Lua. So I decide this is not worth 
  it.

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
--]]

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

-- local weakkey = {__mode = "k"}
-- Mt_Sprite.__on_releases__ = setmetatable({}, weakkey)
-- Mt_Sprite.__on_presses__  = setmetatable({}, weakkey)
-- Mt_Sprite.__on_downs__    = setmetatable({}, weakkey)
-- Mt_Sprite.__on_ups__      = setmetatable({}, weakkey)

-- Mt_Sprite.on_release = function(self, btn, func)
  -- C.Sprite_on_release(self, tracked_cb(Mt_Sprite.__on_releases__, self, btn, func))
-- end

Mt_Sprite.on_release = C.Sprite_on_release
Mt_Sprite.on_press   = C.Sprite_on_press

ffi.metatype("pSprite", Mt_Sprite)

local function new_sprite(name, scene, w, h, center)
  -- return ffi.gc(C.Sprite_create(name, scene, w, h, center), function(self)
    -- tracked_cb_removal (Mt_Sprite.__on_releases__, self)
    -- print '--------'
    -- tracked_cb_removal (Mt_Sprite.__on_presses__, self)
    -- tracked_cb_removal (Mt_Sprite.__on_downs__, self)
    -- tracked_cb_removal (Mt_Sprite.__on_ups__, self)
    -- C.Sprite__gc(self)
  -- end)
  return ffi.gc(C.Sprite_create(name, scene, w, h, center), C.Sprite__gc)
end

local Mt_SpriteText = {}
Mt_SpriteText.__index             = Mt_SpriteText
Mt_SpriteText.set_center_aligned  = C.SpriteText_set_center_aligned
Mt_SpriteText.change_text         = C.SpriteText_change_text
Mt_SpriteText.show_number         = C.SpriteText_show_number
Mt_SpriteText.get_text            = function(self)
  return ffi.string(C.SpriteText_get_text(self))
end
Mt_SpriteText.get_font_size       = C.SpriteText_get_font_size

Mt_SpriteText.set_pos             = C.SpriteText_set_pos
Mt_SpriteText.set_rotate          = C.SpriteText_set_rotate
Mt_SpriteText.set_scale           = C.SpriteText_set_scale
Mt_SpriteText.set_red             = C.SpriteText_set_red
Mt_SpriteText.set_green           = C.SpriteText_set_green
Mt_SpriteText.set_blue            = C.SpriteText_set_blue
Mt_SpriteText.set_alpha           = C.SpriteText_set_alpha
Mt_SpriteText.set_visible         = C.SpriteText_set_visible

Mt_SpriteText.on_release          = C.SpriteText_on_release
Mt_SpriteText.on_press            = C.SpriteText_on_press

ffi.metatype("pSpriteText", Mt_SpriteText)

local function new_sprite_text(text, scene, font, size, center, r, g, b)
  return ffi.gc(C.SpriteText_create(text, scene, font, size, center, r, g, b), C.SpriteText__gc)
end

return {
  new_sprite        = new_sprite,
  new_sprite_text   = new_sprite_text
}
