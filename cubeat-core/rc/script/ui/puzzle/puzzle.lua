local ffi = require 'ffi'
local C   = ffi.C
local ui  = require 'rc/script/ui/ui'
require 'rc/script/ui/puzzle/defs'

----------------------------------------------------------------------------

local scene_

function init(self)
  self    = ffi.cast("Puzzle*", self);
  scene_  = self:get_ui_scene()

  local main_setting  = {path='area_rect', x=800, y=400, w=300, h=200}
  local main          = ui.new_image(scene_, main_setting)
  local btnSound_setting  = {title='sound setting', x=30, y=30}
  local btnVideo_setting  = {title='video setting', x=30, y=60}
  local btnSound  = ui.new_button(main.pic, btnSound_setting)
  local btnVideo  = ui.new_button(main.pic, btnVideo_setting)
  
  local sound_setting = {path='area_rect', x=800, y=400, w=300, h=200, alpha=0, visible=false}
  local sound         = ui.new_image(scene_, sound_setting)
  local btnSound1_setting     = {title='level',     x=30, y=30,  alpha=0, visible=false}
  local btnSound2_setting     = {title='turn on',   x=30, y=60,  alpha=0, visible=false}
  local btnSound3_setting     = {title='turn off',  x=30, y=90,  alpha=0, visible=false}
  local btnSoundBack_setting  = {title='back',      x=30, y=140, alpha=0, visible=false}
  local btnSound1     = ui.new_button(sound.pic, btnSound1_setting)
  local btnSound2     = ui.new_button(sound.pic, btnSound2_setting)
  local btnSound3     = ui.new_button(sound.pic, btnSound3_setting)
  local btnSoundBack  = ui.new_button(sound.pic, btnSoundBack_setting)
  
  local video_setting = {path='area_rect', x=800, y=400, w=300, h=200, alpha=0, visible=false}
  local video         = ui.new_image(scene_, video_setting)
  local btnVideo1_setting     = {title='size',  x=30, y=30,   alpha=0, visible=false}
  local btnVideo2_setting     = {title='level', x=30, y=60,   alpha=0, visible=false}
  local btnVideoBack_setting  = {title='back',  x=30, y=140,  alpha=0, visible=false}
  local btnVideo1     = ui.new_button(video.pic, btnVideo1_setting)
  local btnVideo2     = ui.new_button(video.pic, btnVideo2_setting)
  local btnVideoBack  = ui.new_button(video.pic, btnVideoBack_setting)
  
  local function main_setvisible(visible)
    local alpha = 0
    if(visible) then alpha=255 end
    
    main:set_fade(alpha)
    btnSound:set_fade(alpha)
    btnVideo:set_fade(alpha)
  end
  
  local function sound_setvisible(visible)
    local alpha = 0
    if(visible) then alpha=255 end

    sound:set_fade(alpha)
    btnSound1:set_fade(alpha)
    btnSound2:set_fade(alpha)
    btnSound3:set_fade(alpha)
    btnSoundBack:set_fade(alpha)
  end
  
  local function video_setvisible(visible)
    local alpha = 0
    if(visible) then alpha=255 end

    video:set_fade(alpha)
    btnVideo1:set_fade(alpha)
    btnVideo2:set_fade(alpha)
    btnVideoBack:set_fade(alpha)
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
  
  --
  local ask_setting = {}
  local ask = ui.new_askbox(scene_, ask_setting)
  
  local show_ask  = function(title)
                      local show = function(self)
                        ask:set_title(title)
                        ask:set_visible(true)
                      end
                      return show
                    end
  local hide_ask  = function(self) ask:set_visible(false) end
  btnSound1:on_press( show_ask('Set level ?') )
  btnSound2:on_press( show_ask('Set turn on ?') )
  btnSound3:on_press( show_ask('Set turn off ?') )
  ask:on_press_ok(hide_ask)
  ask:on_press_cancel(hide_ask)
  ask:set_visible(false)
end