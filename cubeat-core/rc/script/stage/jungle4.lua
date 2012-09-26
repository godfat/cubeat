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
  --local l0 = new_layer{ name = "bg4/99complete", parent = scene_ } 
  local l1 = new_layer{ name = "bg4/01bg", parent = scene_ }
  local l2 = new_layer{ name = "bg4/02man1", parent = scene_ }
  l2:set_pos(206, 293)
  
  -- local l3 = new_layer{ name = "bg4/04man3", parent = scene_ } -- When should we make him showoff muscle?
  -- l3:set_pos(204, 290)
  
  local eye_sparkle = function()
    local sp = new_layer{ name = "bg4/05maneye", parent = scene_, layer = 3 }
    sp:set_pos(239, 332)
    sp:set_alpha(0)
    sp:tween("Linear", "Alpha", 255, 0, 1000, 0, function(self) self:remove() end)
  end
  local sparkle_timer = event.on_timer("global", eye_sparkle, 2000, -1)
  
  local l5 = new_layer{ name = "bg4/06whale", parent = scene_ }
  l5:set_pos(739, 0)
  l5:lazy_fix_alpha_artifact()
  l5:tween("SineCirc", "Pos2D", ffi.new("v2", 739, -5), ffi.new("v2", 745, 0), 8000, -1)
  
  local l6 = new_layer{ name = "radial_bloom", parent = scene_, width = 1024, height = 640, center = true }
  l6:set_pos(675, 250)
  l6:tween("SineCirc", "Alpha", 144, 64, 7000, -1)  
  
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
