local ffi   = require 'ffi'
local C     = ffi.C
local view  = require 'rc/script/ui/view'
local ui    = require 'rc/script/ui/ui'
local config= require 'rc/script/ui/demo/talk/config'
local switch= require 'rc/script/ui/demo/switch/switch'

local function init(demo, parent)
  local menu = {}
  
  menu.TalkBackGround = ui.new_image{ parent=parent, path=config.bg_path, x=config.bg_x, y=config.bg_y,
                                      w=config.bg_w, h=config.bg_h }
  menu.TalkActorL     = ui.new_image{ parent=parent, path='char1_new/glad', x=config.act_L_x, y=config.act_L_y,
                                      w=config.act_w, h=config.act_h, depth=config.act_d }
  menu.TalkActorR     = ui.new_image{ parent=parent, path='char2_new/glad', x=config.act_R_x, y=config.act_R_y,
                                      w=config.act_w, h=config.act_h, depth=config.act_d }

  menu.ContentL = ui.new_text{ parent=parent, title='ContentR', x=config.con_L_x, y=config.con_L_y, size=32 }
  menu.ContentR = ui.new_text{ parent=parent, title='ContentL', x=config.con_R_x, y=config.con_R_y, size=32 }
  
  menu.ContentBG_L  = ui.new_image{ parent=parent, path='area_rect', x=config.conBG_L_x, y=config.conBG_L_y,
                                    w=config.conBG_w, h=config.conBG_h }
  menu.ContentBG_R  = ui.new_image{ parent=parent, path='area_rect', x=config.conBG_R_x, y=config.conBG_R_y,
                                    w=config.conBG_w, h=config.conBG_h }
  
  menu.back     = ui.new_text{ parent=parent, title='Back', x=0, y=300, size=32, depth=-100 }
  menu.back:on_press(function(self)
    switch.load_page(demo, 'testmenu', 'in')
  end)
  
  return menu
end

return {
  init = init
}