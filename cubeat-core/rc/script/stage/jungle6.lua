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
  local l1 = new_layer{ name = "bg6/01bg", parent = scene_ } 
  
  local l1_2 = new_layer{ name = "bg6/015tableback", parent = scene_ }
  l1_2:set_pos(34, 472)
  
  local l2 = new_layer{ name = "bg6/02blade", parent = scene_, center = true }
  l2:set_pos(127, 489)
  l2:tween("Linear", "Rotation", ffi.new("v3",0,0,0), ffi.new("v3",0,0,360), 500, -1)
  
  local l3 = new_layer{ name = "bg6/03tablefront", parent = scene_ }
  l3:set_pos(0, 464)
  
  local l4 = new_layer{ name = "bg6/04whale", parent = scene_ }
  l4:set_pos(465, 414)
  l4:tween("SineCirc", "Pos2D", ffi.new("v2",465, 420), ffi.new("v2", 465, 410), 3000, -1)
  
  local l5 = new_layer{ name = "bg6/05bubble1", parent = scene_ }
  l5:set_pos(440, 420)
  l5:tween("SineCirc", "Alpha", 0, 255, 3000, -1)
  
  local l6 = new_layer{ name = "bg6/06bubble2", parent = scene_ }
  l6:set_pos(440, 420)
  l6:tween("SineCirc", "Alpha", 255, 0, 3000, -1)
  
  -- local l7 = new_layer{ name = "bg6/07water1", parent = scene_ }
  -- l7:set_pos(371, 375)
  -- l7:tween("SineCirc", "Alpha", 0, 160, 3000, -1)
  
  local l8 = new_layer{ name = "bg6/08water2", parent = scene_ }
  l8:set_pos(371, 375)
  l8:tween("SineCirc", "Alpha", 0, 160, 3000, -1)
  
  local l9 = new_layer{ name = "bg6/09water3", parent = scene_ }
  l9:set_pos(371, 375)
  l9:tween("SineCirc", "Alpha", 160, 0, 3000, -1)
  
  local l10 = new_layer{ name = "bg6/10dove", parent = scene_ }
  l10:set_pos(1083, 206)
  -- When to make dove disappear?
  
  local l11 = new_layer{ name = "bg6/11eye1", parent = scene_, center = true }
  l11:set_pos(1194, 352)
  
  -- local l12 = new_layer{ name = "bg6/12eye2", parent = scene_, center = true }
  -- l12:set_pos(1194, 352) -- When to make eye blinks??
  
  local l13 = new_layer{ name = "bg6/13light1", parent = scene_ }
  l13:set_pos(310, 520)
  l13:set_blending("VIVID_LIGHT")
  
  local l14 = new_layer{ name = "bg6/14light2", parent = scene_ }
  l14:set_pos(194, 290)
  l14:set_blending("VIVID_LIGHT")
  
  local l15 = new_layer{ name = "bg6/15light3", parent = scene_ }
  l15:set_pos(260, 80)
  l15:set_blending("VIVID_LIGHT")
  
  local l16 = new_layer{ name = "bg6/16light4", parent = scene_ }
  l16:set_pos(510, 10)
  l16:set_blending("SCREEN")
  
  local l17 = new_layer{ name = "bg6/17light5", parent = scene_ }
  l17:set_pos(940, 160)
  l17:set_blending("SCREEN")
  
  local l18 = new_layer{ name = "bg6/18light6", parent = scene_ }
  l18:set_pos(250, 0)
  l18:set_blending("SCREEN")
  
  -- local l0 = new_layer{ name = "bg6/99complete", parent = scene_ } 
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
