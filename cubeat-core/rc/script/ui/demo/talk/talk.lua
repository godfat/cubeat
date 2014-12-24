local ffi     = require 'ffi'
local C       = ffi.C
local view    = require 'rc/script/ui/view'
local ui      = require 'rc/script/ui/ui'
local config  = require 'rc/script/ui/demo/talk/config'
local effect  = require 'rc/script/ui/demo/talk/effect'
local switch  = require 'rc/script/ui/demo/switch/switch'
local select_config = require 'rc/script/ui/demo/select/config'
local event   = require 'rc/script/event/event'
local utf8    = require 'rc/script/utf8_simple'
local random  = require 'rc/script/helper'.random
local storystage  = require 'rc/script/ui/demo/storyend/config'
local parameter   = require 'rc/script/ui/demo/challenge/parameter'
local record      = require 'rc/script/ui/demo/challenge/record'

local root_ = nil

local demo_game_ = nil
 
local data_ = nil
local ask_panel_ = nil
local step_      = 1
--local actor_appear_ = {false, false}
local complete_rundown_ = 0
local current_line_end_flag_  = false 
local old_timer_handle_       = nil
local timer_handle_           = nil
local actor_effect_end_flag_  = true
local word_effect_end_flag_   = true
local special_effect_end_flag_= true

local function game_start(self)
  if demo_game_ then
    local c1p = "char/char"..tostring(select_config.ch_choose[1]).."_new"
    local c2p = "char/char"..tostring(select_config.ch_choose[2]).."_new"
    local sconf = "stage/jungle"..tostring(select_config.ch_choose[2])
    
    if data_ and data_.game_mode~=99 then
      --local sconf = "stage/jungle"..tostring(select_config.ch_choose[2])
      demo_game_:init_mode(data_.game_mode, c1p, c2p, sconf, data_.level)
    elseif data_ and data_.game_mode==99 then
      local story_data = storystage.get_data(select_config.ch_choose[1])
      
      if story_data.ch == select_config.ch_choose[1] then
        --demo_game_:init_tutorial(c1p, c1p, sconf)
        local lv = story_data.lv
        demo_game_:init_story(c1p, c2p, sconf, lv)
      else        
        local lv = story_data.lv
        demo_game_:init_story(c1p, c2p, sconf, lv)
      end
    else
      switch.load_page('mainmenu')
    end
  end
end

local function get_script( lang )
  local script
  if lang == 'EN'   then script = require 'rc/script/ui/demo/talk/script_EN' end
  if lang == 'TW'   then script = require 'rc/script/ui/demo/talk/script_TW' end
  if lang == 'JP'   then script = require 'rc/script/ui/demo/talk/script_JP' end
  
  if script == nil then script = require 'rc/script/ui/demo/talk/script_EN' end --default EN
  return script
end

local function get_end_script( lang )
  local script
  if lang == 'EN'   then script = require 'rc/script/ui/demo/talk/script_end_EN' end
  if lang == 'TW'   then script = require 'rc/script/ui/demo/talk/script_end_TW' end
  if lang == 'JP'   then script = require 'rc/script/ui/demo/talk/script_end_JP' end
  
  if script == nil then script = require 'rc/script/ui/demo/talk/script_end_EN' end --default EN
  return script
end

local function get_font( lang )
  local font
  if lang == 'EN'   then font = 'kimberley' end
  if lang == 'TW'   then font = 'NotoSansHant-Medium' end
  if lang == 'JP'   then font = 'rounded-mplus-1m-medium' end
  
  if font == nil then font = 'kimberley' end
  return font
end


local function reset()
  step_      = 1
  --actor_appear_ = {false, false}
  complete_rundown_ = 0
  current_line_end_flag_  = false 

  if timer_handle_ then
    timer_handle_:remove()
    timer_handle_ = nil
  end
  if old_timer_handle_ then
    old_timer_handle_:remove()
    old_timer_handle_ = nil
  end
  collectgarbage("collect")
end


local function talk_end()
  reset()
  
  if data_ and data_.game_mode==99 then
    if data_.game_end then
      if storystage.get_stage()==6 then -- story mode end
        -- save story character clear data
        local data = { win='true', character=select_config.ch_choose[1] }
        record.save(demo_game_, parameter.story, data)
        -- end game
        --demo_game_:leave_and_cleanup()
        switch.load_page('staffroll', {id='storyend'}, {character=select_config.ch_choose[1]})
      else -- go to next story game talk
        storystage.next_stage()
        local story_data = storystage.get_data(select_config.ch_choose[1])
        select_config.ch_choose[2] = story_data.ch
        local function load_talk_page()
          switch.load_page('talk', nil, {game_mode=99})
          switch.show_effect( {id="slide_out_transfer_to_talk", stage_id=select_config.ch_choose[2]} )
        end
        switch.show_effect( { id="slide_in_transfer", cb=load_talk_page } )
      end
    else -- start next story game
      game_start()
    end
    
  else -- start game
    game_start()
  end
end

local function play_blip_sound(soundtype)
  if soundtype == 1 then
    demo_game_:play_sound("talk/Blip73.wav")
  elseif soundtype == 2 then
    demo_game_:play_sound("talk/Blip74.wav")
  elseif soundtype == 3 then
    demo_game_:play_sound("talk/Blip73.wav")
  elseif soundtype == 4 then
    demo_game_:play_sound("talk/Blip67.wav")
  elseif soundtype == 5 then
    demo_game_:play_sound("talk/Blip86.wav")
  elseif soundtype == 6 then
    demo_game_:play_sound("talk/Blip67.wav")
  else
    print("Lua talk: play_blip_sound error")
  end
end

local function action(menu, rundown)
  print("Lua talk: step "..step_..", completed "..complete_rundown_)
  if step_ ~= complete_rundown_+1 and step_ ~= complete_rundown_ then return end
  if not current_line_end_flag_ and timer_handle_ then
    timer_handle_:remove()
    collectgarbage("collect")
  end
  if step_ >= #rundown then
    talk_end()
    return
  end
  
  local ch = rundown[step_].ch
  local actor   = 'actor'..tostring(ch)
  local content = 'content'..tostring(ch)
  local panel   = 'panel'..tostring(ch)
  local light   = 'light'..tostring(ch)
  
  local function add_complete_rundown()
    complete_rundown_ = complete_rundown_+1
    current_line_end_flag_ = false
    timer_handle_ = nil
    
    print('Lua talk: completed: '..complete_rundown_)
    
    menu[light]:set_visible(true)
    local x = menu[panel]:get_pos_x()+config.light_offset_x
    local y = menu[panel]:get_pos_y()+config.light_offset_y
    menu[light]:tween("Linear", "Pos2D",
                      ffi.new("v2", x, y-10),
                      ffi.new("v2", x, y),
                      config.light_time, -1)
  end
  local function flag_check()
    if (actor_effect_end_flag_==true and word_effect_end_flag_==true and special_effect_end_flag_==true) then
      add_complete_rundown()
    end
  end
  local function actor_effect_cb()
    actor_effect_end_flag_ = true
    flag_check()
  end
  local function word_effect_cb()
    word_effect_end_flag_ = true
    flag_check()
  end
  local function special_effect_cb()
    special_effect_end_flag_ = true
    flag_check()
  end
  local function check_effect_status(effect_a, effect_w, special)
    if (effect_a==nil or (effect_a and effect_a == "slide_in")) and 
       (effect_w==nil or (effect_w and effect_w ~= "shake")) and 
       special==nil 
    then
      return false
    else
      actor_effect_end_flag_  = (effect_a ==nil or effect_a=="slide_in")
      word_effect_end_flag_   = (effect_w ==nil or effect_w=="size_L" or effect_w=="size_M" or effect_w=="size_S")
      special_effect_end_flag_= (special==nil)
      return true
    end
  end

  menu.light1:set_visible(false)
  menu.light2:set_visible(false)
  
  --[[
  if actor_appear_[ch]==false then
    menu[actor]:set_visible(true)
    menu[content]:set_visible(true)
    menu[panel]:set_visible(true)
    actor_appear_[ch]=true
  end
  --]]
  
  -- just show one character content
  local hide_ch = (ch==1 and 2) or 1
  --menu['actor'..tostring(hide_ch)]:set_visible(false)
  menu['content'..tostring(hide_ch)]:set_visible(false)
  menu['panel'..tostring(hide_ch)]:set_visible(false)
  menu[actor]:set_visible(true)
  menu[content]:set_visible(true)
  menu[panel]:set_visible(true)
  
  local sound_type = 0
  
  --actor image
  if rundown[step_].img then
    menu[actor]:set_texture(rundown[step_].img)
    sound_type = tonumber(rundown[step_].img:sub(5,5)) -- get character number to decide what he/she sounds like.
  end
  --actor image pos
  if rundown[step_].ch_pos then
    menu[actor]:set_pos(rundown[step_].ch_pos.x, rundown[step_].ch_pos.y)
  elseif rundown[step_].img then
    local img = rundown[step_].img
    local act_pos = config.act_pos[img][ch]
    menu[actor]:set_pos(act_pos.x, act_pos.y)
  end
  --board
  if rundown[step_].board then
    menu[panel]:set_texture(rundown[step_].board)
  end
  --board_flip
  if current_line_end_flag_ or not timer_handle_ then
    if rundown[step_].board_flip then
      if rundown[step_].board_flip=="H" then menu[panel]:texture_flipH() end
      if rundown[step_].board_flip=="V" then menu[panel]:texture_flipV() end
      if rundown[step_].board_flip=="HV" then menu[panel]:texture_flipH() menu[panel]:texture_flipV() end
    end
  end
  --text pos
  if rundown[step_].pos then
    menu[panel]:set_pos(rundown[step_].pos.x, rundown[step_].pos.y)
    menu[content]:set_pos(menu[panel]:get_pos_x()+config.con_offset_x,
                          menu[panel]:get_pos_y()+config.con_offset_y)
  elseif rundown[step_].img then
    local img = rundown[step_].img
    local conBG_pos = config.conBG_pos[img][ch]
    menu[panel]:set_pos(conBG_pos.x, conBG_pos.y)
    menu[content]:set_pos(conBG_pos.x+config.con_offset_x,
                          conBG_pos.y+config.con_offset_y)
  end
  
  --text
  local show_full_text = function(call_rundown_complete)
    if old_timer_handle_ then
      old_timer_handle_:remove()
      old_timer_handle_ = nil
    end
    if timer_handle_ then
      timer_handle_:remove()
      timer_handle_ = nil
    end
    if rundown[step_].text then
      print('Lua talk: current line ends')
      menu[content]:change_text(rundown[step_].text)
      current_line_end_flag_ = true
      step_ = step_+1
      print('Lua talk: step now is '..step_)
      if call_rundown_complete then
        play_blip_sound(sound_type)
        add_complete_rundown()
      end
    end
  end
  
  local currlen = utf8.len(rundown[step_].text)
  local word_by_word
  word_by_word = function(progress)
    if old_timer_handle_ then
      old_timer_handle_:remove()
      old_timer_handle_ = nil
    end
    menu[content]:change_text( utf8.sub(rundown[step_].text, 1, progress) )
    play_blip_sound(sound_type)
    if progress == currlen - 1 then
      old_timer_handle_ = timer_handle_
      timer_handle_ = event.on_timer("ui", function() show_full_text(true) end, 30)
    elseif progress < currlen - 1 then
      old_timer_handle_ = timer_handle_ 
      timer_handle_ = event.on_timer("ui", function() word_by_word( progress + 1 ) end, 30)
    else
      print('Lua talk: error, impossible logic flow in word_by_word() function')
    end
  end
  
  local show_slow_text = function()
    if rundown[step_].text then
      if not current_line_end_flag_ and not timer_handle_ then
        print('Lua talk: no timer running and current line is still not end')
        menu[content]:change_text("")
        timer_handle_ = event.on_timer("ui", function() word_by_word(1) end, 100)
      elseif timer_handle_ then
        print('Lua talk: timer is already running but current line is not end and player clicks')
        show_full_text(true)
      end
    end
  end
  
  --run effect
  local effect_a  = rundown[step_].effect_a
  local effect_w  = rundown[step_].effect_w
  local special = rundown[step_].special
    
  if current_line_end_flag_ or not timer_handle_ then
    effect.actor_effect  (effect_a, menu[actor], menu[content], menu[panel], ch, rundown[step_].img, actor_effect_cb)
    effect.word_effect   (effect_w, menu[actor], menu[content], menu[panel], ch, rundown[step_].img, word_effect_cb)
    if special then
      effect.special_effect(special.id, menu[actor], menu[content], menu[panel], ch, rundown[step_].img, special_effect_cb, special)
    end
  end
    
  if check_effect_status(effect_a, effect_w, special) then
    print ('Lua talk: this line has effect (step_:'..step_..', text: '..rundown[step_].text..')')
    demo_game_:play_sound("talk/Exp13.wav")
    show_full_text()
  else 
    print ('Lua talk: this line doesn\'t have effect (step_:'..step_..', text: '..rundown[step_].text..')')
    show_slow_text()
  end 
end


local function init(demo, parent, data)
  local menu = {}
  demo_game_ = demo
  data_ = data
  
  root_ = view.new_sprite("blahblah", parent, 0, 0, false)
  
  local ch_choose = {}
  ch_choose[1] = select_config.ch_choose[1]
  ch_choose[2] = select_config.ch_choose[2]
  
  local function play() -- the same as "starter" need refactor
    local script
    if data.game_end then
      script = get_end_script(config.lang)
    else
      script = get_script(config.lang)
    end
    local rundown = script.get_rundown(ch_choose[1], ch_choose[2])
    if rundown ~= nil then
      --menu.skip:set_visible(true)
      action(menu, rundown)
    else
      reset()
      game_start()
    end
  end
  local function leave()
    reset()
    switch.load_page('mainmenu')
  end
  --local bg_path = 'bg' .. tostring(ch_choose[2]) .. '/99complete'
  local bg_path = 'nothing'
  menu.TalkBackGround = ui.new_image{ parent=root_, path=bg_path or config.bg_path, x=config.bg_x, y=config.bg_y,
                                      w=config.bg_w, h=config.bg_h }
  menu.TalkBackGround:set_alpha(0)
  
  for ch=1,2 do
    local ch_path = 'char'..tostring(ch_choose[ch])..'_new/glad'
    local actor   = 'actor'..tostring(ch)
    local content = 'content'..tostring(ch)
    local panel   = 'panel'..tostring(ch)
    local light   = 'light'..tostring(ch)
    local act_pos   = config.act_pos[ch_path][ch]
    local conBG_pos = config.conBG_pos[ch_path][ch]
    menu[actor]   = ui.new_image{ parent=menu.TalkBackGround._cdata, path=ch_path, x=act_pos.x, y=act_pos.y,
                                  w=config.act_w, h=config.act_h, depth=config.act_d, visible=false }
    menu[content] = ui.new_text { parent=menu.TalkBackGround._cdata, title=' ', x=conBG_pos.x+config.con_offset_x, y=conBG_pos.y+config.con_offset_y,
                                  depth=config.con_d, size=28, visible=false, font=get_font(config.lang) }
    menu[panel]   = ui.new_image{ parent=menu.TalkBackGround._cdata, path=config.conBG_path, x=conBG_pos.x, y=conBG_pos.y,
                                  w=config.conBG_w, h=config.conBG_h, depth=config.conBG_d, visible=false }
    menu[light]   = ui.new_image{ parent=menu.TalkBackGround._cdata, path=config.light_path, x=config.light_x[ch], y=config.light_y[ch],
                                  w=config.light_w, h=config.light_h, depth=config.light_d, visible=false }
    if ch==2 then menu[actor]:texture_flipH() end
  end
  --[[
  menu.skip = ui.new_text{ parent=menu.TalkBackGround._cdata, title='skip', x=800, y=500,
                           depth=-120, size=32, visible=false, font=get_font(config.lang) }
  menu.skip:on_press(talk_end)
  --]]
  
  menu.clickBlock = ui.new_image{ parent=menu.TalkBackGround._cdata, path='blahblah', x=config.block_x, y=config.block_y,
                                  w=config.block_w, h=config.block_h, alpha=config.block_a, depth=config.block_d }
  menu.clickBlock:on_press(play)
  menu.clickBlock:on_press_r(
                              function(self)
                                ask_panel_ = ui.new_askbox{ parent = menu.TalkBackGround._cdata,
                                                            title = "LEAVE?",
                                                            depth = -500,
                                                            visible=false }
                                ask_panel_:on_press_ok(leave)
                                ask_panel_:set_visible(true)
                              end
                            )
  --play() -- run first talk
  
  return menu
end

local function starter(menu) -- the same as "starter" need refactor
  local script
  if data_.game_end then
    script = get_end_script(config.lang)
  else
    script = get_script(config.lang)
  end
  local rundown = script.get_rundown(select_config.ch_choose[1], select_config.ch_choose[2])
  if rundown ~= nil then
    --menu.skip:set_visible(true)
    reset()
    action(menu, rundown)
  else
    reset()
    game_start()
  end
end

return {
  init = init,
  starter = starter
}