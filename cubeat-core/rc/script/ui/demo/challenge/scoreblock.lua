local ffi    = require 'ffi'
local C      = ffi.C
local view   = require 'rc/script/ui/view'
local ui     = require 'rc/script/ui/ui'

local blocker_
local score_
local score_r_
local score_b_

local function create_score_block(scene)
  blocker_ = ui.new_image{ parent=scene, path="blahblah", x=1000, y=400, w=160, h=80, depth=100, center=true, alpha=128 }
  blocker_:set_color(0, 0, 0)
  
  score_ = ui.new_text{ parent=scene, x=985, y=400, size=32, title='score', center=true }
  score_:set_scale(1.5)
  local s1 = ffi.new("v2", 985, -200)
  local e1 = ffi.new("v2", 985, 400)
  blocker_:tween("Linear", "Pos2D", s1, e1, 800, 0, nil, 0)
  score_:tween("Linear", "Pos2D", s1, e1, 800, 0, nil, 0)
end

local function create_score_block_double(scene)
  blocker_ = ui.new_image{ parent=scene, path="blahblah", x=1000, y=400, w=160, h=80, depth=100, center=true, alpha=128 }
  blocker_:set_color(0, 0, 0)
  
  score_r_ = ui.new_text{ parent=scene, x=945, y=400, size=32, title='score', center=true }
  score_r_:set_scale(1.5)
  score_b_ = ui.new_text{ parent=scene, x=1025, y=400, size=32, title='score', center=true }
  score_b_:set_scale(1.5)
  local s1 = ffi.new("v2", 985, -200)
  local e1 = ffi.new("v2", 985, 400)
  local s2 = ffi.new("v2", 945, -200)
  local e2 = ffi.new("v2", 945, 400)
  local s3 = ffi.new("v2", 1025, -200)
  local e3 = ffi.new("v2", 1025, 400)
  blocker_:tween("Linear", "Pos2D", s1, e1, 800, 0, nil, 0)
  score_r_:tween("Linear", "Pos2D", s2, e2, 800, 0, nil, 0)
  score_b_:tween("Linear", "Pos2D", s3, e3, 800, 0, nil, 0)
end

local function set_score(v)
  score_:change_text( tostring(v) )
end

local function set_score_double(r, b)
  score_r_:change_text( tostring(r) )
  score_b_:change_text( tostring(b) )
  score_r_:set_color(255, 0, 0)
  score_b_:set_color(0, 0, 255)
end

local function remove_score_block()
  if blocker_ then blocker_:remove() end
  if score_ then score_:remove() end
  if score_r_ then score_r_:remove() end
  if score_b_ then score_b_:remove() end
end

return{
  create_score_block        = create_score_block,
  create_score_block_double = create_score_block_double,
  --
  set_score                 = set_score,
  set_score_double          = set_score_double,
  --
  remove_score_block        = remove_score_block,
}