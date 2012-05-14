local ffi       = require 'ffi'
local C         = ffi.C
local ui        = require 'rc/script/ui/puzzle/ui'

----------------------------------------------------------------------------

local scene_, sp_

function init(self)
  self    = ffi.cast("Puzzle*", self);
  scene_  = self:get_ui_scene()

  local btnSound_setting = {title='sound setting', x=800, y=400}
  local btnVideo_setting = {title='video setting', x=800, y=430}
  local btnSound = ui.new_ui_button(scene_, btnSound_setting)
  local btnVideo = ui.new_ui_button(scene_, btnVideo_setting)
  
  local btnSound1_setting = {title='level', x=800, y=400, alpha=0, visible=false}
  local btnSound2_setting = {title='turn on', x=800, y=430, alpha=0, visible=false}
  local btnSound3_setting = {title='turn off', x=800, y=460, alpha=0, visible=false}
  local btnSoundBack_setting = {title='back', x=800, y=500, alpha=0, visible=false}
  local btnSound1 = ui.new_ui_button(scene_, btnSound1_setting)
  local btnSound2 = ui.new_ui_button(scene_, btnSound2_setting)
  local btnSound3 = ui.new_ui_button(scene_, btnSound3_setting)
  local btnSoundBack = ui.new_ui_button(scene_, btnSoundBack_setting)

  local btnVideo1_setting = {title='size', x=800, y=400, alpha=0, visible=false}
  local btnVideo2_setting = {title='level', x=800, y=430, alpha=0, visible=false}
  local btnVideoBack_setting = {title='back', x=800, y=500, alpha=0, visible=false}
  local btnVideo1 = ui.new_ui_button(scene_, btnVideo1_setting)
  local btnVideo2 = ui.new_ui_button(scene_, btnVideo2_setting)
  local btnVideoBack = ui.new_ui_button(scene_, btnVideoBack_setting)
  
  local function mainpage_setvisible(visible)
    if(visible==true) then
      btnSound:set_fade(255)
      btnVideo:set_fade(255)
    else
      btnSound:set_fade(0)
      btnVideo:set_fade(0)
    end
  end
  
  local function soundpage_setvisible(visible)
    if(visible==true) then
      btnSound1:set_fade(255)
      btnSound2:set_fade(255)
      btnSound3:set_fade(255)
      btnSoundBack:set_fade(255)
    else
      btnSound1:set_fade(0)
      btnSound2:set_fade(0)
      btnSound3:set_fade(0)
      btnSoundBack:set_fade(0)
    end
  end
  
  local function videopage_setvisible(visible)
    if(visible==true) then
     btnVideo1:set_fade(255)
     btnVideo2:set_fade(255)
     btnVideoBack:set_fade(255)
    else
     btnVideo1:set_fade(0)
     btnVideo2:set_fade(0)
     btnVideoBack:set_fade(0)
    end
  end
  
  local Sound_on_press  = function(self)
                            mainpage_setvisible(false)
                            soundpage_setvisible(true)
                          end
  btnSound:on_press(Sound_on_press)
  
  local Video_on_press  = function(self)
                            mainpage_setvisible(false)
                            videopage_setvisible(true)
                          end
  btnVideo:on_press(Video_on_press)
  
  local SoundBack_on_press  = function(self)
                                mainpage_setvisible(true)
                                soundpage_setvisible(false)
                              end
  btnSoundBack:on_press(SoundBack_on_press)
  
  local VideoBack_on_press  = function(self)
                                mainpage_setvisible(true)
                                videopage_setvisible(false)
                              end
  btnVideoBack:on_press(VideoBack_on_press)
end