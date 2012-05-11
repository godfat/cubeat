local ffi       = require 'ffi'
local C         = ffi.C
local ui        = require 'rc/script/ui/puzzle/ui'

----------------------------------------------------------------------------

local scene_, sp_, spt_, btn_

function init(self)
  self    = ffi.cast("Puzzle*", self);
  scene_  = self:get_ui_scene()
  
  local sp_setting  = { path='win', x=800, y=300, w=256, h=256 }
  sp_ = ui.new_ui_image(scene_, sp_setting)
  
  local spt_setting = { title='Hello world', x=800, y=600, size=48 }
  spt_ = ui.new_ui_text(scene_, spt_setting)
  
  local btn_setting = { title='button', x=800, y=650 }
  btn_ = ui.new_ui_button(scene_, btn_setting)
  local btn_press = function(self) spt_:set_visible(false) end
  btn_:on_press(btn_press)
end