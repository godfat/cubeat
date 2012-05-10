local ffi       = require 'ffi'
local C         = ffi.C
local ui        = require 'rc/script/ui/puzzle/ui'

----------------------------------------------------------------------------

local scene_, sp_, spt_

function init(self)
  self    = ffi.cast("Puzzle*", self);
  scene_  = self:get_ui_scene()
  
  local sp_setting  = { path='win', x=800, y=300, w=256, h=256 }
  sp_ = ui.new_ui_image(scene_, sp_setting)
  
  local spt_setting = { title='Hello world', x=800, y=600 }
  spt_ = ui.new_ui_text(scene_, spt_setting)
end