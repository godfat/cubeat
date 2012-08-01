local ffi     = require 'ffi'
local C       = ffi.C
local view    = require 'rc/script/ui/view'
local ui      = require 'rc/script/ui/ui'
local config  = require 'rc/script/ui/demo/talk/config'
local switch  = require 'rc/script/ui/demo/switch/switch'
local select_config = require 'rc/script/ui/demo/select/config'

local function init(demo, parent)
  local menu = {}
  
  local ch_choose = {}
  ch_choose[1] = select_config.ch_choose[1]
  ch_choose[2] = select_config.ch_choose[2]
  
  menu.TalkBackGround = ui.new_image{ parent=parent, path=config.bg_path, x=config.bg_x, y=config.bg_y,
                                      w=config.bg_w, h=config.bg_h }
  
  for i=1,2 do
    local ch_path = 'char'..tostring(ch_choose[i])..'_new/glad'
    local actor   = 'actor'..tostring(i)
    local content = 'content'..tostring(i)
    local panel   = 'panel'..tostring(i)
    menu[actor]   = ui.new_image{ parent=parent, path=ch_path, x=config.act_x[i], y=config.act_y[i],
                                  w=config.act_w, h=config.act_h, depth=config.act_d }
    menu[content] = ui.new_text{ parent=parent, title=' ', x=config.con_x[i], y=config.con_y[i], size=32 }
    menu[panel]   = ui.new_image{ parent=parent, path='area_rect', x=config.conBG_x[i], y=config.conBG_y[i],
                                  w=config.conBG_w, h=config.conBG_h }
  end
  
  menu.back     = ui.new_text{ parent=parent, title='Back', x=0, y=300, size=32, depth=-100 }
  menu.back:on_press(function(self)
    switch.load_page('testmenu', 'in')
  end)
  
  return menu
end

return {
  init = init
}