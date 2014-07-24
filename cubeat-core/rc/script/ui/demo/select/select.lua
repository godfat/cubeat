local ffi   = require 'ffi'
local C     = ffi.C
local view  = require 'rc/script/ui/view'
local ui    = require 'rc/script/ui/ui'
local config= require 'rc/script/ui/demo/select/config'
local switch= require 'rc/script/ui/demo/switch/switch'
local random= require 'rc/script/helper'.random
local storystage = require 'rc/script/ui/demo/storyend/config'
local parameter   = require 'rc/script/ui/demo/challenge/parameter'
local record      = require 'rc/script/ui/demo/challenge/record'

local root_ = nil

local Input1      = C.Input_get_input1()
local Input2      = C.Input_get_input2()
local Input1_left = C.Input_get_trig1(C.Input_get_input1())
local Input2_left = C.Input_get_trig1(C.Input_get_input2())
local Input1_right= C.Input_get_trig2(C.Input_get_input1())
local Input2_right= C.Input_get_trig2(C.Input_get_input2())

--temporary addition
local demo_game_ = nil
local data_ = nil
local ch_lock_ = 2
local selectlock_ = {false, false}

local function choose_character(self)
  if demo_game_ then
    local ch2 = random(6)+1
    local c1p = "char/char"..tostring(config.ch_choose[1]).."_new"
    local c2p
    if data_ and (data_.game_mode == 0 or data_.game_mode == 1) then
      c2p = "char/char"..tostring(config.ch_choose[2]).."_new"
    elseif data_ and data_.game_mode == 99 then
      local story_data = storystage.get_data(config.ch_choose[1])
      c2p = "char/char"..tostring(story_data.ch).."_new"
      config.ch_choose[2] = story_data.ch
    else
    --[[
      c2p = "char/char"..tostring(ch2).."_new"
      config.ch_choose[2] = ch2
    --]]
    end
    local sconf = "stage/jungle"..tostring(config.ch_choose[2])

    if data_ and data_.game_mode == 99 then
      local function load_talk_page()
        switch.load_page('talk', nil, data_)
        switch.show_effect( {id="slide_out_transfer_to_talk"} )
      end
      switch.show_effect( { id="slide_in_transfer", cb=load_talk_page } )
    else
      demo_game_:init_mode(data_.game_mode, c1p, c2p, sconf, data_.level)
    end

  end
end

local function ready_to_start(menu)
  return  function(self)
            local ready = false
            if data_ and data_.game_mode ~= 99 then
              ready = ( selectlock_[1]==true and selectlock_[2]==true )
            else
              ready = ( selectlock_[1]==true )
            end

            if ready==true then
              choose_character()
              -- menu.start:set_visible(true)
              -- don't show "start", just simply start after characters are chosen
            end
          end
end

local function select_effect(input, icon_no, menu)
  return  function(self)
            local ch = input
            if data_ and data_.game_mode == 1 then
              if ch==1 and selectlock_[ch]==true then ch = 2 end
            end
            if selectlock_[ch]==true then return end
            if icon_no ~= 0 and icon_no ~= config.ch_choose[ch] then return end
            selectlock_[ch] = true
            local fullkey = 'actor_full_'..tostring(ch)
            local key     = 'ready_'..tostring(ch)
            
            if icon_no == 0 then
              menu[fullkey]:set_color(255, 255, 255)
            end
            if menu[key] then menu[key]:set_visible(true) end
            
            local scale_s = ffi.new("value3", 1.1, 1.1, 0)
            local scale_e = ffi.new("value3", 1,   1,   0)
            menu['actor_full_'..tostring(ch)]:tween('Linear', 'Scale', scale_s, scale_e, 100, 0, ready_to_start(menu), 0)
          end
end

local function cancel_select(input, icon_no, menu)
  return  function(self)
            local ch = input
            if selectlock_[ch]==false then return end
            if config.ch_choose[ch] ~= icon_no then return end
            selectlock_[ch] = false
            local key = 'ready_'..tostring(ch)
            if menu[key] then menu[key]:set_visible(false) end
            menu.start:set_visible(false)
          end
end

  -- add enter_focus & leave_focus event to actor_icon
local function leave_icon(input, icon_no, menu)
  return  function(self)
            local ch = input
            if data_ and data_.game_mode == 1 then
              if ch==1 and selectlock_[ch]==true then ch = 2 end
            end
            if selectlock_[ch]==true then return end
            local fullkey = 'actor_full_'..tostring(ch)
            local fadekey = 'actor_fade_'..tostring(ch)
            
            if icon_no ~= 0 then
              menu[fadekey]:set_texture(config.full_path(icon_no))
            elseif icon_no == 0 then
              menu[fullkey]:set_color(255, 255, 255)
              menu[fadekey]:set_texture(config.full_path(config.ch_choose[ch]))
            end
          end
end

local function enter_icon(input, icon_no, menu)
  local show = function(self)
    local ch = input
    if data_ and data_.game_mode == 1 then
      if ch==1 and selectlock_[ch]==true then ch = 2 end
    end
    
    if selectlock_[ch]==true then return end
    
    local fullkey = 'actor_full_'..tostring(ch)
    local fadekey = 'actor_fade_'..tostring(ch)

    if icon_no ~= 0 then
      if icon_no == config.ch_choose[ch] then
        return
      end
      
      menu[fadekey]:tween('Linear', 'Alpha', 255, 0, config.fade_time)
      menu[fullkey]:set_pos(-config.full_w, config.full_y)
      menu[fullkey]:set_texture(config.full_path(icon_no))
      if data_ and data_.game_mode ~= 99 then
        menu[fullkey]:tween('Linear', 'Pos2D', config.move_start[ch], config.move_end[ch], config.move_time)
      else
        local move_end = ffi.new("value2", config.screen_w/2 - config.full_w/2, -100)
        menu[fullkey]:tween('Linear', 'Pos2D', config.move_start[ch], move_end, config.move_time)
      end
      config.ch_choose[ch] = icon_no
      
    elseif icon_no == 0 then
      if data_ and data_.game_mode ~= 99 then
        menu[fullkey]:set_pos(config.full_x[ch], config.full_y)
      else
        menu[fullkey]:set_pos(config.screen_w/2-config.full_w/2, config.full_y)
      end
    end
  end
  return show
end

local function random_icon_on_up(input, icon_no, menu)
  local show = function(self)
    local ch = input
    if data_ and data_.game_mode == 1 then
      if ch==1 and selectlock_[ch]==true then ch = 2 end
    end
    
    if selectlock_[ch]==true then return end
    
    local fullkey = 'actor_full_'..tostring(ch)
    local fadekey = 'actor_fade_'..tostring(ch)
    
    ----
    
    local i = random(6) + 1
    if i == config.ch_choose[ch] then i = i+1 end
    if i > 6 then i = 1 end
    menu[fullkey]:set_texture(config.full_path(i))
    menu[fullkey]:set_color(0, 0, 0)
    config.ch_choose[ch] = i
  end
  return show
end

local function init(demo, parent, data)
  local menu = {}
  demo_game_ = demo
  data_ = data
  ch_lock_ = 2
  selectlock_ = {false, false}
  
  root_ = view.new_sprite("blahblah", parent, 0, 0, false)
  
  menu.btn_back = ui.new_text{ parent = root_, x=10, y=-10, size=32, title='<= go back'}
  menu.btn_back:set_scale(1.5)
  menu.btn_back:set_depth(-300)
  menu.btn_back:on_press(function(self)
    if data_ then
      switch.load_page(data_.last_menu or 'mainmenu', {id="slide_in_title"}, data_)
    else
      switch.load_page('mainmenu', {id="slide_in_title"})
    end
  end)
  
  menu.up_blocker = ui.new_image{ parent = root_, path='nothing', x=0, y=0, w=1280, h=52 }
  menu.up_blocker:set_color(0, 0, 0)
  menu.up_blocker:set_alpha(96)
  menu.up_blocker:set_depth(-150)
  
  menu.bottom_blocker = ui.new_image{ parent = root_, path='nothing', x=0, y=500, w=1280, h=300 }
  menu.bottom_blocker:set_color(0, 0, 0)
  menu.bottom_blocker:set_alpha(96)
  menu.bottom_blocker:set_depth(-150)
  
  menu.menutext = ui.new_text{ parent = root_, x=460, y=-10, size=32, title='character select'}
  menu.menutext:set_scale(1.5)
  menu.menutext:set_depth(-300)

  -- create select_actor_page
  --menu.select_actor_page = ui.new_image{ parent=root_, path='nothing', visible=true, x=0, y=0, w=0, h=0 }
                                         
  -- create actor_icon
  for i=1,6 do
    local k = 'actor_icon_'..tostring(i)
    menu[k] = ui.new_image{ parent=root_, path=config.icon_path(i),
                            x=config.icon_x[i], y=config.icon_y, w=config.icon_w, h=config.icon_h,
                            depth =config.icon_depth }
  end
  
  -- create random icon
  if data_ and data_.game_mode ~= 99 then
    menu['actor_icon_0'] = ui.new_image{ parent=root_, path='itembox/moving/2',
                                         x=config.screen_w/2-config.icon_w/2, y=config.icon_y,
                                         w=config.icon_w, h=config.icon_h,
                                         depth =config.icon_depth }
  end

  -- create actor_full & actor_fade & ready_text
  local num_actor = 1
  if data_ and data_.game_mode ~= 99 then num_actor = 2 end
  for ch=1, num_actor do
    local fullkey = 'actor_full_'..tostring(ch)
    local fadekey = 'actor_fade_'..tostring(ch)
    local readykey= 'ready_'..tostring(ch)
    local actor_x
    if data_ and data_.game_mode == 99 then 
      actor_x = (config.screen_w/2) - (config.full_w/2)
    else 
      actor_x = config.full_x[ch] 
    end
    menu[fullkey] = ui.new_image{ parent=root_, path=config.full_path(1),
                                  x= actor_x, 
                                  y=config.full_y, w=config.full_w, h=config.full_h,
                                  depth=config.full_depth }
    menu[fadekey] = ui.new_image{ parent=root_, path=config.full_path(1), alpha=0,
                                  x= actor_x, 
                                  y=config.full_y, w=config.full_w, h=config.full_h,
                                  depth=config.full_depth }
    menu[readykey]= ui.new_text{ parent=root_, x=actor_x+(config.full_w/2), y=config.ready_y,
                                 depth=config.ready_depth, size=config.ready_size, title='READY', center=true, visible=false }
    menu[readykey]:set_color(255,255,0)
    
    if ch==2 then
      menu[fullkey]:texture_flipH()
      menu[fadekey]:texture_flipH()
    end
  end
  for i=6,1,-1 do--preload texture
    menu.actor_full_1:set_texture(config.full_path(i))
    menu.actor_fade_1:set_texture(config.full_path(i))
    
    if data_ and data_.game_mode ~= 99 then 
      menu.actor_full_2:set_texture(config.full_path(i)) 
      menu.actor_fade_2:set_texture(config.full_path(i)) 
    end
  end

  for i=1,3 do
    local lock_b = record.load(parameter.story, {character=i*2})
    local lock_a = record.load(parameter.story, {character=i*2-1})
    if lock_b and lock_a then ch_lock_ = (i+1)*2 end
    if ch_lock_>6 then ch_lock_=6 end
    if data_ and data_.game_mode ~= 99 then ch_lock_=6 end -- Only TGS Ver.
  end
  --for i,v in ipairs(actor_icon) do
  for i=1,ch_lock_ do
    local k = 'actor_icon_'..tostring(i)
    --menu[k]:on_press( choose_character, 1 ) -- only allow player 1 to "check" for now.
    menu[k]:on_press( select_effect(1, i, menu), Input1_left )
    menu[k]:on_press_r( cancel_select(1, i, menu), Input1_right )
    if data_ and data_.game_mode == 0 then
      menu[k]:on_press( select_effect(2, i, menu), Input2_left )
      menu[k]:on_press_r( cancel_select(2, i, menu), Input2_right )
    end
    
    menu[k]:on_leave_focus( leave_icon(1, i, menu), Input1 )
    menu[k]:on_enter_focus( enter_icon(1, i, menu), Input1 )
    if data_ and data_.game_mode == 0 then 
      menu[k]:on_leave_focus( leave_icon(2, i, menu), Input2 )
      menu[k]:on_enter_focus( enter_icon(2, i, menu), Input2 )
    end
  end
  
  if data_ and data_.game_mode ~= 99 then
    menu['actor_icon_0']:on_press( select_effect(1, 0, menu), Input1_left )
    menu['actor_icon_0']:on_leave_focus( leave_icon(1, 0, menu), Input1 )
    menu['actor_icon_0']:on_enter_focus( enter_icon(1, 0, menu), Input1 )
    menu['actor_icon_0']:on_up( random_icon_on_up(1, 0, menu), Input1_left )
    if data_ and data_.game_mode == 0 then
      menu['actor_icon_0']:on_press( select_effect(2, 0, menu), Input2_left )
      menu['actor_icon_0']:on_leave_focus( leave_icon(2, 0, menu), Input2 )
      menu['actor_icon_0']:on_enter_focus( enter_icon(2, 0, menu), Input2 )
      menu['actor_icon_0']:on_up( random_icon_on_up(2, 0, menu), Input2_left )
    end
  end
  
  if ch_lock_~=6 then
    for i=ch_lock_+1,6 do
      local k = 'actor_icon_'..tostring(i)
      menu[k]:set_color(50,50,50)
    end
  end
  
  --create game start button
  menu.start = ui.new_text{ parent=root_, x=config.start_x, y=config.start_y,
                            depth=config.start_depth, size=config.start_size, title='START', center=true, visible=false}
  menu.start:on_press(choose_character)

  --load ch_choose texture
  menu.actor_full_1:set_texture(config.full_path(config.ch_choose[1]))
  menu.actor_fade_1:set_texture(config.full_path(config.ch_choose[1]))
  
  if data_ and data_.game_mode ~= 99 then
    menu.actor_full_2:set_texture(config.full_path(config.ch_choose[2]))
    menu.actor_fade_2:set_texture(config.full_path(config.ch_choose[2]))
  end
  
  return menu
end

return {
  init  = init
}
