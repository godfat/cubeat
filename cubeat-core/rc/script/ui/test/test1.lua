
local ffi      = require 'ffi'
local C        = ffi.C

local view     = require 'rc/script/ui/test/view'

-------------------- scripts above this line should be separated -----------------
----------------------------------------------------------------------------------

function test_ui_entry(self)
  self = ffi.cast("TestUI*", self);
  local s = self:get_ui_scene()
  local sp = view.newSprite("area_rect", s, 64, 64, true)
  sp:set_pos(640, 360)
  sp:set_scale(1.5, 1.5, 1)
  sp:set_red(0)
  sp:set_alpha(100)
  sp:set_visible(true)
end
