local ffi  = require 'ffi'
local C    = ffi.C
local view = require 'rc/script/ui/view'
local ui   = require 'rc/script/ui/ui'
local value= require 'rc/script/ui/demo/select/config'

local actor_icon  = {}
local actor_full  = {}
local actor_fade  = {}

--temporary addition
local ch_choose_ = {1, 2}
local demo_game_ = nil
local function choose_character(self)
  if demo_game_ then
    local ch1 = "char/char"..tostring(ch_choose_[1]).."_new"
    local ch2 = "char/char"..tostring(ch_choose_[2]).."_new"
    local sconf = "stage/jungle"..tostring(ch_choose_[1])
    demo_game_:init_vs_cpu(ch1, ch2, sconf)
  end
end

  -- add enter_focus & leave_focus event to actor_icon
local function leave_icon(input, icon_no)
  return function(self) actor_fade[input]:set_texture(value.full_path(icon_no)) end
end

local function enter_icon(input, icon_no)
  local show = function(self)
    
    ch_choose_[input] = icon_no
  
    if icon_no==value.player[input] then
      return
    end
    if actor_full[input]:get_pos_x()==value.full_x[input] then
      actor_fade[input]:tween('Linear', 'Alpha', 255, 0, value.fade_time)
    end
    
    actor_full[input]:set_pos(-value.full_w, value.full_y)
    actor_full[input]:set_texture(value.full_path(icon_no))
    actor_full[input]:tween('Linear', 'Pos2D', value.move_start[input], value.move_end[input], value.move_time)
    value.player[input] = icon_no
  end  
  return show
end

local function init(demo_game, parent)
  
  demo_game_ = demo_game
  
  -- create select_actor_page
  select_actor_page = ui.new_image{ parent=parent, path=value.bg_path, visible=false,
                                    w=value.bg_w, h=value.bg_h }

  -- create actor_icon
  for i=1,6 do
    actor_icon[i] = ui.new_image{ parent=select_actor_page._cdata, path=value.icon_path(i),
                                  x=value.icon_x[i], y=value.icon_y, w=value.icon_w, h=value.icon_h,
                                  depth =value.icon_depth }
  end
  
  -- create actor_full & actor_fade
  for i=1,2 do
    actor_full[i] = ui.new_image{ parent=select_actor_page._cdata, path=value.full_path(1),
                                  x=value.full_x[i], y=value.full_y, w=value.full_w, h=value.full_h,
                                  depth=value.full_depth }
    actor_fade[i] = ui.new_image{ parent=select_actor_page._cdata, path=value.full_path(1), alpha=0,
                                  x=value.full_x[i], y=value.full_y, w=value.full_w, h=value.full_h,
                                  depth=value.full_depth }
  end
  for i=6,1,-1 do--load texture
    actor_full[1]:set_texture(value.full_path(i))
    actor_full[2]:set_texture(value.full_path(i))
    actor_fade[1]:set_texture(value.full_path(i))
    actor_fade[2]:set_texture(value.full_path(i))
  end
  
  for i,v in ipairs(actor_icon) do
    v:on_press( choose_character )
    v:on_leave_focus( view.Input1, leave_icon(1, i) )
    v:on_enter_focus( view.Input1, enter_icon(1, i) )
    v:on_leave_focus( view.Input2, leave_icon(2, i) )
    v:on_enter_focus( view.Input2, enter_icon(2, i) )
  end
  
  return select_actor_page
end

return {
  init  = init
}