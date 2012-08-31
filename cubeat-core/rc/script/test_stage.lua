require 'rc/script/strict'
local ffi  = require 'ffi'
local C    = ffi.C
local view = require 'rc/script/ui/view'

local scene_
local layer0_
local layer1_
local layer2_
local layer22_
local layer3_
local layer4_
local layer5_
local layer6_
local layer7_
local layer8_
local layer9_

function init(scene)
  scene_ = ffi.cast("pScene*", scene)
  -- layer0_ = view.new_sprite("bg1/99complete", scene_)
  -- layer0_:set_depth(0)
  
  layer1_ = view.new_sprite("bg1/01sky", scene_)       
  layer1_:set_depth(-10)
  layer1_:set_pos( 1280 - layer1_:get_size_x(), 0 )
  
  layer2_ = view.new_sprite("bg1/02cloud1", scene_)
  layer2_:set_depth(-20)
  layer2_:tween("Linear", "Pos2D", ffi.new("v2", 910, 245), ffi.new("v2", 1020, 245), 20000, -1)
  layer2_:tween("SineCirc", "Alpha", 0, 255, 20000, -1)
  
  layer22_ = view.new_sprite("bg1/02cloud1", scene_)
  layer22_:set_depth(-20)
  layer22_:set_alpha(0)
  layer22_:tween("Linear", "Pos2D", ffi.new("v2", 870, 245), ffi.new("v2", 980, 245), 20000, -1, nil, 15000)
  layer22_:tween("SineCirc", "Alpha", 0, 255, 20000, -1, nil, 15000)
  
  layer3_ = view.new_sprite("bg1/03cloud2", scene_)
  layer3_:set_depth(-30)
  layer3_:set_pos(425, 300)
  
  layer4_ = view.new_sprite("bg1/04cloud3", scene_)
  layer4_:set_depth(-40)
  layer4_:set_pos( 1280 - layer4_:get_size_x(), 0 )
  
  layer5_ = view.new_sprite("bg1/05cloud4", scene_)
  layer5_:set_depth(-50)
  layer5_:set_pos(325, 0)
  
  layer6_ = view.new_sprite("bg1/06cube", scene_)
  layer6_:set_depth(-60)
  layer6_:set_pos(500, 107)
  
  layer7_ = view.new_sprite("bg1/07flyingwhale", scene_)
  layer7_:set_depth(-70)
  layer7_:set_pos(1078, 199)
  layer7_:tween("SineCirc", "Pos2D", ffi.new("v2", 1070, 199), ffi.new("v2", 1090, 199), 7000, -1)
  
  layer8_ = view.new_sprite("bg1/09building", scene_)
  layer8_:set_depth(-80)
  
  layer9_ = view.new_sprite("bg1/08whaleface", scene_)
  layer9_:set_depth(-90)
  layer9_:set_pos(180, 0)
end

function cleanup()
  layer1_ = nil
  layer2_ = nil
  layer22_ = nil
  layer3_ = nil
  layer4_ = nil
  layer5_ = nil
  layer6_ = nil
  layer7_ = nil
  layer8_ = nil
  layer9_ = nil
  collectgarbage 'collect'
end

-- function init(scene)
  -- scene_ = ffi.cast("pScene*", scene)
  -- local sp = view.new_sprite("bg3/complete", scene_, 1280, 720, false)
  -- local bl = view.new_sprite("radial_bloom", scene_, 1024, 640, true)
  -- bl:set_pos(675, 300)
  
  -- bl:tween("SineCirc", "Alpha", 150, 64, 7000, -1)
-- end
