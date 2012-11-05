local ffi   = require 'ffi'
local C     = ffi.C
local view  = require 'rc/script/ui/view'
local ui    = require 'rc/script/ui/ui'

local ask_panel_ = nil

local function update(demo, scene, data)
  local screen_w  = C.Get_SCREEN_W()
  local screen_h  = C.Get_SCREEN_H()
  ask_panel_  = ui.new_image{parent=scene, path='nothing', w=1024, h=576, center=true}
  ask_panel_:set_pos(screen_w/2, screen_h/2)
  ask_panel_:set_color(0, 0, 0)
  ask_panel_:set_alpha(192)
  
  local guide_x=-460
  local guide_y=-240
  ask_panel_.guide  = ui.new_text{ parent=ask_panel_._cdata, size=24,
                                title = "Shoot [left button] these:                              Match these:\n\n          colored cubes\n\n          colored cubes with chain\n\n          garbages (won't match)\n\nSpeed-Up: [right button]                                Pause: [middle button]" }
  ask_panel_.text   = ui.new_text{ parent=ask_panel_._cdata, size=30, center=true,
                                title="Tutorial is 90 seconds long.\nTry to make 8 or more matches in the first 20 seconds." }
  ask_panel_.guide:set_pos(guide_x, guide_y)
  ask_panel_.guide:set_depth(-50)
  ask_panel_.text:set_pos(0, 90)
  ask_panel_.text:set_depth(-50)
  
  ask_panel_.cube1 = ui.new_image { parent = ask_panel_._cdata, path = 'cubes/cube1', w=48, h=48 }
  ask_panel_.cube1:set_color(255, 0, 0)
  ask_panel_.cube1:set_pos(10+guide_x, 45+guide_y)
  ask_panel_.cube2 = ui.new_image { parent = ask_panel_._cdata, path = 'cubes/cube2', w=48, h=48 }
  ask_panel_.cube2:set_color(255, 0, 0)
  ask_panel_.cube2:tween("SineCirc", "GradientEmissive", 0, 144, 500, -1)
  ask_panel_.cube2:set_pos(10+guide_x, 95+guide_y)
  ask_panel_.cube3 = ui.new_image { parent = ask_panel_._cdata, path = 'cubes/cube3', w=48, h=48 }
  ask_panel_.cube3:set_pos(10+guide_x, 145+guide_y)
  ask_panel_.cube1:set_depth(-50)
  ask_panel_.cube2:set_depth(-50)
  ask_panel_.cube3:set_depth(-50)
  
  ask_panel_.cube4 = ui.new_image { parent = ask_panel_._cdata, path = 'cubes/cube1', w=48, h=48 }
  ask_panel_.cube5 = ui.new_image { parent = ask_panel_._cdata, path = 'cubes/cube2', w=48, h=48 }
  ask_panel_.cube6 = ui.new_image { parent = ask_panel_._cdata, path = 'cubes/cube3', w=48, h=48 }
  ask_panel_.cube7 = ui.new_image { parent = ask_panel_._cdata, path = 'cubes/cube1', w=48, h=48 }
  ask_panel_.cube8 = ui.new_image { parent = ask_panel_._cdata, path = 'cubes/cube2', w=48, h=48 }
  ask_panel_.cube9 = ui.new_image { parent = ask_panel_._cdata, path = 'cubes/cube3', w=48, h=48 }
  ask_panel_.cube0 = ui.new_image { parent = ask_panel_._cdata, path = 'cubes/cube4', w=48, h=48 }
  ask_panel_.cube4:set_color(0, 255, 0)
  ask_panel_.cube5:set_color(0, 255, 0)
  ask_panel_.cube6:set_color(0, 255, 0)
  ask_panel_.cube7:set_color(0, 0, 255)
  ask_panel_.cube8:set_color(0, 0, 255)
  ask_panel_.cube9:set_color(0, 0, 255)
  ask_panel_.cube0:set_color(0, 0, 255)
  ask_panel_.cube4:set_pos(550+guide_x, 60+guide_y)
  ask_panel_.cube4:tween("Linear", "Pos2D", ffi.new("v2", 550+guide_x, 30+guide_y), ffi.new("v2", 550+guide_x, 60+guide_y), 1000, -1)
  ask_panel_.cube5:set_pos(598+guide_x, 60+guide_y)
  ask_panel_.cube6:set_pos(646+guide_x, 60+guide_y)
  ask_panel_.cube7:set_pos(550+guide_x, 130+guide_y)
  ask_panel_.cube8:set_pos(598+guide_x, 130+guide_y)
  ask_panel_.cube9:set_pos(646+guide_x, 130+guide_y)
  ask_panel_.cube0:set_pos(694+guide_x, 130+guide_y)
  ask_panel_.cube9:tween("Linear", "Pos2D", ffi.new("v2", 646+guide_x, 105+guide_y), ffi.new("v2", 646+guide_x, 130+guide_y), 1000, -1)
  ask_panel_.cube4:set_depth(-50)
  ask_panel_.cube5:set_depth(-50)
  ask_panel_.cube6:set_depth(-50)
  ask_panel_.cube7:set_depth(-50)
  ask_panel_.cube8:set_depth(-50)
  ask_panel_.cube9:set_depth(-50)
  ask_panel_.cube0:set_depth(-50)
  
  ask_panel_.ok = ui.new_text{ parent=ask_panel_._cdata, title='OK', size=30, center=true }
  ask_panel_.ok:set_scale(1.5)
  ask_panel_.ok:set_pos(0, 200)
  ask_panel_.ok:set_depth(-50)
  ask_panel_.ok:on_press( function(self)
                            demo:eventual_resume()
                            ask_panel_:set_visible(false)
                          end )
  demo:eventual_pause()
end

return {
  update  = update
}