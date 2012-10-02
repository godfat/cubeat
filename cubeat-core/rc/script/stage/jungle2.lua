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
  --local l0 = new_layer{ name = "bg2/99complete", parent = scene_ } 
  local l1 = new_layer{ name = "bg2/01bg", parent = scene_ }
  local l2 = new_layer{ name = "bg2/02whale", parent = scene_ } 
  l2:set_pos(14, 260)
  -- local l3 = new_layer{ name = "bg2/03whalesmile", scene_ } -- When should we face-off?
  -- l3:set_pos(14, 260)
  
  local l4 = new_layer{ name = "bg2/04glasses", parent = scene_ }
  l4:set_pos(202, 307)
  l4:tween("SineCirc", "Pos2D", ffi.new("v2", 202, 309), ffi.new("v2", 205, 301), 4000, -1)
  
  local l5 = new_layer{ name = "radial_bloom", parent = scene_, width = 960, height = 640 }
  l5:set_pos(200, 30)
  l5:tween("SineCirc", "Alpha", 192, 96, 10000, -1)
  
  local debug_timer = event.on_timer("global", function() view.debug_hack() end, 1000, -1)
end

function cycle()
end

function set_release()
  need_release_ = true
end

function cleanup()
  stage_base.cleanup(need_release_)
  event.clear_timers_of("global")
  collectgarbage 'collect'
end
