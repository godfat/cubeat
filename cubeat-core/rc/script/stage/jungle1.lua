require 'rc/script/strict'
local ffi   = require 'ffi'
local C     = ffi.C
local view  = require 'rc/script/ui/view'
local event = require 'rc/script/event/event'

local stage_base              = require 'rc/script/stage/base_function'
local new_layer               = stage_base.new_layer
local three_stage_alpha_tween = stage_base.three_stage_alpha_tween

local scene_ 
local need_release_ = false

function init(scene)
  scene_ = ffi.cast("pScene*", scene)
  --local l0 = new_layer{ name = "bg1/99complete", parent = scene_ } 
  
  local l1 = new_layer{ name = "bg1/01sky", parent = scene_ }
  l1:set_pos( 1280 - l1:get_size_x(), 0 )
  
  local produce_cloud1 = function()
    local local_cloud = new_layer{ name = "bg1/02cloud1", parent = scene_, layer = 2 } 
    local_cloud:tween("Linear", "Pos2D", ffi.new("v2", 910, 245), ffi.new("v2", 1020, 245), 20000, -1)
    local_cloud:tween("SineCirc", "Alpha", 0, 255, 20000, 0, function(self) self:remove() end)
  end
  produce_cloud1()
  local cloud1_timer = event.on_timer("jungle1", produce_cloud1, 10000, -1)
  
  local produce_cloud2 = function()
    local local_cloud = new_layer{ name = "bg1/03cloud2", parent = scene_, layer = 3 }
    local_cloud:set_pos(425, 300)
    local_cloud:tween("Linear", "Pos2D", ffi.new("v2", 400, 300), ffi.new("v2", 520, 300), 15000, -1)
    local_cloud:tween("SineCirc", "Alpha", 64, 255, 15000, 0, function(self) self:remove() end)
  end
  produce_cloud2()
  local cloud2_timer = event.on_timer("jungle1", produce_cloud2, 7500, -1)
  
  local produce_cloud3 = function()
    local local_cloud = new_layer{ name = "bg1/04cloud3", parent = scene_, layer = 4 }
    local pos_x = 1280 - local_cloud:get_size_x()
    local_cloud:set_pos( pos_x, 0 )
    local_cloud:tween("Linear", "Pos2D", ffi.new("v2", pos_x, 0), ffi.new("v2", pos_x+30, 0), 30000, -1)
    three_stage_alpha_tween(local_cloud, 30000, 4000)
  end
  produce_cloud3()
  local cloud3_timer = event.on_timer("jungle1", produce_cloud3, 25000, -1)
    
  local produce_cloud4 = function()
    local local_cloud = new_layer{ name = "bg1/05cloud4", parent = scene_, layer = 5 }
    local_cloud:set_pos(325, 0)
    local_cloud:tween("Linear", "Pos2D", ffi.new("v2", 315, 0), ffi.new("v2", 345, 0), 10000, -1)
    three_stage_alpha_tween(local_cloud, 10000, 2000)
  end
  produce_cloud4()
  local cloud4_timer = event.on_timer("jungle1", produce_cloud4, 8000, -1)
  
  local l6 = new_layer{ name = "bg1/06cube", parent = scene_ }
  l6:set_pos(500, 107)
  
  local l7 = new_layer{ name = "bg1/07flyingwhale", parent = scene_ }
  l7:set_pos(1078, 199)
  l7:tween("SineCirc", "Pos2D", ffi.new("v2", 1070, 199), ffi.new("v2", 1090, 199), 7000, -1)
  
  local l8 = new_layer{ name = "bg1/09building", parent = scene_ } 
  
  local l9 = new_layer{ name = "bg1/08whaleface", parent = scene_ }  
  l9:set_pos(180, 0)
end

function cycle()
end

function set_release()
  need_release_ = true
end

function cleanup()
  stage_base.cleanup(need_release_)
  event.clear_timers_of("jungle1")
  collectgarbage 'collect'
end
