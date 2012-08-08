local ffi     = require 'ffi'
local C       = ffi.C
local view    = require 'rc/script/ui/view'
local ui      = require 'rc/script/ui/ui'
local config  = require 'rc/script/ui/demo/talk/config'
local script  = require 'rc/script/ui/demo/talk/script'
local switch  = require 'rc/script/ui/demo/switch/switch'
local select_config = require 'rc/script/ui/demo/select/config'


local index_      = 1
local first_talk_ = {false, false}


local function action(menu, rundown)
  local ch = rundown[index_].index
  local actor   = 'actor'..tostring(ch)
  local content = 'content'..tostring(ch)
  local panel   = 'panel'..tostring(ch)
  
  if first_talk_[ch]==false then
    menu[actor]:set_visible(true)
    menu[content]:set_visible(true)
    menu[panel]:set_visible(true)
    first_talk_[ch]=true
  end
  menu[actor]:set_texture(rundown[index_].img)
  menu[content]:change_text(rundown[index_].text)
  
  index_=index_+1
  if index_>table.getn(rundown) then index_=1 end
end

local function reset()
  index_      = 1
  first_talk_ = {false, false}
end

local function init(demo, parent)
  local menu = {}
  
  local ch_choose = {}
  ch_choose[1] = select_config.ch_choose[1]
  ch_choose[2] = select_config.ch_choose[2]
  
  local function play()
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
  menu.TalkBackGround:on_press(play)
  menu.TalkBackGround:on_press_r(leave)
  
  for ch=1,2 do
    local ch_path = 'char'..tostring(ch_choose[ch])..'_new/glad'
    local actor   = 'actor'..tostring(ch)
    local content = 'content'..tostring(ch)
    local panel   = 'panel'..tostring(ch)
    menu[actor]   = ui.new_image{ parent=parent, path=ch_path, x=config.act_x[ch], y=config.act_y[ch],
                                  w=config.act_w, h=config.act_h, depth=config.act_d, visible=false }
    menu[content] = ui.new_text { parent=parent, title=' ', x=config.con_x[ch], y=config.con_y[ch],
                                  size=32, visible=false }
    menu[panel]   = ui.new_image{ parent=parent, path='area_rect', x=config.conBG_x[ch], y=config.conBG_y[ch],
                                  w=config.conBG_w, h=config.conBG_h, visible=false }
  end
  
  return menu
end

return {
  init = init
}