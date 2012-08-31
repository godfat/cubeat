require 'rc/script/strict'
local ffi  = require 'ffi'
local C    = ffi.C
local view = require 'rc/script/ui/view'

local scene_
local layer1_
local layer2_
local layer3_
local layer4_
local layer5_
local layer6_
local layer7_
local layer8_
local layer9_

function init(scene)
  scene_ = ffi.cast("pScene*", scene)
  layer1_ = view.new_sprite("bg1/01sky", scene_, 1280, 720, false):set_depth(-1)
  layer2_ = view.new_sprite("bg1/02cloud1", scene_, 1280, 720, false):set_depth(-2)
  layer3_ = view.new_sprite("bg1/03cloud2", scene_, 1280, 720, false):set_depth(-3)
  layer4_ = view.new_sprite("bg1/04cloud3", scene_, 1280, 720, false):set_depth(-4)
  layer5_ = view.new_sprite("bg1/05cloud4", scene_, 1280, 720, false):set_depth(-5)
  layer6_ = view.new_sprite("bg1/06cube", scene_, 1280, 720, false):set_depth(-6)
  layer7_ = view.new_sprite("bg1/07flyingwhale", scene_, 1280, 720, false):set_depth(-7)
  layer8_ = view.new_sprite("bg1/09building", scene_, 1280, 720, false):set_depth(-8)
  layer9_ = view.new_sprite("bg1/08whaleface", scene_, 1280, 720, false):set_depth(-9)
end

function cleanup()
  layer1_ = nil
  layer2_ = nil
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
