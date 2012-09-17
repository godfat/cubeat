local ffi   = require 'ffi'
local C     = ffi.C
local view  = require 'rc/script/ui/view'
local ui    = require 'rc/script/ui/ui'
local config= require 'rc/script/ui/demo/select/config'
local switch= require 'rc/script/ui/demo/switch/switch'
local random= require 'rc/script/helper'.random


--temporary addition
local demo_game_ = nil
local data_ = nil
local function choose_character(self)
  if demo_game_ then
    local ch2 = random(6)+1
    local c1p = "char/char"..tostring(config.ch_choose[1]).."_new"
    local c2p
    if data_.game_mode ~= 1 then
      c2p = "char/char"..tostring(config.ch_choose[2]).."_new"
    else
      c2p = "char/char"..tostring(ch2).."_new"
    end
    local sconf = "stage/jungle"..tostring(config.ch_choose[1])

    demo_game_:init_mode(data_.game_mode, c1p, c2p, sconf, data_.level)
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

    if icon_no == config.ch_choose[input] then
      return
    end
    if menu[fullkey]:get_pos_x() > 0 then
      menu[fadekey]:tween('Linear', 'Alpha', 255, 0, config.fade_time)
    end

    menu[fullkey]:set_pos(-config.full_w, config.full_y)
    menu[fullkey]:set_texture(config.full_path(icon_no))
    if data_.game_mode ~= 1 then
      menu[fullkey]:tween('Linear', 'Pos2D', config.move_start[input], config.move_end[input], config.move_time)
    else
      local move_end = ffi.new("value2", config.screen_w/2 - 225, 0)
      menu[fullkey]:tween('Linear', 'Pos2D', config.move_start[input], move_end, config.move_time)
    end
    config.ch_choose[input] = icon_no
  end
  return show
end

local function init(demo, parent, data)
  local menu = {}
  demo_game_ = demo
  data_ = data
  
  menu.btn_back = ui.new_text{ parent = parent, x=-470, y=-310, size=32, title='<= go back'}
  menu.btn_back:set_scale(1.5)
  menu.btn_back:set_depth(-300)
  menu.btn_back:on_press(function(self)
    switch.load_page(data_.last_menu or 'mainmenu', 'in', data_)
  end)
  
  menu.up_blocker = ui.new_image{ parent = parent, path='nothing', x=-480, y=-300, w=1280, h=52 }
  menu.up_blocker:set_color(0, 0, 0)
  menu.up_blocker:set_alpha(96)
  menu.up_blocker:set_depth(-250)
  
  menu.bottom_blocker = ui.new_image{ parent = parent, path='nothing', x=-480, y=200, w=1280, h=300 }
  menu.bottom_blocker:set_color(0, 0, 0)
  menu.bottom_blocker:set_alpha(96)
  menu.bottom_blocker:set_depth(-150)
  
  menu.menutext = ui.new_text{ parent = parent, x=-20, y=-310, size=32, title='character select'}
  menu.menutext:set_scale(1.5)
  menu.menutext:set_depth(-300)

  -- create select_actor_page
  menu.select_actor_page = ui.new_image{ parent=parent, path='nothing', visible=true, x= -480, y= -300,
                                         w=0, h=0 }

  -- create actor_icon
  for i=1,6 do
    local k = 'actor_icon_'..tostring(i)
    menu[k] = ui.new_image{ parent=menu.select_actor_page._cdata, path=config.icon_path(i),
                            x=config.icon_x[i], y=config.icon_y, w=config.icon_w, h=config.icon_h,
                            depth =config.icon_depth }
  end

  -- create actor_full & actor_fade
  local num_actor = 1
  if data_ and data_.game_mode ~= 1 then num_actor = 2 end
  for ch=1, num_actor do
    local fullkey = 'actor_full_'..tostring(ch)
    local fadekey = 'actor_fade_'..tostring(ch)
    local actor_x
    if data_ and data_.game_mode == 1 then 
      actor_x = (config.screen_w/2) - (config.full_w/2)
    else 
      actor_x = config.full_x[ch] 
    end
    menu[fullkey] = ui.new_image{ parent=menu.select_actor_page._cdata, path=config.full_path(1),
                                  x= actor_x, 
                                  y=config.full_y, w=config.full_w, h=config.full_h,
                                  depth=config.full_depth }
    menu[fadekey] = ui.new_image{ parent=menu.select_actor_page._cdata, path=config.full_path(1), alpha=0,
                                  x= actor_x, 
                                  y=config.full_y, w=config.full_w, h=config.full_h,
                                  depth=config.full_depth }
    if ch==2 then
      menu[fullkey]:texture_flipH()
      menu[fadekey]:texture_flipH()
    end
  end
  for i=6,1,-1 do--preload texture
    menu.actor_full_1:set_texture(config.full_path(i))
    menu.actor_fade_1:set_texture(config.full_path(i))
    
    if data_ and data_.game_mode ~= 1 then 
      menu.actor_full_2:set_texture(config.full_path(i)) 
      menu.actor_fade_2:set_texture(config.full_path(i)) 
    end
  end

  --for i,v in ipairs(actor_icon) do
  for i=1,6 do
    local k = 'actor_icon_'..tostring(i)
    --menu[k]:on_press( choose_character )
    menu[k]._cdata:on_press( view.Input1_left, choose_character ) -- only allow player 1 to "check" for now.
    menu[k]:on_leave_focus( view.Input1, leave_icon(1, i, menu) )
    menu[k]:on_enter_focus( view.Input1, enter_icon(1, i, menu) )
    
    if data_ and data_.game_mode ~= 1 then 
      menu[k]:on_leave_focus( view.Input2, leave_icon(2, i, menu) )
      menu[k]:on_enter_focus( view.Input2, enter_icon(2, i, menu) )
    end
  end

  --load ch_choose texture
  menu.actor_full_1:set_texture(config.full_path(config.ch_choose[1]))
  menu.actor_fade_1:set_texture(config.full_path(config.ch_choose[1]))
  
  if data_ and data_.game_mode ~= 1 then
    menu.actor_full_2:set_texture(config.full_path(config.ch_choose[2]))
    menu.actor_fade_2:set_texture(config.full_path(config.ch_choose[2]))
  end
  
  return menu
end

return {
  init  = init
}
