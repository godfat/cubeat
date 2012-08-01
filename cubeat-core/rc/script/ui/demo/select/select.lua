local ffi   = require 'ffi'
local C     = ffi.C
local view  = require 'rc/script/ui/view'
local ui    = require 'rc/script/ui/ui'
local config= require 'rc/script/ui/demo/select/config'
local switch= require 'rc/script/ui/demo/switch/switch'


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
local function leave_icon(input, icon_no, menu)
  return  function(self)
            local fadekey = 'actor_fade_'..tostring(input)
            menu[fadekey]:set_texture(config.full_path(icon_no))
          end
end

local function enter_icon(input, icon_no, menu)
  local show = function(self)
    local fullkey = 'actor_full_'..tostring(input)
    local fadekey = 'actor_fade_'..tostring(input)
    
    ch_choose_[input] = icon_no
  
    if icon_no==config.player[input] then
      return
    end
    if menu[fullkey]:get_pos_x()==config.full_x[input] then
      menu[fadekey]:tween('Linear', 'Alpha', 255, 0, config.fade_time)
    end
    
    menu[fullkey]:set_pos(-config.full_w, config.full_y)
    menu[fullkey]:set_texture(config.full_path(icon_no))
    menu[fullkey]:tween('Linear', 'Pos2D', config.move_start[input], config.move_end[input], config.move_time)
    config.player[input] = icon_no
  end  
  return show
end

local function init(demo, parent)
  local menu = {}
  demo_game_ = demo
  
  -- create select_actor_page
  menu.select_actor_page = ui.new_image{ parent=parent, path=config.bg_path, visible=true, x= -480, y= -300,
                                         w=config.bg_w, h=config.bg_h }
  menu.select_actor_page:on_press_r(function(self)
                                      switch.load_page('testmenu', 'in')
                                    end)

  -- create actor_icon
  for i=1,6 do
    local k = 'actor_icon_'..tostring(i)
    menu[k] = ui.new_image{ parent=menu.select_actor_page._cdata, path=config.icon_path(i),
                            x=config.icon_x[i], y=config.icon_y, w=config.icon_w, h=config.icon_h,
                            depth =config.icon_depth }
  end
  
  -- create actor_full & actor_fade
  for i=1,2 do
    local fullkey = 'actor_full_'..tostring(i)
    local fadekey = 'actor_fade_'..tostring(i)
    menu[fullkey] = ui.new_image{ parent=menu.select_actor_page._cdata, path=config.full_path(1),
                                  x=config.full_x[i], y=config.full_y, w=config.full_w, h=config.full_h,
                                  depth=config.full_depth }
    menu[fadekey] = ui.new_image{ parent=menu.select_actor_page._cdata, path=config.full_path(1), alpha=0,
                                  x=config.full_x[i], y=config.full_y, w=config.full_w, h=config.full_h,
                                  depth=config.full_depth }
  end
  for i=6,1,-1 do--load texture
    menu.actor_full_1:set_texture(config.full_path(i))
    menu.actor_full_2:set_texture(config.full_path(i))
    menu.actor_fade_1:set_texture(config.full_path(i))
    menu.actor_fade_2:set_texture(config.full_path(i))
  end
  
  --for i,v in ipairs(actor_icon) do
  for i=1,6 do
    local k = 'actor_icon_'..tostring(i)
    menu[k]:on_press( choose_character )
    menu[k]:on_leave_focus( view.Input1, leave_icon(1, i, menu) )
    menu[k]:on_enter_focus( view.Input1, enter_icon(1, i, menu) )
    menu[k]:on_leave_focus( view.Input2, leave_icon(2, i, menu) )
    menu[k]:on_enter_focus( view.Input2, enter_icon(2, i, menu) )
  end
  
  return menu
end

return {
  init  = init
}