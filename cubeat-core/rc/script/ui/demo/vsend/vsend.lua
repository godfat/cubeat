local ffi     = require 'ffi'
local C       = ffi.C
local view    = require 'rc/script/ui/view'
local ui      = require 'rc/script/ui/ui'
local config  = require 'rc/script/ui/demo/storyend/config'
local select_config       = require 'rc/script/ui/demo/select/config'
local endingcheck_config  = require 'rc/script/ui/demo/endingcheck/config'

local screen_w_ = C.Get_SCREEN_W()
local screen_h_ = C.Get_SCREEN_H()
local center_x = screen_w_/2
local center_y = screen_h_/2

local blocker_
local win_t_
local lose_t_
local end_text_
local end_text2_
local char_big1_
local char_big2_

local function hide()
  blocker_:set_visible(false)
  win_t_:set_visible(false)
  lose_t_:set_visible(false)
  end_text_:set_visible(false)
  end_text2_:set_visible(false)
  char_big1_:set_visible(false)
  char_big2_:set_visible(false)
end

local function create(scene)
  
  blocker_  = ui.new_image{ parent=scene, path="blocker", x=center_x, y=center_y, w=screen_w_, h=368, depth=-10, alpha=144, center=true }
  blocker_:set_gradient_diffuse(0)
  
  win_t_  = ui.new_text { parent=scene, path="Win" , x=384, y=192, size=32, depth=-450, center=true }
  lose_t_ = ui.new_text { parent=scene, path="Lose", x=384, y=192, size=32, depth=-450, center=true }
  
  end_text_ = ui.new_text{ parent=scene, x=0, y=0, size=30, title="play again?", center=true }
  end_text2_= ui.new_text{ parent=scene, x=0, y=0, size=30, title="\nyes: left click\nleave: right click", center=true }
  
  char_big1_ = ui.new_image{ parent=scene, path="char1_new/glad", x=0, y=0, w=432, h=648, center=true }
  char_big2_ = ui.new_image{ parent=scene, path="char1_new/glad", x=0, y=0, w=432, h=648, center=true }
  
  --
  
  hide()
  
end

local function show(demo, game_mode, p1_win)
  blocker_:set_alpha(0)
  blocker_:tween("Linear", "Alpha", 0, 144, 500, 0, nil, 1500)
  blocker_:set_visible(true)
  blocker_:set_pos(center_x, center_y)
  
  win_t_:set_visible(true)
  win_t_:set_scale(0)
  lose_t_:set_visible(true)
  lose_t_:set_scale(0)
  
  local pos1 = {x=screen_w_/4, y=screen_h_/2}
  local pos2 = {x=screen_w_/4*3, y=screen_h_/2}
  local char_big_filename1 = "char"..tostring(select_config.ch_choose[1]).."_new"
  local char_big_filename2 = "char"..tostring(select_config.ch_choose[2]).."_new"
  
  if p1_win==false then
    lose_t_:set_pos(pos1.x, pos1.y)
    win_t_:set_pos(pos2.x, pos2.y)
    if game_mode == endingcheck_config.GM_PVC then
      demo:play_sound("3/3c/lose.wav")
    else
      demo:play_sound("3/3c/win.wav")
    end
    
    char_big_filename1 = char_big_filename1 .. ("/sad")
    char_big_filename2 = char_big_filename2 .. ("/glad")
  else
    lose_t_:set_pos(pos2.x, pos2.y)
    win_t_:set_pos(pos1.x, pos1.y)
    demo:play_sound("3/3c/win.wav")
    
    char_big_filename1 = char_big_filename1 .. ("/glad")
    char_big_filename2 = char_big_filename2 .. ("/sad")
  end
  
  --char_big1_ = ui.new_image{ parent=scene_, path=char_big_filename1, x=pos1.x-32, y=pos1.y+64, w=432, h=648, center=true }
  --char_big2_ = ui.new_image{ parent=scene_, path=char_big_filename2, x=pos2.x+32, y=pos2.y+64, w=432, h=648, center=true }
  char_big1_:set_texture(char_big_filename1)
  char_big2_:set_texture(char_big_filename2)
  char_big2_:texture_flipH()
  local s1 = ffi.new("v2", pos1.x-32, -pos1.y+64)
  local e1 = ffi.new("v2", pos1.x-64, pos1.y-48)
  local s2 = ffi.new("v2", pos2.x+32, -pos2.y+64)
  local e2 = ffi.new("v2", pos2.x+64, pos2.y-48)
  char_big1_:set_visible(true)
  char_big2_:set_visible(true)
  char_big1_:tween("OBounce", "Pos2D", s1, e1, 1500, 0, nil, 0)
  char_big2_:tween("OBounce", "Pos2D", s2, e2, 1500, 0, nil, 0)
  
  demo:hide_character_animations()
  
  win_t_:set_depth(-450)
  win_t_:tween("OElastic", "Scale", 0, 1, 1000, 0, nil, 1500)
  lose_t_:set_depth(-450)
  lose_t_:tween("OElastic", "Scale", 0, 1, 1000, 0, nil, 1500)
  
  end_text_:set_visible(true)
  end_text_:change_text("Retry")
  end_text_:set_pos(center_x, center_y - 60)
  end_text_:set_alpha(0)
  end_text_:set_scale(1.3)
  end_text_:set_depth(-450)
  end_text_:tween("Linear", "Alpha", 0, 255, 500, 0, nil, 2500)
  end_text2_:set_visible(true)
  end_text2_:change_text("Leave")
  end_text2_:set_pos(center_x, center_y + 60)
  end_text2_:set_alpha(0)
  end_text2_:set_scale(1.3)
  end_text2_:set_depth(-450)
  end_text2_:tween("Linear", "Alpha", 0, 255, 500, 0, nil, 2500)
  
  end_text_:on_press(function(self)
      hide()
      demo:reinit()
  end)
  end_text2_:on_press(function(self)
    hide()
    demo:leave_and_cleanup()
  end)
  
end

return {
  create  = create,
  show    = show,
}