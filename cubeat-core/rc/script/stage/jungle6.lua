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
  --local l0 = new_layer{ name = "bg6/99complete", parent = scene_ } 
  local l1 = new_layer{ name = "bg6/01bg", parent = scene_ } 
  
  local l2 = new_layer{ name = "bg6/02blade", parent = scene_, center = true }
  l2:set_pos(127, 489)
  l2:tween("Linear", "Rotation", ffi.new("v3",0,0,0), ffi.new("v3",0,0,360), 500, -1)
  
  local l3 = new_layer{ name = "bg6/03tablefront", parent = scene_ }
  l3:set_pos(0, 464)
  
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
