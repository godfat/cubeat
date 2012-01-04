
local ffi      = require 'ffi'
local C        = ffi.C

local view     = require 'rc/script/ui/test/view'

-------------------- scripts above this line should be separated -----------------
----------------------------------------------------------------------------------

function test_ui_entry(self)
  self = ffi.cast("TestUI*", self);
  local s = self:get_ui_scene()
  local sp = view.newSprite("area_rect", s, 64, 64, true)
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
end
