
local ffi      = require 'ffi'
local C        = ffi.C

local view     = require 'rc/script/ui/test/view'

-------------------- scripts above this line should be separated -----------------
----------------------------------------------------------------------------------

local scene_, sp 

local input1_trig1 = {}
local input1_trig2 = {}
local input2_trig1 = {}
local input2_trig2 = {}

function init(self)
  self = ffi.cast("TestUI*", self);
  scene_ = self:get_ui_scene()
  sp     = view.new_sprite("area_rect", scene_, 64, 64, true)
  sp:set_pos(640, 360)
  
  print (input1_trig1)
  print (input1_trig2)
  print (input2_trig1)
  print (input2_trig2)

  sp:on_release(input1_trig1, function(self)
    print 'hi' 
    self:on_release(input1_trig2, function(self)
      print 'hello'
    end)
  end)
 
end

function cycle(self)
  self = ffi.cast("TestUI*", self);
end

function destroy(self)
  self = ffi.cast("TestUI*", self);
end
