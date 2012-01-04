
local ffi      = require 'ffi'
local C        = ffi.C

local view     = require 'rc/script/ui/test/view'

-------------------- scripts above this line should be separated -----------------
----------------------------------------------------------------------------------

local scene_, sp 
local cb

function init(self)
  self    = ffi.cast("TestUI*", self);
  scene_  = self:get_ui_scene()
  sp      = view.newSprite("area_rect", scene_, 64, 64, true)
  sp:set_texture("cubes/cube-b-1")
  sp:set_pos(640, 360)
  sp:set_scale(1.5, 1.5, 1)
  --sp:set_red(0)
  --sp:set_green(0)
  --sp:set_blue(0)
  --sp:set_alpha(100)
  --sp:set_redE(100)
  --sp:set_greenE(100)
  --sp:set_blueE(100)
  sp:set_visible(true)
  sp:set_center_aligned(false)
  sp:move_to(100, 100)
  sp:move_tween(0,0,5000)
  sp:texture_flipH()
  sp:texture_flipV()
  sp:set_size(32, 32)
  
  cb = ffi.cast("PSC_OBJCALLBACK", function(self) 
    print 'hi'
  end)
  
  sp:on_release(cb)
end

function cycle(self)
  self = ffi.cast("TestUI*", self);
end

function destroy(self)
  self = ffi.cast("TestUI*", self);
  cb:free()
end

function test_ui_create_buttons(self)
  self = ffi.cast("TestUI*", self);
  local s = self:get_ui_scene()
  
  local btn_green = {}
  for i=1,6 do
    btn_green[i] = view.newSprite("cubes/cube-g-1", s, 64, 64, false)
    btn_green[i]:move_to(600, (20+i*80))
  end
  
  local btn_blue = {}
  for i=1,6 do
    btn_blue[i] = view.newSprite("cubes/cube-b-1", s, 64, 64, false)
    btn_blue[i]:move_to(680, (20+i*80))
  end
  
  local btn_red = {}
  for i=1,6 do
    btn_red[i] = view.newSprite("cubes/cube-r-1", s, 64, 64, false)
    btn_red[i]:move_to(760, (20+i*80))
  end  
end
