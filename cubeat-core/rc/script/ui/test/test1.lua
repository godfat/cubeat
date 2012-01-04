
local ffi      = require 'ffi'
local C        = ffi.C

local view     = require 'rc/script/ui/test/view'

-------------------- scripts above this line should be separated -----------------
----------------------------------------------------------------------------------

local scene_, sp 
local cb

function init(self)
  self = ffi.cast("TestUI*", self);
  scene_ = self:get_ui_scene()
  sp     = view.newSprite("area_rect", scene_, 64, 64, true)
  sp:set_pos(640, 360)
  
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
