local ffi = require 'ffi'
local C   = ffi.C
require 'rc/script/ui/demo/defs'

----------------------------------------------------------------------------

local scene_

function init(self)
  self   = ffi.cast("Demo*", self);
  scene_ = self:get_ui_scene()
  print 'Hello from Lua!'
end

function mainmenu()

end
