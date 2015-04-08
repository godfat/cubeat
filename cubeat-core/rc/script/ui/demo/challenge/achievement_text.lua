local ffi       = require 'ffi'
local C         = ffi.C
local view      = require 'rc/script/ui/view'
local ui        = require 'rc/script/ui/ui'
local event     = require 'rc/script/event/event'
local parameter = require 'rc/script/ui/demo/challenge/parameter'

local achievement_ui_text_

local achievement_string = {
  highest_chain_4 = "Chain level 4 achieved!",
  highest_chain_6 = "Chain level 6 achieved!",
}

local function create(scene)
  achievement_ui_text_ = ui.new_text{ parent = scene, title = '_achievement_string_', x=640, y=620, size=32, depth=-1000, center = true }
  achievement_ui_text_:set_scale(1.2)
  achievement_ui_text_:set_visible(false)
end

local function pop_achievement_ui(key)
  if achievement_string[key] then
    achievement_ui_text_:change_text( achievement_string[key] )
    achievement_ui_text_:set_visible(true)
    achievement_ui_text_:tween("OElastic", "Scale", ffi.new("v3", 0.1, 0.1, 0.1), ffi.new("v3", 1.2, 1.2, 1.2), 500)
    event.on_timer("ui", function()
      achievement_ui_text_:set_visible(false)
    end, 2000)
  end
end

return{
  create              = create,
  pop_achievement_ui  = pop_achievement_ui
}