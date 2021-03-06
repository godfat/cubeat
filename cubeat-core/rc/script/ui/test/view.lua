
-------------- WARNING !!!!!!!!!!!!!!!!!!!!
-------------- THIS FILE IS OBSOLETE !!!!!!!!!!!!!!!!!!!


local ffi      = require 'ffi'
local helper   = require 'rc/script/helper'
local msgpack  = require 'rc/script/ui/test/luajit-msgpack-pure'
local basepath = helper.basepath
local C        = ffi.C

ffi.cdef[[
typedef struct TestUI TestUI;
typedef struct pObject pObject;
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

-- local Mt_Sprite = {}
-- Mt_Sprite.__index                 = Mt_Sprite
-- Mt_Sprite.set_texture             = C.Sprite_set_texture
-- Mt_Sprite.set_center_aligned      = C.Sprite_set_center_aligned
-- Mt_Sprite.set_depth               = C.Sprite_set_depth
-- Mt_Sprite.move_to                 = C.Sprite_move_to
-- Mt_Sprite.move_tween              = C.Sprite_move_tween
-- Mt_Sprite.texture_flipH           = C.Sprite_texture_flipH
-- Mt_Sprite.texture_flipV           = C.Sprite_texture_flipV

-- Mt_Sprite.set_pos                 = C.Sprite_set_pos
-- Mt_Sprite.set_rotate              = C.Sprite_set_rotate
-- Mt_Sprite.set_scale               = C.Sprite_set_scale
-- Mt_Sprite.set_color_diffuse       = C.Sprite_set_color_diffuse
-- Mt_Sprite.set_gradient_diffuse    = C.Sprite_set_gradient_diffuse
-- Mt_Sprite.set_gradient_emissive   = C.Sprite_set_gradient_emissive
-- Mt_Sprite.set_red                 = C.Sprite_set_red
-- Mt_Sprite.set_green               = C.Sprite_set_green
-- Mt_Sprite.set_blue                = C.Sprite_set_blue
-- Mt_Sprite.set_redE                = C.Sprite_set_redE
-- Mt_Sprite.set_greenE              = C.Sprite_set_greenE
-- Mt_Sprite.set_blueE               = C.Sprite_set_blueE
-- Mt_Sprite.set_alpha               = C.Sprite_set_alpha
-- Mt_Sprite.set_frame               = C.Sprite_set_frame
-- Mt_Sprite.set_visible             = C.Sprite_set_visible
-- Mt_Sprite.set_size                = C.Sprite_set_size

-- Mt_Sprite.get_pos_x               = C.Sprite_get_pos_x
-- Mt_Sprite.get_pos_y               = C.Sprite_get_pos_y
-- Mt_Sprite.get_size_x              = C.Sprite_get_size_x
-- Mt_Sprite.get_size_y              = C.Sprite_get_size_y
-- Mt_Sprite.get_screen_pos_x        = C.Sprite_get_screen_pos_x
-- Mt_Sprite.get_screen_pos_y        = C.Sprite_get_screen_pos_y


-- -- local weakkey = {__mode = "k"}
-- -- Mt_Sprite.__on_releases__ = setmetatable({}, weakkey)
-- -- Mt_Sprite.__on_presses__  = setmetatable({}, weakkey)
-- -- Mt_Sprite.__on_downs__    = setmetatable({}, weakkey)
-- -- Mt_Sprite.__on_ups__      = setmetatable({}, weakkey)

-- -- Mt_Sprite.on_release = function(self, btn, func)
  -- -- C.Sprite_on_release(self, tracked_cb(Mt_Sprite.__on_releases__, self, btn, func))
-- -- end

-- Mt_Sprite.on_release              = function(p, b, cb) C.Sprite_on_release(ffi.cast("pSprite*", p), b, cb) end
-- Mt_Sprite.on_press                = function(p, b, cb) C.Sprite_on_press(ffi.cast("pSprite*", p), b, cb) end
-- Mt_Sprite.on_up                   = function(p, b, cb) C.Sprite_on_up(ffi.cast("pSprite*", p), b, cb) end
-- Mt_Sprite.on_down                 = function(p, b, cb) C.Sprite_on_down(ffi.cast("pSprite*", p), b, cb) end
-- Mt_Sprite.on_enter_focus          = function(p, b, cb) C.Sprite_on_enter_focus(ffi.cast("pSprite*", p), b, cb) end
-- Mt_Sprite.on_leave_focus          = function(p, b, cb) C.Sprite_on_leave_focus(ffi.cast("pSprite*", p), b, cb) end

-- Mt_Sprite.on_tween_line_pos           = C.Sprite_on_tween_line_pos
-- Mt_Sprite.on_tween_line_rotation      = C.Sprite_on_tween_line_rotation
-- Mt_Sprite.on_tween_line_scale         = C.Sprite_on_tween_line_scale
-- Mt_Sprite.on_tween_line_color_diffuse = C.Sprite_on_tween_line_color_diffuse
-- Mt_Sprite.on_tween_line_red           = C.Sprite_on_tween_line_red
-- Mt_Sprite.on_tween_line_green         = C.Sprite_on_tween_line_green
-- Mt_Sprite.on_tween_line_blue          = C.Sprite_on_tween_line_blue
-- Mt_Sprite.on_tween_line_alpha         = C.Sprite_on_tween_line_alpha

-- ffi.metatype("pSprite", Mt_Sprite)

-- local function new_sprite(name, parent, w, h, center)
  -- -- return ffi.gc(C.Sprite_create(name, ffi.cast("pObject*", parent), w, h, center), function(self)
    -- -- tracked_cb_removal (Mt_Sprite.__on_releases__, self)
    -- -- print '--------'
    -- -- tracked_cb_removal (Mt_Sprite.__on_presses__, self)
    -- -- tracked_cb_removal (Mt_Sprite.__on_downs__, self)
    -- -- tracked_cb_removal (Mt_Sprite.__on_ups__, self)
    -- -- C.Sprite__gc(self)
  -- -- end)
  -- return ffi.gc(C.Sprite_create(name, ffi.cast("pObject*", parent), w, h, center), C.Sprite__gc)
-- end

-- --local function new_sprite_from_sprite(name, sprite, w, h, center)
-- --  return ffi.gc(C.Sprite_create_from_sprite(name, sprite, w, h, center), C.Sprite__gc)
-- --end

-- local Mt_SpriteText = setmetatable({}, {__index = Mt_Sprite}) -- jslin: inheritance chain added.

-- Mt_SpriteText.__index             = Mt_SpriteText
-- Mt_SpriteText.set_center_aligned  = C.SpriteText_set_center_aligned
-- Mt_SpriteText.set_depth           = C.SpriteText_set_depth
-- Mt_SpriteText.change_text         = C.SpriteText_change_text
-- Mt_SpriteText.show_number         = C.SpriteText_show_number
-- Mt_SpriteText.get_text            = function(self)
  -- return ffi.string(C.SpriteText_get_text(self))
-- end
-- Mt_SpriteText.get_font_size       = C.SpriteText_get_font_size

-- Mt_SpriteText.set_pos             = C.SpriteText_set_pos
-- Mt_SpriteText.set_rotate          = C.SpriteText_set_rotate
-- Mt_SpriteText.set_scale           = C.SpriteText_set_scale
-- Mt_SpriteText.set_red             = C.SpriteText_set_red
-- Mt_SpriteText.set_green           = C.SpriteText_set_green
-- Mt_SpriteText.set_blue            = C.SpriteText_set_blue
-- Mt_SpriteText.set_alpha           = C.SpriteText_set_alpha
-- Mt_SpriteText.set_visible         = C.SpriteText_set_visible

-- Mt_SpriteText.get_pos_x           = C.SpriteText_get_pos_x
-- Mt_SpriteText.get_pos_y           = C.SpriteText_get_pos_y
-- Mt_SpriteText.get_size_x          = C.SpriteText_get_size_x
-- Mt_SpriteText.get_size_y          = C.SpriteText_get_size_y
-- Mt_SpriteText.get_screen_pos_x    = C.SpriteText_get_screen_pos_x
-- Mt_SpriteText.get_screen_pos_y    = C.SpriteText_get_screen_pos_y

-- Mt_SpriteText.on_tween_line_pos           = C.SpriteText_on_tween_line_pos
-- Mt_SpriteText.on_tween_line_rotation      = C.SpriteText_on_tween_line_rotation
-- Mt_SpriteText.on_tween_line_scale         = C.SpriteText_on_tween_line_scale
-- Mt_SpriteText.on_tween_line_color_diffuse = C.SpriteText_on_tween_line_color_diffuse
-- Mt_SpriteText.on_tween_line_red           = C.SpriteText_on_tween_line_red
-- Mt_SpriteText.on_tween_line_green         = C.SpriteText_on_tween_line_green
-- Mt_SpriteText.on_tween_line_blue          = C.SpriteText_on_tween_line_blue
-- Mt_SpriteText.on_tween_line_alpha         = C.SpriteText_on_tween_line_alpha

-- ffi.metatype("pSpriteText", Mt_SpriteText)

-- local function new_sprite_text(text, parent, font, size, center, r, g, b)
  -- return ffi.gc(C.SpriteText_create(text, ffi.cast("pObject*",parent), font, size, center, r, g, b), C.SpriteText__gc)
-- end

-- --local function new_sprite_text_from_sprite(text, sprite, font, size, center, r, g, b)
-- --  return ffi.gc(C.SpriteText_create_from_sprite(text, sprite, font, size, center, r, g, b), C.SpriteText__gc)
-- --end

-- local Input1      = C.Input_get_input1()
-- local Input2      = C.Input_get_input2()
-- local Input1_left = C.Input_get_trig1(C.Input_get_input1())
-- local Input2_left = C.Input_get_trig1(C.Input_get_input2())

-- local function set_on_press_callback(sprite, func)
  -- sprite:on_press( Input1_left, func )
  -- sprite:on_press( Input2_left, func )
-- end
-- local function set_on_release_callback(sprite, func)
  -- sprite:on_release( Input1_left, func )
  -- sprite:on_release( Input2_left, func )
-- end
-- local function set_on_down_callback(sprite, func)
  -- sprite:on_down( Input1_left, func )
  -- sprite:on_down( Input2_left, func )
-- end
-- local function set_on_up_callback(sprite, func)
  -- sprite:on_up( Input1_left, func )
  -- sprite:on_up( Input2_left, func )
-- end

-- local function set_focus_leave_color(sprite, focus_color, leave_color)
  -- local sprite_focus =  function(self)
                          -- sprite:set_red(focus_color.r)
                          -- sprite:set_green(focus_color.g)
                          -- sprite:set_blue(focus_color.b)
                        -- end
  -- local sprite_leave =  function(self)
                          -- sprite:set_red(leave_color.r)
                          -- sprite:set_green(leave_color.g)
                          -- sprite:set_blue(leave_color.b)
                        -- end
  -- sprite:on_enter_focus(Input1, sprite_focus)
  -- sprite:on_leave_focus(Input1, sprite_leave)
  -- sprite:on_enter_focus(Input2, sprite_focus)
  -- sprite:on_leave_focus(Input2, sprite_leave)
-- end

-- local function load_setting(ui_setting, setting)
  -- for k,v in pairs(setting) do
    -- if ui_setting[k] then
      -- ui_setting[k] = v
    -- end
  -- end
-- end

-- ----------

-- local function new_ui_button(parent, setting)
  -- local button = {}
  -- button.setting ={
                    -- x=0, y=0, alpha=255, depth=-10,
                    -- visible=true,
                    -- title="text",
                    -- focus_color={r=255,g=255,b=255},
                    -- leave_color={r=255,g=255,b=0}
                  -- }
  -- load_setting(button.setting, setting)
  -- --
  -- button.title = new_sprite_text(button.setting.title, parent, "kimberley", 24, false, 255, 255, 0)
  -- --
  -- button.set_pos      = function(self, posx, posy)
                          -- button.title:set_pos(posx, posy)
                        -- end
  -- button.set_visible  = function(self, visible)
                          -- button.title:set_visible(visible)
                        -- end
  -- button.set_alpha    = function(self, alpha)
                          -- button.title:set_alpha(alpha)
                        -- end
  -- button.set_depth    = function(self, depth)
                          -- button.title:set_depth(depth)
                        -- end
  -- button.set_fade     = function(self, alpha)
                          -- button:set_visible(true)
                          -- if alpha == 0 then
                            -- local tween_cb = function(self) button:set_visible(false) end
                            -- button.title:on_tween_line_alpha(alpha, 500, 0, tween_cb, 0)
                          -- else
                            -- local tween_cb = function(self) end
                            -- button.title:on_tween_line_alpha(alpha, 500, 0, tween_cb, 0)
                          -- end
                        -- end
  -- button.moveto       = function(self, posx, posy, duration, delay)
                          -- local tween_cb = function(self) end
                          -- button.title:on_tween_line_pos(posx, posy, duration, 0, tween_cb, delay)
                        -- end
  -- button.on_press     = function(self, func)
                          -- set_on_press_callback(button.title, func)
                        -- end
  -- --
  -- set_focus_leave_color(button.title, button.setting.focus_color, button.setting.leave_color)
  -- button:set_pos(button.setting.x, button.setting.y)
  -- button:set_alpha(button.setting.alpha)
  -- button:set_depth(button.setting.depth)
  -- button:set_visible(button.setting.visible)
  -- --
  -- return button
-- end

-- local function new_ui_ratio(parent, setting)
  -- local ratio = {}
  -- ratio.setting = {
                    -- x=0, y=0, alpha=255, depth=-10,
                    -- visible=true,
                    -- title="ratio",
                    -- is_pressed=false,
                    -- focus_color={r=255,g=255,b=255},
                    -- leave_color={r=255,g=255,b=0}
                  -- }
  -- load_setting(ratio.setting, setting)
  -- --
  -- ratio.icon        = new_sprite("cubes/cube1", parent, 32, 32, false)
  -- ratio.title       = new_sprite_text(ratio.setting.title, parent, "kimberley", 24, false, 255, 255, 0)
  -- ratio.debug_text  = new_sprite_text("FALSE", parent, "kimberley", 24, true, 100, 100, 255)
  -- --
  -- local function ratio_set_press(pressed)
    -- if pressed == true then
      -- ratio.icon:set_texture("cubes/cube-b-1")
      -- ratio.setting.is_pressed = true
    -- else
      -- ratio.icon:set_texture("cubes/cube1")
      -- ratio.setting.is_pressed = false
    -- end
    -- ratio.debug_text:change_text(tostring(ratio.setting.is_pressed))
  -- end
  -- --
  -- local ratio_press = function(self)
                        -- if ratio.setting.is_pressed == false then
                          -- ratio_set_press(true)
                        -- else
                          -- ratio_set_press(false)
                        -- end
                      -- end
  -- set_on_press_callback(ratio.icon, ratio_press)
  -- set_on_press_callback(ratio.title, ratio_press)
  -- --
  -- ratio.set_pos     = function(self, posx, posy)
                        -- ratio.icon:set_pos(posx, posy)
                        -- ratio.title:set_pos(posx+50, posy-5)
                        -- ratio.debug_text:set_pos(posx+370, posy+10)
                      -- end
  -- ratio.set_visible = function(self, visible)
                        -- ratio.icon:set_visible(visible)
                        -- ratio.title:set_visible(visible)
                        -- ratio.debug_text:set_visible(visible)
                      -- end
  -- ratio.set_alpha   = function(self, alpha)
                        -- ratio.icon:set_alpha(alpha)
                        -- ratio.title:set_alpha(alpha)
                        -- ratio.debug_text:set_alpha(alpha)
                      -- end
  -- ratio.set_depth   = function(self, depth)
                        -- ratio.icon:set_depth(depth)
                        -- ratio.title:set_depth(depth)
                      -- end
  -- ratio.set_fade    = function(self, alpha)
                        -- ratio:set_visible(true)
                        -- if alpha == 0 then
                          -- local tween_cb_icon = function(self) ratio.icon:set_visible(false) end
                          -- local tween_cb_title= function(self) ratio.icon:set_visible(false) end
                          -- local tween_cb_debug= function(self) ratio.icon:set_visible(false) end
                          -- ratio.icon:on_tween_line_alpha(alpha, 500, 0, tween_cb_icon, 0)
                          -- ratio.title:on_tween_line_alpha(alpha, 500, 0, tween_cb_title, 0)
                          -- ratio.debug_text:on_tween_line_alpha(alpha, 500, 0, tween_cb_debug, 0)
                        -- else
                          -- local tween_cb = function(self) end
                          -- ratio.icon:on_tween_line_alpha(alpha, 500, 0, tween_cb, 0)
                          -- ratio.title:on_tween_line_alpha(alpha, 500, 0, tween_cb, 0)
                          -- ratio.debug_text:on_tween_line_alpha(alpha, 500, 0, tween_cb, 0)
                        -- end
                      -- end
  -- ratio.set_pressed = function(self, pressed)
                        -- ratio_set_press(pressed)
                      -- end
  -- ratio.moveto      = function(self, posx, posy, duration, delay)
                        -- local tween_cb = function(self) end
                        -- ratio.icon:on_tween_line_pos(posx, posy, duration, 0, tween_cb, delay)
                        -- ratio.title:on_tween_line_pos(posx+50, posy-5, duration, 0, tween_cb, delay)
                        -- ratio.debug_text:on_tween_line_pos(posx+370, posy+10, duration, 0, tween_cb, delay)
                      -- end
  -- ratio.on_press    = function(self, func)
                        -- local callback  = function(self)
                                            -- if ratio.setting.is_pressed == false then
                                              -- ratio_set_press(true)
                                            -- else
                                              -- ratio_set_press(false)
                                            -- end
                                            -- func(self)
                                          -- end
                        -- set_on_press_callback(ratio.icon, callback)
                        -- set_on_press_callback(ratio.title, callback)
                      -- end
  -- --
  -- set_focus_leave_color(ratio.title, ratio.setting.focus_color, ratio.setting.leave_color)
  -- ratio:set_pos(ratio.setting.x, ratio.setting.y)
  -- ratio:set_alpha(ratio.setting.alpha)
  -- ratio:set_depth(ratio.setting.depth)
  -- ratio:set_visible(ratio.setting.visible)
  -- --
  -- return ratio
-- end

-- local function new_ui_selectbox(parent, setting)
  -- box = {}
  -- box.setting = {
                  -- x=0, y=0, alpha= 255, depth=-10,
                  -- visible= true,
                  -- index=1,
                  -- title_tb={"SELECT1", "SELECT2", "SELECT3"},
                  -- focus_color={r=255,g=255,b=0},
                  -- leave_color={r=255,g=255,b=255}
                -- }
  -- load_setting(box.setting, setting)
  -- --
  -- box.left      = new_sprite("cubes/cube-b-1", parent, 32, 32, false)
  -- box.right     = new_sprite("cubes/cube-b-1", parent, 32, 32, false)
  -- box.title     = new_sprite_text(box.setting.title_tb[box.setting.index], parent, "kimberley", 24, true, 255, 255, 0)
  -- box.debug_text= new_sprite_text(tostring(box.setting.index), parent, "kimberley", 24, true, 100, 100, 255)
  -- --
  -- local function box_change_text(index)
    -- box.title:change_text(box.setting.title_tb[index])
    -- box.debug_text:change_text(tostring(index))
  -- end
  -- local function box_index_plus()
    -- box.setting.index = box.setting.index + 1
    -- if box.setting.index > table.getn(box.setting.title_tb) then box.setting.index = 1 end
    -- box_change_text(box.setting.index)
  -- end
  -- local function box_index_minus()
    -- box.setting.index = box.setting.index - 1
    -- if box.setting.index < 1 then box.setting.index = table.getn(box.setting.title_tb) end
    -- box_change_text(box.setting.index)
  -- end
  -- --
  -- local left_press  = function(self) box_index_minus() end
  -- set_on_press_callback(box.left, left_press)

  -- local right_press = function(self) box_index_plus() end
  -- set_on_press_callback(box.right, right_press)
  -- --
  -- box.set_pos       = function(self, posx, posy)
                        -- box.left:set_pos(posx, posy)
                        -- box.right:set_pos(posx+280, posy)
                        -- box.title:set_pos(posx+150, posy+10)
                        -- box.debug_text:set_pos(posx+370, posy+10)
                      -- end
  -- box.set_visible   = function(self, visible)
                        -- box.left:set_visible(visible)
                        -- box.right:set_visible(visible)
                        -- box.title:set_visible(visible)
                        -- box.debug_text:set_visible(visible)
                      -- end
  -- box.set_alpha     = function(self, alpha)
                        -- box.left:set_alpha(alpha)
                        -- box.right:set_alpha(alpha)
                        -- box.title:set_alpha(alpha)
                        -- box.debug_text:set_alpha(alpha)
                      -- end
  -- box.set_depth     = function(self, depth)
                        -- box.left:set_depth(depth)
                        -- box.right:set_depth(depth)
                      -- end
  -- box.set_fade      = function(self, alpha)
                        -- box:set_visible(true)
                        -- if alpha == 0 then
                          -- local tween_cb_left   = function(self) box.left:set_visible(false) end
                          -- local tween_cb_right  = function(self) box.right:set_visible(false) end
                          -- local tween_cb_title  = function(self) box.title:set_visible(false) end
                          -- local tween_cb_debug  = function(self) box.debug_text:set_visible(false) end
                          -- box.left:on_tween_line_alpha(alpha, 500, 0, tween_cb_left, 0)
                          -- box.right:on_tween_line_alpha(alpha, 500, 0, tween_cb_right, 0)
                          -- box.title:on_tween_line_alpha(alpha, 500, 0, tween_cb_title, 0)
                          -- box.debug_text:on_tween_line_alpha(alpha, 500, 0, tween_cb_debug, 0)
                        -- else
                          -- local tween_cb = function(self) end
                          -- box.left:on_tween_line_alpha(alpha, 500, 0, tween_cb, 0)
                          -- box.right:on_tween_line_alpha(alpha, 500, 0, tween_cb, 0)
                          -- box.title:on_tween_line_alpha(alpha, 500, 0, tween_cb, 0)
                          -- box.debug_text:on_tween_line_alpha(alpha, 500, 0, tween_cb, 0)
                        -- end
                      -- end
  -- box.set_index     = function(self, index)
                        -- if index < 0 then return end
                        -- if index > table.getn(box.setting.title_tb) then return end
                        -- box.setting.index = index
                        -- box_change_text(box.setting.index)
                      -- end
  -- box.moveto        = function(self, posx, posy, duration, delay)
                        -- local tween_cb = function(self) end
                        -- box.left:on_tween_line_pos(posx, posy, duration, 0, tween_cb, delay)
                        -- box.right:on_tween_line_pos(posx+280, posy, duration, 0, tween_cb, delay)
                        -- box.title:on_tween_line_pos(posx+150, posy+10, duration, 0, tween_cb, delay)
                        -- box.debug_text:on_tween_line_pos(posx+370, posy+10, duration, 0, tween_cb, delay)
                      -- end
  -- box.left_on_press = function(self, func)
                        -- local callback  = function(self)
                                            -- box_index_minus()
                                            -- func(self)
                                          -- end
                        -- set_on_press_callback(box.left, callback)
                      -- end
  -- box.right_on_press= function(self, func)
                        -- local callback  = function(self)
                                            -- box_index_plus()
                                            -- func(self)
                                          -- end
                        -- set_on_press_callback(box.right, callback)
                      -- end
  -- --
  -- set_focus_leave_color(box.left, box.setting.focus_color, box.setting.leave_color)
  -- set_focus_leave_color(box.right, box.setting.focus_color, box.setting.leave_color)
  -- box:set_pos(box.setting.x, box.setting.y)
  -- box:set_alpha(box.setting.alpha)
  -- box:set_depth(box.setting.depth)
  -- box:set_visible(box.setting.visible)
  -- --
  -- return box
-- end

-- local function new_ui_scrollbar(parent, setting)
  -- scrollbar = {}
  -- scrollbar.setting = {
                        -- x=0, y=0, alpha=255, depth=-50,
                        -- visible=true,
                        -- index=0,
                        -- range=10,
                        -- input=nil,
                        -- focus1=false,
                        -- focus2=false,
                        -- focus_color={r=255,g=255,b=255},
                        -- leave_color={r=255,g=255,b=0}
                      -- }
  -- load_setting(scrollbar.setting, setting)
  -- --
  -- scrollbar.parent      = parent
  -- scrollbar.line        = new_sprite("cubes/cube1", parent, 256, 16, false)
  -- scrollbar.button      = new_sprite("cubes/cube-b-1", parent, 32, 32, false)
  -- scrollbar.title       = new_sprite_text("0", parent, "kimberley", 24, true, 255, 255, 0)
  -- scrollbar.debug_text  = new_sprite_text("off", parent, "kimberley", 24, true, 100, 100, 255)
  -- --
  -- local function set_input(input)
    -- if input == nil then
      -- scrollbar.setting.input = nil
      -- scrollbar.debug_text:change_text("off")
    -- else
      -- scrollbar.setting.input = input
      -- if input == Input1 then
        -- scrollbar.debug_text:change_text("1")
      -- else
        -- scrollbar.debug_text:change_text("2")
      -- end
    -- end
  -- end
  -- local function update_button_position(input)
    -- local pos_x = C.Input_get_cursor_x(input) - scrollbar.parent:get_screen_pos_x() - (scrollbar.button:get_size_x()/2)
    -- local pos_y = scrollbar.button:get_pos_y()
    -- local bg_left = scrollbar.line:get_pos_x()
    -- local bg_right= bg_left + scrollbar.line:get_size_x() - scrollbar.button:get_size_x()
    -- if pos_x < bg_left then pos_x = bg_left end
    -- if pos_x > bg_right then pos_x = bg_right end
    -- scrollbar.button:set_pos(pos_x, pos_y)
    -- --
    -- scrollbar.setting.index = math.floor( (pos_x-bg_left)*scrollbar.setting.range/(scrollbar.line:get_size_x()-scrollbar.button:get_size_x()) )
    -- scrollbar.title:change_text(tostring(scrollbar.setting.index))
  -- end
  -- --
  -- local function scrollbar_button_press(input)
    -- local function button_press(self)
      -- if scrollbar.setting.input == nil then
        -- set_input(input)
      -- end
    -- end
    -- return button_press
  -- end
  -- local function scrollbar_button_release(input)
    -- local function button_release(self)
      -- if scrollbar.setting.input == input then
        -- set_input(nil)
      -- end
    -- end
    -- return button_release
  -- end
  -- scrollbar.button:on_press( Input1_left, scrollbar_button_press(Input1) )
  -- scrollbar.button:on_press( Input2_left, scrollbar_button_press(Input2) )
  -- scrollbar.button:on_release( Input1_left, scrollbar_button_release(Input1) )
  -- scrollbar.button:on_release( Input2_left, scrollbar_button_release(Input2) )
  -- --
  -- local function scrollbar_button_focus(input)
    -- local function button_focus(self)
      -- scrollbar.button:set_blue(0)
      -- if input == Input1 then
        -- scrollbar.setting.focus1 = true
      -- else
        -- scrollbar.setting.focus2 = true
      -- end
    -- end
    -- return button_focus
  -- end
  -- local function scrollbar_button_leave(input)
    -- local function button_leave(self)
      -- if input == Input1 then
        -- scrollbar.setting.focus1 = false
        -- if scrollbar.setting.focus2 == false then scrollbar.button:set_blue(255) end
      -- else
        -- scrollbar.setting.focus2 = false
        -- if scrollbar.setting.focus1 == false then scrollbar.button:set_blue(255) end
      -- end
      -- if scrollbar.setting.input == input then
        -- update_button_position(input)
        -- set_input(nil)
      -- end
    -- end
    -- return button_leave
  -- end
  -- scrollbar.button:on_enter_focus( Input1, scrollbar_button_focus(Input1) )
  -- scrollbar.button:on_enter_focus( Input2, scrollbar_button_focus(Input2) )
  -- scrollbar.button:on_leave_focus( Input1, scrollbar_button_leave(Input1) )
  -- scrollbar.button:on_leave_focus( Input2, scrollbar_button_leave(Input2) )
  -- --
  -- local function scrollbar_button_down(input)
    -- local button_down = function(self)
      -- update_button_position(input)
    -- end
    -- return button_down
  -- end
  -- scrollbar.button:on_down( Input1_left, scrollbar_button_down(Input1) )
  -- scrollbar.button:on_down( Input2_left, scrollbar_button_down(Input2) )
  -- --
  -- local function scrollbar_line_press(input)
    -- local line_press = function(self)
      -- if scrollbar.setting.input == nil then
        -- update_button_position(input)
        -- set_input(input)
      -- end
    -- end
    -- return line_press
  -- end
  -- scrollbar.line:on_press( Input1_left, scrollbar_line_press(Input1) )
  -- scrollbar.line:on_press( Input2_left, scrollbar_line_press(Input2) )
  -- --
  -- scrollbar.set_pos     = function(self, posx, posy)
                            -- scrollbar.line:set_pos(posx, posy+10)
                            -- local bg_left = scrollbar.line:get_pos_x()
                            -- local bg_right= bg_left + scrollbar.line:get_size_x() - scrollbar.button:get_size_x()
                            -- local btn_posx = ( scrollbar.setting.index / scrollbar.setting.range ) * (bg_right - bg_left) + bg_left
                            -- scrollbar.button:set_pos(btn_posx, posy)
                            -- scrollbar.title:set_pos(posx+300, posy+10)
                            -- scrollbar.debug_text:set_pos(posx+370, posy+10)
                          -- end
  -- scrollbar.set_visible = function(self, visible)
                            -- scrollbar.line:set_visible(visible)
                            -- scrollbar.button:set_visible(visible)
                            -- scrollbar.title:set_visible(visible)
                            -- scrollbar.debug_text:set_visible(visible)
                          -- end
  -- scrollbar.set_alpha   = function(self, alpha)
                            -- scrollbar.line:set_alpha(alpha)
                            -- scrollbar.button:set_alpha(alpha)
                            -- scrollbar.title:set_alpha(alpha)
                            -- scrollbar.debug_text:set_alpha(alpha)
                          -- end
  -- scrollbar.set_depth   = function(self, depth)
                            -- scrollbar.line:set_depth(depth)
                            -- scrollbar.button:set_depth(depth-50)
                          -- end
  -- scrollbar.set_fade    = function(self, alpha)
                            -- scrollbar:set_visible(true)
                            -- if alpha == 0 then
                              -- local tween_cb_line   = function(self) scrollbar.line:set_visible(false) end
                              -- local tween_cb_button = function(self) scrollbar.button:set_visible(false) end
                              -- local tween_cb_title  = function(self) scrollbar.title:set_visible(false) end
                              -- local tween_cb_debug  = function(self) scrollbar.debug_text:set_visible(false) end
                              -- scrollbar.line:on_tween_line_alpha(alpha, 500, 0, tween_cb_line, 0)
                              -- scrollbar.button:on_tween_line_alpha(alpha, 500, 0, tween_cb_button, 0)
                              -- scrollbar.title:on_tween_line_alpha(alpha, 500, 0, tween_cb_title, 0)
                              -- scrollbar.debug_text:on_tween_line_alpha(alpha, 500, 0, tween_cb_debug, 0)

                            -- else
                              -- local tween_cb = function(self) end
                              -- scrollbar.line:on_tween_line_alpha(alpha, 500, 0, tween_cb, 0)
                              -- scrollbar.button:on_tween_line_alpha(alpha, 500, 0, tween_cb, 0)
                              -- scrollbar.title:on_tween_line_alpha(alpha, 500, 0, tween_cb, 0)
                              -- scrollbar.debug_text:on_tween_line_alpha(alpha, 500, 0, tween_cb, 0)
                            -- end
                          -- end
  -- scrollbar.set_index   = function(self, index)
                            -- if index == nil then index = 0 end
                            -- if index < 0 then index = 0 end
                            -- if index > scrollbar.setting.range then index = scrollbar.setting.range end
                            -- local bg_left = scrollbar.line:get_pos_x()
                            -- local bg_right= bg_left + scrollbar.line:get_size_x() - scrollbar.button:get_size_x()
                            -- local posx = ( index / scrollbar.setting.range ) * (bg_right - bg_left) + bg_left
                            -- local posy = scrollbar.button:get_pos_y()
                            -- scrollbar.button:set_pos(posx, posy)
                            -- scrollbar.title:change_text(tostring(index))
                            -- scrollbar.setting.index = index
                          -- end
  -- scrollbar.on_down     = function(self, func)
                            -- local function press_callback(input)
                              -- local callback = function(self)
                                -- if scrollbar.setting.input == input then
                                  -- update_button_position(input)
                                  -- func(self)
                                -- end
                              -- end
                              -- return callback
                            -- end
                            -- scrollbar.button:on_down( Input1_left, press_callback(Input1) )
                            -- scrollbar.button:on_down( Input2_left, press_callback(Input2) )
                          -- end
  -- --
  -- scrollbar:set_depth(scrollbar.setting.depth)
  -- scrollbar:set_pos(scrollbar.setting.x, scrollbar.setting.y)
  -- scrollbar:set_alpha(scrollbar.setting.alpha)
  -- scrollbar:set_visible(scrollbar.setting.visible)
  -- return scrollbar
-- end

-- local function load_option(option_data)
  -- local file = io.open("rc/config/option", "r")
  -- if file == nil then
    -- io.output("rc/config/option")
    -- io.close()
  -- else
    -- local offset
    -- offset, option_data = msgpack.unpack( file:read() )
    -- file:close()
  -- end
  -- return option_data
-- end

-- local function save_option(option_data)
  -- local s = msgpack.pack(option_data)
  -- local file = io.open("rc/config/option", "w")
  -- file:write(s)
  -- file:close()
-- end

-- return {
  -- new_sprite                  = new_sprite,
-- --new_sprite_from_sprite      = new_sprite_from_sprite,
  -- new_sprite_text             = new_sprite_text,
-- --new_sprite_text_from_sprite = new_sprite_text_from_sprite,
  -- --
  -- new_ui_button               = new_ui_button,
  -- new_ui_ratio                = new_ui_ratio,
  -- new_ui_selectbox            = new_ui_selectbox,
  -- new_ui_scrollbar            = new_ui_scrollbar,
  -- --
  -- load_option                 = load_option,
  -- save_option                 = save_option
-- }
