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
  local l0 = new_layer{ name = "bg5/99complete", parent = scene_ } 
  
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
