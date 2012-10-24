local ffi     = require 'ffi'
local C       = ffi.C
local view    = require 'rc/script/ui/view'
local ui      = require 'rc/script/ui/ui'
local config  = require 'rc/script/ui/demo/talk/config'
local effect  = require 'rc/script/ui/demo/talk/effect'
local switch  = require 'rc/script/ui/demo/switch/switch'
local select_config = require 'rc/script/ui/demo/select/config'


local ask_panel_ = nil
local step_      = 1
local actor_appear_ = {false, false}
local complete_rundown_ = 0
local actor_effect_end_flag_ = true
local word_effect_end_flag_  = true


local function get_script( lang )
  local script
  if lang == 'EN'   then script = require 'rc/script/ui/demo/talk/script_EN' end
  if lang == 'TW'   then script = require 'rc/script/ui/demo/talk/script_TW' end
  
  if script == nil then script = require 'rc/script/ui/demo/talk/script_EN' end --default EN
  return script
end


local function add_complete_rundown()
  complete_rundown_ = complete_rundown_+1
end
local function actor_effect_cb()
  actor_effect_end_flag_ = true
  if word_effect_end_flag_==true then add_complete_rundown() end
end
local function word_effect_cb()
  word_effect_end_flag_ = true
  if actor_effect_end_flag_==true then add_complete_rundown() end
end
local function check_effect_status(effect_a, effect_w)
  if effect_a==nil and effect_w==nil then
    add_complete_rundown()
  else
    actor_effect_end_flag_ = (effect_a==nil)
    word_effect_end_flag_  = (effect_w==nil)
  end
end


local function reset()
  step_      = 1
  actor_appear_ = {false, false}
  complete_rundown_ = 0
end


local function action(menu, rundown)
  if step_ ~= complete_rundown_+1 then return end
  
  local ch = rundown[step_].ch
  local actor   = 'actor'..tostring(ch)
  local content = 'content'..tostring(ch)
  local panel   = 'panel'..tostring(ch)
  
  if actor_appear_[ch]==false then
    menu[actor]:set_visible(true)
    menu[content]:set_visible(true)
    menu[panel]:set_visible(true)
    actor_appear_[ch]=true
  end
  
  --actor image
  if rundown[step_].img then
    menu[actor]:set_texture(rundown[step_].img)
  end
  --text
  if rundown[step_].text then
    menu[content]:change_text(rundown[step_].text)
  end
  --text pos
  if rundown[step_].pos then
    menu[panel]:set_pos(rundown[step_].pos.x, rundown[step_].pos.y)
    menu[content]:set_pos(menu[panel]:get_pos_x()+config.con_offset_x,
                          menu[panel]:get_pos_y()+config.con_offset_y)
  end
  --run effect
  local effect_a = rundown[step_].effect_a
  local effect_w = rundown[step_].effect_w
  check_effect_status(effect_a, effect_w)
  effect.actor_effect(effect_a, menu[actor], menu[content], menu[panel], ch, actor_effect_cb)
  effect.word_effect (effect_w, menu[actor], menu[content], menu[panel], ch, word_effect_cb)
  
  step_=step_+1
  if step_>table.getn(rundown) then
    reset()
    switch.load_page('testmenu', 'in')
  end
end


local function init(demo, parent)
  local menu = {}
  
  local ch_choose = {}
  ch_choose[1] = select_config.ch_choose[1]
  ch_choose[2] = select_config.ch_choose[2]
  
  local function play()
    local script  = get_script(config.lang)
    local rundown = script.get_rundown(ch_choose[1], ch_choose[2])
    if rundown ~= nil then
      action(menu, rundown)
    end
  end
  local function leave()
    reset()
    switch.load_page('testmenu', 'in')
  end
  
  menu.TalkBackGround = ui.new_image{ parent=parent, path=config.bg_path, x=config.bg_x, y=config.bg_y,
                                      w=config.bg_w, h=config.bg_h }
  
  for ch=1,2 do
    local ch_path = 'char'..tostring(ch_choose[ch])..'_new/glad'
    local actor   = 'actor'..tostring(ch)
    local content = 'content'..tostring(ch)
    local panel   = 'panel'..tostring(ch)
    menu[actor]   = ui.new_image{ parent=menu.TalkBackGround._cdata, path=ch_path, x=config.act_x[ch], y=config.act_y[ch],
                                  w=config.act_w, h=config.act_h, depth=config.act_d, visible=false }
    menu[content] = ui.new_text { parent=menu.TalkBackGround._cdata, title=' ', x=config.con_x[ch], y=config.con_y[ch],
                                  depth=config.con_d, size=32, visible=false }
    menu[panel]   = ui.new_image{ parent=menu.TalkBackGround._cdata, path='area_rect', x=config.conBG_x[ch], y=config.conBG_y[ch],
                                  w=config.conBG_w, h=config.conBG_h, depth=config.conBG_d, visible=false }
    if ch==2 then menu[actor]:texture_flipH() end
  end
  
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
  
  return menu
end


return {
  init = init
}