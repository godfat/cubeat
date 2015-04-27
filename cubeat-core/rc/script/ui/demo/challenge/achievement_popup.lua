local ffi       = require 'ffi'
local C         = ffi.C
local view      = require 'rc/script/ui/view'
local ui        = require 'rc/script/ui/ui'
local event     = require 'rc/script/event/event'
local parameter = require 'rc/script/ui/demo/challenge/parameter'

local achievement_popup_bg_
local achievement_popup_
local achievement_desc_

local function create(scene)
  achievement_popup_bg_ = ui.new_image{ parent = scene, path = 'stroke', x = 0, y = 672, w = 96, h = 640, depth=-500 }
  achievement_popup_bg_:set_rotation(90)
  achievement_popup_bg_:set_color(0, 0, 0)
  achievement_popup_bg_:set_visible(false)
  achievement_popup_bg_:set_pickable(false)

  achievement_popup_ = ui.new_text{ parent = scene, title = '_achievement_string_', x=20, y=600, size=32, depth=-600 }
  achievement_popup_:set_scale(1.2)
  achievement_popup_:set_visible(false)
  achievement_popup_:set_pickable(false)
  
  -- decide lang config here
  achievement_desc_ = require 'rc/script/ui/demo/challenge/achievement_desc_EN'
end

local function pop_achievement_ui(key)
  if achievement_desc_[key] then
    achievement_popup_bg_:set_visible(true)
    
    achievement_popup_:change_text( "Achieved: " .. achievement_desc_[key].name )
    achievement_popup_:set_visible(true)
    achievement_popup_:tween("OElastic", "Scale", ffi.new("v3", 0.1, 0.1, 0.1), ffi.new("v3", 1.2, 1.2, 1.2), 500)
    event.on_timer("ui", function()
      achievement_popup_bg_:set_visible(false)
      achievement_popup_:set_visible(false)
    end, 2000)
  else
    print( "\nLua: achievement_popup no such key: ", key )
  end
end

return{
  create              = create,
  pop_achievement_ui  = pop_achievement_ui
}