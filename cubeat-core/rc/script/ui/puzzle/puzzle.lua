local ffi = require 'ffi'
local C   = ffi.C
local ui  = require 'rc/script/ui/ui'
require 'rc/script/ui/puzzle/defs'

----------------------------------------------------------------------------

local scene_

function init(self)
  self    = ffi.cast("Puzzle*", self);
  scene_  = self:get_ui_scene()

  local main      = ui.new_image{parent=scene_, path='area_rect', x=800, y=400, w=300, h=200}
  local btnSound  = ui.new_text{parent=main._cdata, title='sound setting', x=30, y=30}
  local btnVideo  = ui.new_text{parent=main._cdata, title='video setting', x=30, y=60}
  
  local ratio     = ui.new_ratio{parent=main._cdata, title='test ratio', x=30, y=90}
  local ratio_press = function(self) end
  ratio:on_press(ratio_press)
  
  local selectbox = ui.new_selectbox{parent=main._cdata, x=30, y=120, list={'john', 'kevin', 'duo'}}
  
  local sound         = ui.new_image{parent=scene_, path='area_rect', x=800, y=400, w=300, h=200, visible=false}
  local btnSound1     = ui.new_text{parent=sound._cdata, title='level',     x=30, y=30}
  local btnSound2     = ui.new_text{parent=sound._cdata, title='turn on',   x=30, y=60}
  local btnSound3     = ui.new_text{parent=sound._cdata, title='turn off',  x=30, y=90}
  local btnSoundBack  = ui.new_text{parent=sound._cdata, title='back',      x=30, y=140}
  
  local video         = ui.new_image{parent=scene_, path='area_rect', x=800, y=400, w=300, h=200, visible=false}
  local btnVideo1     = ui.new_text{parent=video._cdata, title='size',  x=30, y=30}
  local btnVideo2     = ui.new_text{parent=video._cdata, title='level', x=30, y=60}
  local btnVideoBack  = ui.new_text{parent=video._cdata, title='back',  x=30, y=140}
  
  local function main_setvisible(visible)
    if(visible) then
      main:set_fade{s=0, e=255, cb=nil}
      btnSound:set_fade{s=0, e=255, cb=nil}
      btnVideo:set_fade{s=0, e=255, cb=nil}
    else
      main:set_fade{s=255, e=0, cb=function(self) self:set_visible(false) end}
      btnSound:set_fade{s=255, e=0, cb=nil}
      btnVideo:set_fade{s=255, e=0, cb=nil}
    end
  end
  
  local function sound_setvisible(visible)
    if(visible) then
      sound:set_fade{s=0, e=255, cb=nil}
      btnSound1:set_fade{s=0, e=255, cb=nil}
      btnSound2:set_fade{s=0, e=255, cb=nil}
      btnSound3:set_fade{s=0, e=255, cb=nil}
      btnSoundBack:set_fade{s=0, e=255, cb=nil}
    else
      sound:set_fade{s=255, e=0, cb=function(self) self:set_visible(false) end}
      btnSound1:set_fade{s=255, e=0, cb=nil}
      btnSound2:set_fade{s=255, e=0, cb=nil}
      btnSound3:set_fade{s=255, e=0, cb=nil}
      btnSoundBack:set_fade{s=255, e=0, cb=nil}
    end
  end
  
  local function video_setvisible(visible)
    if(visible) then
      video:set_fade{s=0, e=255, cb=nil}
      btnVideo1:set_fade{s=0, e=255, cb=nil}
      btnVideo2:set_fade{s=0, e=255, cb=nil}
      btnVideoBack:set_fade{s=0, e=255, cb=nil}
    else
      video:set_fade{s=255, e=0, cb=function(self) self:set_visible(false) end}
      btnVideo1:set_fade{s=255, e=0, cb=nil}
      btnVideo2:set_fade{s=255, e=0, cb=nil}
      btnVideoBack:set_fade{s=255, e=0, cb=nil}
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
  
  --
  local ask = ui.new_askbox{parent=scene_}
  
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
  
  --
  local score = ui.new_list{parent=scene_, visible=false}
  score:load_list('score')
  for k, v in pairs(score.list) do
    print(k, v)
  end
  
  local score_list1 ={yui     = 100,
                      jessica = 1500,
                      roy     = 35}
  local score_list2 ={john    = 100,
                      alex    = 200,
                      bill    = 300,
                      jj      = 400}
  local show_list = function(list_type)
                      local show = function(self)
                        if list_type == 1 then
                          score:set_list(score_list1)
                        else
                          score:set_list(score_list2)
                        end
                        score:save_list('score')
                        
                        score:set_visible(true)
                        video:set_visible(false)
                      end
                      return show
                    end
  local hide_list = function(self)
                      score:set_visible(false)
                      video:set_visible(true)
                    end
  btnVideo1:on_press(show_list(1))
  btnVideo2:on_press(show_list(2))
  score:on_press_back(hide_list)
end