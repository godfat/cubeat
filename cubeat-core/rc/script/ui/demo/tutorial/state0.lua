local ffi   = require 'ffi'
local C     = ffi.C
local view  = require 'rc/script/ui/view'
local ui    = require 'rc/script/ui/ui'

local ask_panel_ = nil

local function update(demo, scene, data)
  ask_panel_=ui.new_tutorialinfo{ parent=scene }
  ask_panel_:on_press_ok( function(self) demo:eventual_resume() end )
  demo:eventual_pause()
end

return {
  update  = update
}