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

-- STAGE 1
-- function init(scene)
  -- scene_ = ffi.cast("pScene*", scene)
  -- -- layer0_ = view.new_sprite("bg1/99complete", scene_)
  -- -- layer0_:set_depth(0)
  
  -- layer1_ = view.new_sprite("bg1/01sky", scene_)       
  -- layer1_:set_depth(-100)
  -- layer1_:set_pos( 1280 - layer1_:get_size_x(), 0 )
  
  -- local produce_cloud1 = function()
    -- local local_cloud = view.new_sprite("bg1/02cloud1", scene_)
    -- local_cloud:set_depth(-200)
    -- local_cloud:tween("Linear", "Pos2D", ffi.new("v2", 910, 245), ffi.new("v2", 1020, 245), 2000, -1)
    -- local_cloud:tween("SineCirc", "Alpha", 0, 255, 2000, 0, function(self) self:remove() end)
  -- end
  -- produce_cloud1()
  -- local cloud1_timer = event.on_timer("global", produce_cloud1, 1000, -1)
  
  -- local produce_cloud2 = function()
    -- local local_cloud = view.new_sprite("bg1/03cloud2", scene_)
    -- local_cloud:set_depth(-300)
    -- local_cloud:set_pos(425, 300)
    -- local_cloud:tween("Linear", "Pos2D", ffi.new("v2", 400, 300), ffi.new("v2", 520, 300), 1500, -1)
    -- local_cloud:tween("SineCirc", "Alpha", 64, 255, 1500, 0, function(self) self:remove() end)
  -- end
  -- produce_cloud2()
  -- local cloud2_timer = event.on_timer("global", produce_cloud2, 750, -1)
  
  -- local produce_cloud3 = function()
    -- local local_cloud = view.new_sprite("bg1/04cloud3", scene_)
    -- local_cloud:set_depth(-400)
    -- local pos_x = 1280 - local_cloud:get_size_x()
    -- local_cloud:set_pos( pos_x, 0 )
    -- local_cloud:tween("Linear", "Pos2D", ffi.new("v2", pos_x, 0), ffi.new("v2", pos_x+30, 0), 3000, -1)
    -- three_stage_alpha_tween(local_cloud, 3000, 500)
  -- end
  -- produce_cloud3()
  -- local cloud3_timer = event.on_timer("global", produce_cloud3, 2500, -1)
    
  -- local produce_cloud4 = function()
    -- local local_cloud = view.new_sprite("bg1/05cloud4", scene_)
    -- local_cloud:set_depth(-500)
    -- local_cloud:set_pos(325, 0)
    -- local_cloud:tween("Linear", "Pos2D", ffi.new("v2", 315, 0), ffi.new("v2", 345, 0), 1000, -1)
    -- three_stage_alpha_tween(local_cloud, 1000, 200)
  -- end
  -- produce_cloud4()
  -- local cloud4_timer = event.on_timer("global", produce_cloud4, 800, -1)
  
  -- layer6_ = view.new_sprite("bg1/06cube", scene_)
  -- layer6_:set_depth(-600)
  -- layer6_:set_pos(500, 107)
  
  -- layer7_ = view.new_sprite("bg1/07flyingwhale", scene_)
  -- layer7_:set_depth(-700)
  -- layer7_:set_pos(1078, 199)
  -- layer7_:tween("SineCirc", "Pos2D", ffi.new("v2", 1070, 199), ffi.new("v2", 1090, 199), 7000, -1)
  
  -- layer8_ = view.new_sprite("bg1/09building", scene_)
  -- layer8_:set_depth(-800)
  
  -- layer9_ = view.new_sprite("bg1/08whaleface", scene_)
  -- layer9_:set_depth(-900)
  -- layer9_:set_pos(180, 0)
  
  -- local debug_timer = event.on_timer("global", function() view.debug_hack() end, 1000, -1)
-- end

-- STAGE 2
-- function init(scene)
  -- scene_ = ffi.cast("pScene*", scene)
  -- layer0_ = view.new_sprite("bg2/99complete", scene_)
  -- layer0_:set_depth(0)
  
  -- layer1_ = view.new_sprite("bg2/02whale", scene_)
  -- layer1_:set_depth(-100)
  -- layer1_:set_pos(14, 260)
  
  -- -- layer6_ = view.new_sprite("bg2/03whalesmile", scene_)
  -- -- layer6_:set_depth(-600)
  -- -- layer6_:set_pos(14, 260)
  
  -- layer7_ = view.new_sprite("bg2/04glasses", scene_)
  -- layer7_:set_depth(-700)
  -- layer7_:set_pos(202, 307)
  -- layer7_:tween("SineCirc", "Pos2D", ffi.new("v2", 202, 309), ffi.new("v2", 205, 301), 4000, -1)

  -- layer8_ = view.new_sprite("radial_bloom", scene_, 960, 640)
  -- layer8_:set_depth(-800)
  -- layer8_:tween("SineCirc", "Alpha", 144, 0, 10000, -1)
-- end

-- STAGE 3
-- function init(scene)
  -- scene_ = ffi.cast("pScene*", scene)
  -- layer0_ = view.new_sprite("bg3/99complete", scene_)
  
  -- layer1_ = view.new_sprite("radial_bloom", scene_, 1024, 640, true)
  -- layer1_:set_pos(675, 300)
  -- layer1_:set_depth(-100)
  -- layer1_:tween("SineCirc", "Alpha", 150, 64, 7000, -1)
-- end

-- STAGE 4
-- function init(scene)
  -- scene_ = ffi.cast("pScene*", scene)
  -- layer0_ = view.new_sprite("bg4/99complete", scene_)
  
  -- layer1_ = view.new_sprite("bg4/02man1", scene_)
  -- layer1_:set_depth(-100)
  -- layer1_:set_pos(206, 293)
  
  -- layer6_ = view.new_sprite("bg4/04man3", scene_)
  -- layer6_:set_depth(-600)
  -- layer6_:set_pos(204, 290)
  
  -- local eye_sparkle = function()
    -- local sp = view.new_sprite("bg4/05maneye", scene_)
    -- sp:set_depth(-700)
    -- sp:set_pos(239, 332)
    -- sp:set_alpha(0)
    -- sp:tween("Linear", "Alpha", 255, 0, 1000, 0, function(self) self:remove() end)
  -- end
  -- local sparkle_timer = event.on_timer("global", eye_sparkle, 2000, -1)
  
  -- layer8_ = view.new_sprite("bg4/06whale", scene_)
  -- layer8_:set_depth(-800)
  -- layer8_:set_pos(739, 0)
  -- layer8_:tween("SineCirc", "Pos2D", ffi.new("v2", 732, -5), ffi.new("v2", 745, 0), 8000, -1)
  
  -- layer9_ = view.new_sprite("radial_bloom", scene_, 1024, 640, true)
  -- layer9_:set_depth(-900)
  -- layer9_:set_pos(675, 250)
  -- layer9_:tween("SineCirc", "Alpha", 128, 0, 7000, -1)
-- end

-- STAGE 5
function init(scene)
  scene_ = ffi.cast("pScene*", scene)
  layer0_ = view.new_sprite("bg5/99complete", scene_)
  
  local fish_effect = function()
    local fish  = view.new_sprite("bg5/02fish", scene_)
    fish:set_center_aligned(true)
    fish:set_depth(-100)
    fish:set_pos(290, 500)
    fish:tween("Linear", "Rotation", ffi.new("v3", 0, 0, 60), ffi.new("v3", 0, 0, -120), 950)
    fish:tween("Linear", "PosX", 270, 580, 950)
    fish:tween("SineCirc", "PosY", 640, 480, 950)
    three_stage_alpha_tween(fish, 1000, 350)
    
    local water1 = view.new_sprite("bg5/03fish_water", scene_)
    water1:set_center_aligned(true)
    water1:set_depth(-100)
    water1:set_pos(315, 595)
    water1:tween("Linear", "Scale", ffi.new("v3", 0.5, 0.5, 1), ffi.new("v3", 1.5, 1, 1), 1000)
    water1:tween("SineCirc", "Alpha", 0, 255, 1000, 0, function(self) self:remove() end)
    
    local water2_root = view.new_sprite("nothing", scene_, 0, 0, true)
    water2_root:set_depth(-200)
    water2_root:set_pos(330, 604)
    --water2_root:tween("OSine", "Scale", ffi.new("v3", 0.8, 0.4, 1), ffi.new("v3", 1, 1.1, 1), 600)
    
    local water2 = view.new_sprite("bg5/04fish_water_out", water2_root)
    water2:set_alpha(0)
    water2:set_pos(-15, -60)
    water2:tween("Linear", "Alpha", 255, 0, 500, 0, function(self) self:remove() end, 250)
    
    local water3 = view.new_sprite("bg5/05fish_water_in", scene_)
    water3:set_alpha(0)
    water3:set_depth(-200)
    water3:set_pos(532, 584)
    water3:set_center_aligned(true)
    water3:tween("Linear", "Alpha", 255, 0, 750, 0, function(self) self:remove() end, 800)
  end
  local fish_timer = event.on_timer("global", fish_effect, 10000, -1)
  
  local monster_effect = function()
    local monster = view.new_sprite("bg5/06monster", scene_)
    monster:set_depth(-300)
    monster:set_pos(850, 600)
    monster:tween("Linear", "Pos2D", ffi.new("v2", 1280, 520), ffi.new("v2", 1050, 500), 3000)
    local step1 = function()
      monster:tween("Linear", "Pos2D", ffi.new("v2", 1050, 500), ffi.new("v2", 660, 500), 4000)
    end
    local step2 = function()
      monster:tween("Linear", "Pos2D", ffi.new("v2", 660, 500), ffi.new("v2", 540, 650), 2500)
    end
    local h1 = event.on_timer("global", step1, 3000) 
    local h2 = event.on_timer("global", step2, 7000)
  end
  local monster_timer = event.on_timer("global", monster_effect, 10000, -1)
  
  local layer8_root = view.new_sprite("nothing", scene_, 0, 0)
  layer8_root:set_depth(-800)
  layer8_root:set_pos(1148, 198)
  layer8_root:tween("SineCirc", "Rotation", ffi.new("v3", 0, 0, -6), ffi.new("v3", 0, 0, 6), 4000, -1) 
  
  layer8_ = view.new_sprite("bg5/08bag", layer8_root)
  layer8_:set_pos( -45, -12)
  
  layer9_ = view.new_sprite("bg5/09foreground", scene_)
  layer9_:set_depth(-900)
  layer9_:set_pos( 0, 720 - layer9_:get_size_y() )
  
  local debug_timer = event.on_timer("global", function() view.debug_hack() end, 1000, -1)
end

function cleanup()
  layer0_ = nil
  layer1_ = nil
  layer2_ = nil
  layer3_ = nil
  layer4_ = nil
  layer5_ = nil
  layer6_ = nil
  layer7_ = nil
  layer8_ = nil
  layer9_ = nil
  event.clear_timers_of("global")
  collectgarbage 'collect'
end
