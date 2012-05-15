local ffi       = require 'ffi'
local C         = ffi.C
local ui        = require 'rc/script/ui/puzzle/ui'

----------------------------------------------------------------------------

local scene_, sp_

function init(self)
  self    = ffi.cast("Puzzle*", self);
  scene_  = self:get_ui_scene()

  local main_setting  = {path='area_rect', x=800, y=400, w=300, h=200}
  local main          = ui.new_ui_image(scene_, main_setting)
  local btnSound_setting  = {title='sound setting', x=30, y=30}
  local btnVideo_setting  = {title='video setting', x=30, y=60}
  local btnSound  = ui.new_ui_button(main.pic, btnSound_setting)
  local btnVideo  = ui.new_ui_button(main.pic, btnVideo_setting)
  
  local sound_setting = {path='area_rect', x=800, y=400, w=300, h=200, alpha=0, visible=false}
  local sound         = ui.new_ui_image(scene_, sound_setting)
  local btnSound1_setting     = {title='level',     x=30, y=30,  alpha=0, visible=false}
  local btnSound2_setting     = {title='turn on',   x=30, y=60,  alpha=0, visible=false}
  local btnSound3_setting     = {title='turn off',  x=30, y=90,  alpha=0, visible=false}
  local btnSoundBack_setting  = {title='back',      x=30, y=140, alpha=0, visible=false}
  local btnSound1     = ui.new_ui_button(sound.pic, btnSound1_setting)
  local btnSound2     = ui.new_ui_button(sound.pic, btnSound2_setting)
  local btnSound3     = ui.new_ui_button(sound.pic, btnSound3_setting)
  local btnSoundBack  = ui.new_ui_button(sound.pic, btnSoundBack_setting)
  
  local video_setting = {path='area_rect', x=800, y=400, w=300, h=200, alpha=0, visible=false}
  local video         = ui.new_ui_image(scene_, video_setting)
  local btnVideo1_setting     = {title='size',  x=30, y=30,   alpha=0, visible=false}
  local btnVideo2_setting     = {title='level', x=30, y=60,   alpha=0, visible=false}
  local btnVideoBack_setting  = {title='back',  x=30, y=140,  alpha=0, visible=false}
  local btnVideo1     = ui.new_ui_button(video.pic, btnVideo1_setting)
  local btnVideo2     = ui.new_ui_button(video.pic, btnVideo2_setting)
  local btnVideoBack  = ui.new_ui_button(video.pic, btnVideoBack_setting)
  
  local function main_setvisible(visible)
    if(visible) then
      main:set_fade(255)
      btnSound:set_fade(255)
      btnVideo:set_fade(255)
    else
      main:set_fade(0)
      btnSound:set_fade(0)
      btnVideo:set_fade(0)
    end
  end
  
  local function sound_setvisible(visible)
    if(visible) then
      sound:set_fade(255)
      btnSound1:set_fade(255)
      btnSound2:set_fade(255)
      btnSound3:set_fade(255)
      btnSoundBack:set_fade(255)
    else
      sound:set_fade(0)
      btnSound1:set_fade(0)
      btnSound2:set_fade(0)
      btnSound3:set_fade(0)
      btnSoundBack:set_fade(0)
    end
  end
  
  local function video_setvisible(visible)
    if(visible) then
      video:set_fade(255)
      btnVideo1:set_fade(255)
      btnVideo2:set_fade(255)
      btnVideoBack:set_fade(255)
    else
      video:set_fade(0)
      btnVideo1:set_fade(0)
      btnVideo2:set_fade(0)
      btnVideoBack:set_fade(0)
    end
  end
  
  local btnSound_on_press = function(self)
                              main_setvisible(false)
                              sound_setvisible(true)
                            end
  btnSound:on_press(btnSound_on_press)
  
  local btnVideo_on_press = function(self)
                              main_setvisible(false)
                              video_setvisible(true)
                            end
  btnVideo:on_press(btnVideo_on_press)
  
  local btnSoundBack_on_press = function(self)
                                  main_setvisible(true)
                                  sound_setvisible(false)
                                end
  btnSoundBack:on_press(btnSoundBack_on_press)
  
  local btnVideoBack_on_press = function(self)
                                  main_setvisible(true)
                                  video_setvisible(false)
                                end
  btnVideoBack:on_press(btnVideoBack_on_press)
end