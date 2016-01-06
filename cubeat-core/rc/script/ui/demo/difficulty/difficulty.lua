local ffi   = require 'ffi'
local C     = ffi.C
local view  = require 'rc/script/ui/view'
local ui    = require 'rc/script/ui/ui'
local switch= require 'rc/script/ui/demo/switch/switch'

local root_ = nil

local function init(demo, parent, data)
  local menu = {}
  
  root_ = view.new_sprite("blahblah", parent, 0, 0, true)
  root_:set_pos(view.GET_SCREEN_W()/2, view.GET_SCREEN_H()/2)
  
  menu.bg         = ui.new_image9s{ parent=root_, path='textarea2', x=0, y=0, 
                                    w=850, h=420, w1=34, w2=32, h1=38, h2=35, center=true }
                           
  menu.top_text   = ui.new_text{ parent=parent, x=view.GET_SCREEN_W()/2, y=20, title="VS CPU Mode", 
                                 size=32, depth=-30, center=true}
  menu.top_text:set_scale(1.3)                         
  menu.title      = ui.new_text{ parent=root_, y=-125, font="GN-KillGothic", size=32, title="Difficulty", depth=-30, center=true}
  menu.title:set_scale(1.3)
  
  menu.blocker_top1    = ui.new_image{ parent=parent, path='blocker', w=view.GET_SCREEN_W()+50, h=60, 
                                       x = -10, y = -5 }
  menu.blocker_top2    = ui.new_image{ parent=parent, path='blocker', w=view.GET_SCREEN_W()+50, h=60, 
                                       x = -10, y = 5 }
  menu.blocker_bottom1 = ui.new_image{ parent=parent, path='blocker', w=view.GET_SCREEN_W()+50, h=60, 
                                       x = -10, y = view.GET_SCREEN_H()-50 }
  menu.blocker_bottom2 = ui.new_image{ parent=parent, path='blocker', w=view.GET_SCREEN_W()+50, h=60,
                                       x = -10, y = view.GET_SCREEN_H()-60 }
                                       
  menu.blocker_top1:set_alpha(96)
  menu.blocker_top2:set_alpha(96)
  menu.blocker_bottom1:set_alpha(96)
  menu.blocker_bottom2:set_alpha(96)
  menu.blocker_top1:set_color(0,0,0)
  menu.blocker_top2:set_color(0,0,0)
  menu.blocker_bottom1:set_color(0,0,0)
  menu.blocker_bottom2:set_color(0,0,0)
  menu.blocker_top1:set_rotation(0.7)
  menu.blocker_top2:set_rotation(1)
  menu.blocker_bottom1:set_rotation(-0.7)
  menu.blocker_bottom2:set_rotation(-1)
  
  local pos1s, pos1e = ffi.new("v2", -10, -65), ffi.new("v2", -10, -5)
  local pos2s, pos2e = ffi.new("v2", -10, -55), ffi.new("v2", -10, 5)
  local pos3s, pos3e = ffi.new("v2", -10, view.GET_SCREEN_H()), ffi.new("v2", -10, view.GET_SCREEN_H()-50)
  local pos4s, pos4e = ffi.new("v2", -10, view.GET_SCREEN_H()-10), ffi.new("v2", -10, view.GET_SCREEN_H()-60)
  menu.blocker_top1:tween('OSine', 'Pos2D', pos1s, pos1e, 500)
  menu.blocker_top2:tween('OSine', 'Pos2D', pos2s, pos2e, 500)
  menu.blocker_bottom1:tween('OSine', 'Pos2D', pos3s, pos3e, 500)
  menu.blocker_bottom2:tween('OSine', 'Pos2D', pos4s, pos4e, 500)
  
  menu.btn_easy     = ui.new_button{ parent=root_, path='icon_1', title='easy',       x=-270, y=30, depth=-30, center=true }
  menu.btn_normal   = ui.new_button{ parent=root_, path='icon_1', title='normal',     x= -90, y=30, depth=-30, center=true }
  menu.btn_hard     = ui.new_button{ parent=root_, path='icon_1', title='hard',       x=  90, y=30, depth=-30, center=true }
  menu.btn_veryhard = ui.new_button{ parent=root_, path='icon_1', title='very\nhard', x= 270, y=30, depth=-30, center=true }
  
  menu.btn_easy     :on_focus(view.Input1)
  menu.btn_normal   :on_focus(view.Input1)
  menu.btn_hard     :on_focus(view.Input1)
  menu.btn_veryhard :on_focus(view.Input1)
  
  menu.btn_easy:on_press    ( function(self)
                                data.level = 0
                                demo:init_mode(data.game_mode, data.c1p, data.c2p, data.sconf, data.level)
                              end, view.Input1_left )
  menu.btn_normal:on_press  ( function(self)
                                data.level = 1
                                demo:init_mode(data.game_mode, data.c1p, data.c2p, data.sconf, data.level)
                              end, view.Input1_left )
  menu.btn_hard:on_press    ( function(self)
                                data.level = 2
                                demo:init_mode(data.game_mode, data.c1p, data.c2p, data.sconf, data.level)
                              end, view.Input1_left )
  menu.btn_veryhard:on_press( function(self)
                                data.level = 3
                                demo:init_mode(data.game_mode, data.c1p, data.c2p, data.sconf, data.level)
                              end, view.Input1_left )

  menu.btn_back   = ui.new_text{ parent=root_, x= - view.GET_SCREEN_W()/2 + 10, y=view.GET_SCREEN_H()/2-60, 
                                 size=32, title='< go back', depth=-60 }
  menu.btn_back:set_scale(1.5)
  menu.btn_back:on_press(function(self)
    switch.load_page('select', nil, { game_mode = data.game_mode, title = "VS CPU Mode" })
  end)

  return menu
end

return {
  init = init
}
