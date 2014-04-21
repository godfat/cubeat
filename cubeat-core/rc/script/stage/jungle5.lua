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
  -- local l0 = new_layer{ name = "bg5/99complete", parent = scene_ } 
  
  local l1 = new_layer{ name = "bg5/01bg", parent = scene_ }
  
  local fish_effect = function()
    local fish = new_layer{ name = "bg5/02fish", parent = scene_, layer = 2, center = true }
    fish:set_pos(290, 500)
    fish:tween("Linear", "Rotation", ffi.new("v3", 0, 0, 60), ffi.new("v3", 0, 0, -120), 950)
    fish:tween("Linear", "PosX", 270, 580, 950)
    fish:tween("SineCirc", "PosY", 640, 480, 950)
    three_stage_alpha_tween(fish, 1000, 350)
    
    local water1 = new_layer{ name = "bg5/03fish_water", parent = scene_, layer = 2, center = true }
    water1:set_pos(315, 595)
    water1:tween("Linear", "Scale", ffi.new("v3", 0.5, 0.5, 1), ffi.new("v3", 1.5, 1, 1), 1000)
    water1:tween("SineCirc", "Alpha", 0, 255, 1000, 0, function(self) self:remove() end)
   
    local water2 = new_layer{ name = "bg5/04fish_water_out", parent = scene_, layer = 2 }
    water2:set_alpha(0)
    water2:set_pos(315, 544)
    water2:tween("Linear", "Alpha", 255, 0, 500, 0, function(self) self:remove() end, 300)
    
    local water3 = new_layer{ name = "bg5/05fish_water_in", parent = scene_, layer = 2, center = true }
    water3:set_alpha(0)
    water3:set_pos(532, 584)
    water3:tween("Linear", "Alpha", 255, 0, 750, 0, function(self) self:remove() end, 800)
  end
  local fish_timer = event.on_timer("global", fish_effect, 10000, -1)
  
  local monster_effect = function()
    local monster = new_layer{ name = "bg5/06monster", parent = scene_, layer = 3 }
    monster:set_pos(850, 600)
    monster:tween("Linear", "Pos2D", ffi.new("v2", 1280, 520), ffi.new("v2", 1050, 500), 3000)
    local step1 = function()
      monster:tween("Linear", "Pos2D", ffi.new("v2", 1050, 500), ffi.new("v2", 660, 500), 4000)
    end
    local step2 = function()
      monster:tween("Linear", "Pos2D", ffi.new("v2", 660, 500), ffi.new("v2", 540, 650), 2500, 0, function(self) self:remove() end)
    end
    local h1 = event.on_timer("global", step1, 3000) 
    local h2 = event.on_timer("global", step2, 7000)
  end
  local monster_timer = event.on_timer("global", monster_effect, 10000, -1)
  
  local l4_root = new_layer{ name = "nothing", parent = scene_, width = 0, height = 0 }
  l4_root:set_pos(1148, 198)
  l4_root:tween("SineCirc", "Rotation", ffi.new("v3", 0, 0, -6), ffi.new("v3", 0, 0, 6), 4000, -1) 
  
  local l4 = new_layer{ name = "bg5/08bag", parent = l4_root }
  l4:set_pos(-45, -12)
  
  local l5 = new_layer{ name = "bg5/09foreground", parent = scene_ }
  l5:set_pos( 0, 720 - l5:get_size_y() )
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
