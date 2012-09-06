require 'rc/script/strict'
local ffi   = require 'ffi'
local C     = ffi.C
local view  = require 'rc/script/ui/view'
local event = require 'rc/script/event/event'

local scene_
local layer0_
local layer1_
local layer2_
local layer3_
local layer4_
local layer5_
local layer6_
local layer7_
local layer8_
local layer9_

local function three_stage_alpha_tween(sp, total_time, fade_time)
  local delayed_fade = function(self)
    local delay = total_time - fade_time*2
    sp:tween("Linear", "Alpha", 255, 0, fade_time, 0, function(self) self:remove() end, delay)
  end
  sp:tween("Linear", "Alpha", 0, 255, fade_time, 0, delayed_fade)
end

function init(scene)
  scene_ = ffi.cast("pScene*", scene)
  -- layer0_ = view.new_sprite("bg1/99complete", scene_)
  -- layer0_:set_depth(0)
  
  layer1_ = view.new_sprite("bg1/01sky", scene_)       
  layer1_:set_depth(-100)
  layer1_:set_pos( 1280 - layer1_:get_size_x(), 0 )
  
  layer2_ = view.new_sprite("bg1/02cloud1", scene_)
  layer2_:set_depth(-200)
  layer2_:tween("Linear", "Pos2D", ffi.new("v2", 910, 245), ffi.new("v2", 1020, 245), 2000, -1)
  layer2_:tween("SineCirc", "Alpha", 0, 255, 2000, -1)
  
  layer3_ = view.new_sprite("bg1/03cloud2", scene_)
  layer3_:set_depth(-300)
  layer3_:set_pos(425, 300)
  layer3_:tween("Linear", "Pos2D", ffi.new("v2", 400, 300), ffi.new("v2", 520, 300), 1500, -1)
  layer3_:tween("SineCirc", "Alpha", 64, 255, 1500, -1)
  
  local produce_cloud3 = function()
    local local_cloud = view.new_sprite("bg1/04cloud3", scene_)
    local_cloud:set_depth(-400)
    local pos_x = 1280 - local_cloud:get_size_x()
    local_cloud:set_pos( pos_x, 0 )
    local_cloud:tween("Linear", "Pos2D", ffi.new("v2", pos_x, 0), ffi.new("v2", pos_x+30, 0), 3000, -1)
    three_stage_alpha_tween(local_cloud, 3000, 500)
  end
  produce_cloud3()
  local cloud3_timer = event.on_timer("global", produce_cloud3, 2500, -1)
    
  local produce_cloud4 = function()
    local local_cloud = view.new_sprite("bg1/05cloud4", scene_)
    local_cloud:set_depth(-500)
    local_cloud:set_pos(325, 0)
    local_cloud:tween("Linear", "Pos2D", ffi.new("v2", 315, 0), ffi.new("v2", 345, 0), 1000, -1)
    three_stage_alpha_tween(local_cloud, 1000, 200)
  end
  produce_cloud4()
  local cloud4_timer = event.on_timer("global", produce_cloud4, 800, -1)
  
  layer6_ = view.new_sprite("bg1/06cube", scene_)
  layer6_:set_depth(-600)
  layer6_:set_pos(500, 107)
  
  layer7_ = view.new_sprite("bg1/07flyingwhale", scene_)
  layer7_:set_depth(-700)
  layer7_:set_pos(1078, 199)
  layer7_:tween("SineCirc", "Pos2D", ffi.new("v2", 1070, 199), ffi.new("v2", 1090, 199), 7000, -1)
  
  layer8_ = view.new_sprite("bg1/09building", scene_)
  layer8_:set_depth(-800)
  
  layer9_ = view.new_sprite("bg1/08whaleface", scene_)
  layer9_:set_depth(-900)
  layer9_:set_pos(180, 0)
  
  local debug_timer = event.on_timer("global", function() view.debug_hack() end, 1000, -1)
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
