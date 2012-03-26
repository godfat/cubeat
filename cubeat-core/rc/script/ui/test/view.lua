
local ffi      = require 'ffi'
local helper   = require 'rc/script/helper'
local basepath = helper.basepath
local C        = ffi.C

ffi.cdef[[
typedef struct TestUI TestUI;
typedef struct pScene pScene;
typedef struct pSprite pSprite;
typedef struct pSpriteText pSpriteText;
typedef struct Input Input;
typedef struct Button Button;
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
Mt_Sprite.set_depth               = C.Sprite_set_depth
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

Mt_Sprite.get_pos_x               = C.Sprite_get_pos_x
Mt_Sprite.get_pos_y               = C.Sprite_get_pos_y
Mt_Sprite.get_size_x              = C.Sprite_get_size_x
Mt_Sprite.get_size_y              = C.Sprite_get_size_y

-- local weakkey = {__mode = "k"}
-- Mt_Sprite.__on_releases__ = setmetatable({}, weakkey)
-- Mt_Sprite.__on_presses__  = setmetatable({}, weakkey)
-- Mt_Sprite.__on_downs__    = setmetatable({}, weakkey)
-- Mt_Sprite.__on_ups__      = setmetatable({}, weakkey)

-- Mt_Sprite.on_release = function(self, btn, func)
  -- C.Sprite_on_release(self, tracked_cb(Mt_Sprite.__on_releases__, self, btn, func))
-- end

Mt_Sprite.on_release              = C.Sprite_on_release
Mt_Sprite.on_press                = C.Sprite_on_press
Mt_Sprite.on_up                   = C.Sprite_on_up
Mt_Sprite.on_down                 = C.Sprite_on_down
Mt_Sprite.on_enter_focus          = C.Sprite_on_enter_focus
Mt_Sprite.on_leave_focus          = C.Sprite_on_leave_focus

Mt_Sprite.on_tween_line_pos           = C.Sprite_on_tween_line_pos
Mt_Sprite.on_tween_line_rotation      = C.Sprite_on_tween_line_rotation
Mt_Sprite.on_tween_line_scale         = C.Sprite_on_tween_line_scale
Mt_Sprite.on_tween_line_color_diffuse = C.Sprite_on_tween_line_color_diffuse
Mt_Sprite.on_tween_line_red           = C.Sprite_on_tween_line_red
Mt_Sprite.on_tween_line_green         = C.Sprite_on_tween_line_green
Mt_Sprite.on_tween_line_blue          = C.Sprite_on_tween_line_blue
Mt_Sprite.on_tween_line_alpha         = C.Sprite_on_tween_line_alpha

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

local function new_sprite_from_sprite(name, sprite, w, h, center)
  return ffi.gc(C.Sprite_create_from_sprite(name, sprite, w, h, center), C.Sprite__gc)
end

local Mt_SpriteText = {}
Mt_SpriteText.__index             = Mt_SpriteText
Mt_SpriteText.set_center_aligned  = C.SpriteText_set_center_aligned
Mt_SpriteText.set_depth           = C.SpriteText_set_depth
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
Mt_SpriteText.on_up               = C.SpriteText_on_up
Mt_SpriteText.on_down             = C.SpriteText_on_down
Mt_SpriteText.on_enter_focus      = C.SpriteText_on_enter_focus
Mt_SpriteText.on_leave_focus      = C.SpriteText_on_leave_focus

Mt_SpriteText.on_tween_line_alpha = C.SpriteText_on_tween_line_alpha

ffi.metatype("pSpriteText", Mt_SpriteText)

local function new_sprite_text(text, scene, font, size, center, r, g, b)
  return ffi.gc(C.SpriteText_create(text, scene, font, size, center, r, g, b), C.SpriteText__gc)
end

local function new_sprite_text_from_sprite(text, sprite, font, size, center, r, g, b)
  return ffi.gc(C.SpriteText_create_from_sprite(text, sprite, font, size, center, r, g, b), C.SpriteText__gc)
end

local function new_ui_button(button, text, sprite)
  button.title        = new_sprite_text_from_sprite(text, sprite, "Star Jedi", 24, false, 255, 255, 0)
  button.title:set_depth(-10)
  local button_focus  = function(self) button.title:set_blue(255) end
  local butotn_leave  = function(self) button.title:set_blue(0) end
  button.title:on_enter_focus( C.Input_get_input1(), button_focus )
  button.title:on_leave_focus( C.Input_get_input1(), butotn_leave )
  --
  button.set_pos      = function(self, posx, posy)
                          button.title:set_pos(posx, posy)
                        end
  button.set_visible  = function(self, visible)
                          button.title:set_visible(visible)
                        end
  button.set_alpha    = function(self, alpha)
                          button.title:set_alpha(alpha)
                        end
  button.set_fade     = function(self, alpha)
                          local tween_cb =  function(self) end
                          button.title:on_tween_line_alpha(alpha, 500, 0, tween_cb, 0)
                        end
  button.on_press     = function(self, func)
                          button.title:on_press( C.Input_get_trig1(C.Input_get_input1()), func )
                        end
  return button
end

local function new_ui_ratio(ratio, text, sprite)
  ratio.is_down     = false
  ratio.icon        = new_sprite_from_sprite("cubes/cube1", sprite, 32, 32, false)
  ratio.title       = new_sprite_text_from_sprite(text, sprite, "Star Jedi", 24, false, 255, 255, 0)
  ratio.debug_text  = new_sprite_text_from_sprite("FALSE", sprite, "Star Jedi", 24, false, 100, 100, 255)
  local ratio_focus = function(self) ratio.title:set_blue(255) end
  local ratio_leave = function(self) ratio.title:set_blue(0) end
  ratio.title:on_enter_focus( C.Input_get_input1(), ratio_focus )
  ratio.title:on_leave_focus( C.Input_get_input1(), ratio_leave )
  ratio.icon:set_depth(-10)
  ratio.title:set_depth(-10)
  ratio.set_pos     = function(self, posx, posy)
                        ratio.icon:set_pos(posx, posy)
                        ratio.title:set_pos(posx+50, posy)
                        ratio.debug_text:set_pos(posx+330, posy)
                      end
  ratio.set_visible = function(self, visible)
                        ratio.icon:set_visible(visible)
                        ratio.title:set_visible(visible)
                        ratio.debug_text:set_visible(visible)
                      end
  ratio.set_alpha   = function(self, alpha)
                        ratio.icon:set_alpha(alpha)
                        ratio.title:set_alpha(alpha)
                        ratio.debug_text:set_alpha(alpha)
                      end
  ratio.set_fade    = function(self, alpha)
                        local tween_cb =  function(self) end
                        ratio.icon:on_tween_line_alpha(alpha, 500, 0, tween_cb, 0)
                        ratio.title:on_tween_line_alpha(alpha, 500, 0, tween_cb, 0)
                        ratio.debug_text:on_tween_line_alpha(alpha, 500, 0, tween_cb, 0)
                      end
  ratio.on_press    = function(self, func)
                        local callback  = function(self)
                                            if ratio.is_down == false then
                                              ratio.icon:set_texture("cubes/cube-b-1")
                                              ratio.is_down = true
                                            else
                                              ratio.icon:set_texture("cubes/cube1")
                                              ratio.is_down = false
                                            end
                                            ratio.debug_text:change_text(tostring(ratio.is_down))
                                            func(self)
                                          end
                        ratio.icon:on_press( C.Input_get_trig1(C.Input_get_input1()), callback )
                        ratio.title:on_press( C.Input_get_trig1(C.Input_get_input1()), callback )
                      end
  return ratio
end

return {
  new_sprite                  = new_sprite,
  new_sprite_from_sprite      = new_sprite_from_sprite,
  new_sprite_text             = new_sprite_text,
  new_sprite_text_from_sprite = new_sprite_text_from_sprite,
  new_ui_button               = new_ui_button,
  new_ui_ratio                = new_ui_ratio
}
