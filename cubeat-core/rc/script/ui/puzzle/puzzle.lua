local ffi       = require 'ffi'
local C         = ffi.C
local ui        = require 'rc/script/ui/puzzle/ui'

----------------------------------------------------------------------------

local scene_, sp_

function init(self)
  self    = ffi.cast("Puzzle*", self);
  scene_  = self:get_ui_scene()
  
  local sp_setting  = { path='win', x=800, y=300, w=256, h=256 }
  sp_ = ui.new_ui_image(scene_, sp_setting)
end