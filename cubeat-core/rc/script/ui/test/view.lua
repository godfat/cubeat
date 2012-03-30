
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
Mt_Sprite.get_screen_pos_x        = C.Sprite_get_screen_pos_x
Mt_Sprite.get_screen_pos_y        = C.Sprite_get_screen_pos_y


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
  local button_focus  = function(self) button.title:set_blue(255) end
  local butotn_leave  = function(self) button.title:set_blue(0) end
  button.title:set_depth(-10)
  --
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
                          button:set_visible(true)
                          if alpha == 0 then
                            local tween_cb = function(self) button:set_visible(false) end
                            button.title:on_tween_line_alpha(alpha, 500, 0, tween_cb, 0)
                          else
                            local tween_cb = function(self) end
                            button.title:on_tween_line_alpha(alpha, 500, 0, tween_cb, 0)
                          end
                        end
  button.on_press     = function(self, func)
                          button.title:on_press( C.Input_get_trig1(C.Input_get_input1()), func )
                        end
  return button
end

local function new_ui_ratio(ratio, text, sprite)
  ratio.is_pressed  = false
  ratio.icon        = new_sprite_from_sprite("cubes/cube1", sprite, 32, 32, false)
  ratio.title       = new_sprite_text_from_sprite(text, sprite, "Star Jedi", 24, false, 255, 255, 0)
  ratio.debug_text  = new_sprite_text_from_sprite("FALSE", sprite, "Star Jedi", 24, false, 100, 100, 255)
  ratio.icon:set_depth(-10)
  ratio.title:set_depth(-10)
  --
  local ratio_focus = function(self) ratio.title:set_blue(255) end
  local ratio_leave = function(self) ratio.title:set_blue(0) end
  ratio.title:on_enter_focus( C.Input_get_input1(), ratio_focus )
  ratio.title:on_leave_focus( C.Input_get_input1(), ratio_leave )
  --
  local ratio_press = function(self)
                        if ratio.is_pressed == false then
                          ratio.icon:set_texture("cubes/cube-b-1")
                          ratio.is_pressed = true
                        else
                          ratio.icon:set_texture("cubes/cube1")
                          ratio.is_pressed = false
                        end
                        ratio.debug_text:change_text(tostring(ratio.is_pressed))
                      end
  ratio.icon:on_press( C.Input_get_trig1(C.Input_get_input1()), ratio_press )
  ratio.title:on_press( C.Input_get_trig1(C.Input_get_input1()), ratio_press )
  --
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
                        ratio:set_visible(true)
                        if alpha == 0 then
                          local tween_cb_icon = function(self) ratio.icon:set_visible(false) end
                          local tween_cb_title= function(self) ratio.icon:set_visible(false) end
                          local tween_cb_debug= function(self) ratio.icon:set_visible(false) end
                          ratio.icon:on_tween_line_alpha(alpha, 500, 0, tween_cb_icon, 0)
                          ratio.title:on_tween_line_alpha(alpha, 500, 0, tween_cb_title, 0)
                          ratio.debug_text:on_tween_line_alpha(alpha, 500, 0, tween_cb_debug, 0)
                        else
                          local tween_cb = function(self) end
                          ratio.icon:on_tween_line_alpha(alpha, 500, 0, tween_cb, 0)
                          ratio.title:on_tween_line_alpha(alpha, 500, 0, tween_cb, 0)
                          ratio.debug_text:on_tween_line_alpha(alpha, 500, 0, tween_cb, 0)
                        end
                      end
  ratio.on_press    = function(self, func)
                        local callback  = function(self)
                                            if ratio.is_pressed == false then
                                              ratio.icon:set_texture("cubes/cube-b-1")
                                              ratio.is_pressed = true
                                            else
                                              ratio.icon:set_texture("cubes/cube1")
                                              ratio.is_pressed = false
                                            end
                                            ratio.debug_text:change_text(tostring(ratio.is_pressed))
                                            func(self)
                                          end
                        ratio.icon:on_press( C.Input_get_trig1(C.Input_get_input1()), callback )
                        ratio.title:on_press( C.Input_get_trig1(C.Input_get_input1()), callback )
                      end
  return ratio
end

local function new_ui_selectbox(box, sprite, tb)
  box.index     = 1
  box.title_tb  = tb
  box.left      = new_sprite_from_sprite("cubes/cube-b-1", sprite, 32, 32, false)
  box.right     = new_sprite_from_sprite("cubes/cube-b-1", sprite, 32, 32, false)
  box.title     = new_sprite_text_from_sprite(box.title_tb[box.index], sprite, "Star Jedi", 24, true, 255, 255, 0)
  box.debug_text= new_sprite_text_from_sprite(tostring(box.index), sprite, "Star Jedi", 24, true, 100, 100, 255)
  box.left:set_depth(-10)
  box.right:set_depth(-10)
  --
  local left_focus  = function(self) box.left:set_blue(0) end
  local left_leave  = function(self) box.left:set_blue(255) end
  local right_focus = function(self) box.right:set_blue(0) end
  local right_leave = function(self) box.right:set_blue(255) end
  box.left:on_enter_focus( C.Input_get_input1(), left_focus )
  box.left:on_leave_focus( C.Input_get_input1(), left_leave )
  box.right:on_enter_focus( C.Input_get_input1(), right_focus )
  box.right:on_leave_focus( C.Input_get_input1(), right_leave )
  --
  local left_press  = function(self)
                        box.index = box.index - 1
                        if box.index < 1 then box.index = table.getn(box.title_tb) end
                        box.title:change_text(box.title_tb[box.index])
                        box.debug_text:change_text(tostring(box.index))
                      end
  box.left:on_press( C.Input_get_trig1(C.Input_get_input1()), left_press )
  --
  local right_press = function(self)
                        box.index = box.index + 1
                        if box.index > table.getn(box.title_tb) then box.index = 1 end
                        box.title:change_text(box.title_tb[box.index])
                        box.debug_text:change_text(tostring(box.index))
                      end
  box.right:on_press( C.Input_get_trig1(C.Input_get_input1()), right_press )
  --
  box.set_pos       = function(self, posx, posy)
                        box.left:set_pos(posx, posy)
                        box.right:set_pos(posx+280, posy)
                        box.title:set_pos(posx+150, posy+10)
                        box.debug_text:set_pos(posx+370, posy+10)
                      end
  box.set_visible   = function(self, visible)
                        box.left:set_visible(visible)
                        box.right:set_visible(visible)
                        box.title:set_visible(visible)
                        box.debug_text:set_visible(visible)
                      end
  box.set_alpha     = function(self, alpha)
                        box.left:set_alpha(alpha)
                        box.right:set_alpha(alpha)
                        box.title:set_alpha(alpha)
                        box.debug_text:set_alpha(alpha)
                      end
  box.set_fade      = function(self, alpha)
                        box:set_visible(true)
                        if alpha == 0 then
                          local tween_cb_left   = function(self) box.left:set_visible(false) end
                          local tween_cb_right  = function(self) box.right:set_visible(false) end
                          local tween_cb_title  = function(self) box.title:set_visible(false) end
                          local tween_cb_debug  = function(self) box.debug_text:set_visible(false) end
                          box.left:on_tween_line_alpha(alpha, 500, 0, tween_cb_left, 0)
                          box.right:on_tween_line_alpha(alpha, 500, 0, tween_cb_right, 0)
                          box.title:on_tween_line_alpha(alpha, 500, 0, tween_cb_title, 0)
                          box.debug_text:on_tween_line_alpha(alpha, 500, 0, tween_cb_debug, 0)
                        else
                          local tween_cb = function(self) end
                          box.left:on_tween_line_alpha(alpha, 500, 0, tween_cb, 0)
                          box.right:on_tween_line_alpha(alpha, 500, 0, tween_cb, 0)
                          box.title:on_tween_line_alpha(alpha, 500, 0, tween_cb, 0)
                          box.debug_text:on_tween_line_alpha(alpha, 500, 0, tween_cb, 0)
                        end
                      end
  box.on_press_left = function(self, func)
                        local callback  = function(self)
                                            box.index = box.index - 1
                                            if box.index < 1 then box.index = table.getn(box.title_tb) end
                                            box.title:change_text(box.title_tb[box.index])
                                            box.debug_text:change_text(tostring(box.index))
                                            func(self)
                                          end
                        box.left:on_press( C.Input_get_trig1(C.Input_get_input1()), callback )
                      end
  box.on_press_right= function(self, func)
                        local callback  = function(self)
                                            box.index = box.index + 1
                                            if box.index > table.getn(box.title_tb) then box.index = 1 end
                                            box.title:change_text(box.title_tb[box.index])
                                            box.debug_text:change_text(tostring(box.index))
                                            func(self)
                                          end
                        box.right:on_press( C.Input_get_trig1(C.Input_get_input1()), callback )
                      end
  return box
end

local function new_ui_scrollbar(scrollbar, sprite, range)
  scrollbar.parent      = sprite
  scrollbar.range       = range
  scrollbar.index       = 0
  scrollbar.is_focus    = false
  scrollbar.line        = new_sprite_from_sprite("cubes/cube1", sprite, 256, 16, false)
  scrollbar.button      = new_sprite_from_sprite("cubes/cube-b-1", sprite, 32, 32, false)
  scrollbar.title       = new_sprite_text_from_sprite("0", sprite, "Star Jedi", 24, true, 255, 255, 0)
  scrollbar.debug_text  = new_sprite_text_from_sprite("off", sprite, "Star Jedi", 24, true, 100, 100, 255)
  scrollbar.line:set_depth(-50)
  scrollbar.button:set_depth(-100)
  --
  local scrollbar_button_focus    = function(self)
                                      scrollbar.button:set_blue(0)
                                      scrollbar.is_focus = true
                                      scrollbar.debug_text:change_text("on")
                                    end
  local scrollbar_button_leave    = function(self)
                                      scrollbar.button:set_blue(255)
                                      scrollbar.is_focus = false
                                      scrollbar.debug_text:change_text("off")
                                    end
  scrollbar.button:on_enter_focus( C.Input_get_input1(), scrollbar_button_focus )
  scrollbar.button:on_leave_focus( C.Input_get_input1(), scrollbar_button_leave )
  --
  local scrollbar_button_down = function(self)
    if scrollbar.is_focus == true then
      local pos_x = C.Input_get_cursor_x(C.Input_get_input1()) - scrollbar.parent:get_screen_pos_x() - (scrollbar.button:get_size_x()/2)
      local pos_y = scrollbar.button:get_pos_y()
      local bg_left = scrollbar.line:get_pos_x()
      local bg_right= bg_left + scrollbar.line:get_size_x() - scrollbar.button:get_size_x()
      if pos_x < bg_left then pos_x = bg_left end
      if pos_x > bg_right then pos_x = bg_right end
      scrollbar.button:set_pos(pos_x, pos_y)
      --
      scrollbar.index = math.floor( (pos_x-bg_left)*scrollbar.range/(scrollbar.line:get_size_x()-scrollbar.button:get_size_x()) )
      scrollbar.title:change_text(tostring(scrollbar.index))
    end
  end
  scrollbar.button:on_down( C.Input_get_trig1(C.Input_get_input1()), scrollbar_button_down )
  --
  local scrollbar_line_press = function(self)
    local pos_x = C.Input_get_cursor_x(C.Input_get_input1()) - scrollbar.parent:get_screen_pos_x() - (scrollbar.button:get_size_x()/2)
    local pos_y = scrollbar.button:get_pos_y()
    local bg_left = scrollbar.line:get_pos_x()
    local bg_right= bg_left + scrollbar.line:get_size_x() - scrollbar.button:get_size_x()
    if pos_x < bg_left then pos_x = bg_left end
    if pos_x > bg_right then pos_x = bg_right end
    scrollbar.button:set_pos(pos_x, pos_y)
    --
    scrollbar.index = math.floor( (pos_x-bg_left)*scrollbar.range/(scrollbar.line:get_size_x()-scrollbar.button:get_size_x()) )
    scrollbar.title:change_text(tostring(scrollbar.index))
  end
  scrollbar.line:on_press( C.Input_get_trig1(C.Input_get_input1()), scrollbar_line_press )
  --
  scrollbar.set_pos     = function(self, posx, posy)
                            scrollbar.line:set_pos(posx, posy+10)
                            scrollbar.button:set_pos(posx, posy)
                            scrollbar.title:set_pos(posx+300, posy+10)
                            scrollbar.debug_text:set_pos(posx+370, posy+10)
                          end
  scrollbar.set_visible = function(self, visible)
                            scrollbar.line:set_visible(visible)
                            scrollbar.button:set_visible(visible)
                            scrollbar.title:set_visible(visible)
                            scrollbar.debug_text:set_visible(visible)
                          end
  scrollbar.set_alpha   = function(self, alpha)
                            scrollbar.line:set_alpha(alpha)
                            scrollbar.button:set_alpha(alpha)
                            scrollbar.title:set_alpha(alpha)
                            scrollbar.debug_text:set_alpha(alpha)
                          end
  scrollbar.set_fade    = function(self, alpha)
                            scrollbar:set_visible(true)
                            if alpha == 0 then
                              local tween_cb_line   = function(self) scrollbar.line:set_visible(false) end
                              local tween_cb_button = function(self) scrollbar.button:set_visible(false) end
                              local tween_cb_title  = function(self) scrollbar.title:set_visible(false) end
                              local tween_cb_debug  = function(self) scrollbar.debug_text:set_visible(false) end
                              scrollbar.line:on_tween_line_alpha(alpha, 500, 0, tween_cb_line, 0)
                              scrollbar.button:on_tween_line_alpha(alpha, 500, 0, tween_cb_button, 0)
                              scrollbar.title:on_tween_line_alpha(alpha, 500, 0, tween_cb_title, 0)
                              scrollbar.debug_text:on_tween_line_alpha(alpha, 500, 0, tween_cb_debug, 0)

                            else
                              local tween_cb = function(self) end
                              scrollbar.line:on_tween_line_alpha(alpha, 500, 0, tween_cb, 0)
                              scrollbar.button:on_tween_line_alpha(alpha, 500, 0, tween_cb, 0)
                              scrollbar.title:on_tween_line_alpha(alpha, 500, 0, tween_cb, 0)
                              scrollbar.debug_text:on_tween_line_alpha(alpha, 500, 0, tween_cb, 0)
                            end
                          end
  scrollbar.on_press    = function(self, func)
                            local callback = function(self)
                              if scrollbar.is_focus == true then
                                local pos_x = C.Input_get_cursor_x(C.Input_get_input1()) - scrollbar.parent:get_screen_pos_x() - (scrollbar.button:get_size_x()/2)
                                local pos_y = scrollbar.button:get_pos_y()
                                local bg_left = scrollbar.line:get_pos_x()
                                local bg_right= bg_left + scrollbar.line:get_size_x() - scrollbar.button:get_size_x()
                                if pos_x < bg_left then pos_x = bg_left end
                                if pos_x > bg_right then pos_x = bg_right end
                                scrollbar.button:set_pos(pos_x, pos_y)
                                --
                                scrollbar.index = math.floor( (pos_x-bg_left)*scrollbar.range/(scrollbar.line:get_size_x()-scrollbar.button:get_size_x()) )
                                scrollbar.title:change_text(tostring(scrollbar.index))
                              end
                              func(self)
                            end
                            scrollbar.button:on_down( C.Input_get_trig1(C.Input_get_input1()), callback )
                          end
  return scrollbar
end

return {
  new_sprite                  = new_sprite,
  new_sprite_from_sprite      = new_sprite_from_sprite,
  new_sprite_text             = new_sprite_text,
  new_sprite_text_from_sprite = new_sprite_text_from_sprite,
  new_ui_button               = new_ui_button,
  new_ui_ratio                = new_ui_ratio,
  new_ui_selectbox            = new_ui_selectbox,
  new_ui_scrollbar            = new_ui_scrollbar
}
