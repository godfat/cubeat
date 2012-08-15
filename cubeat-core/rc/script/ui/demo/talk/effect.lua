local ffi     = require 'ffi'
local C       = ffi.C
local view    = require 'rc/script/ui/view'
local ui      = require 'rc/script/ui/ui'
local config  = require 'rc/script/ui/demo/talk/config'


local actor = {}
local word  = {}

actor.show = function(menu, ch, actor, content, panel)
  menu[actor]:set_visible(true)
end

actor.hide = function(menu, ch, actor, content, panel)
  menu[actor]:set_visible(false)
end

actor.slide_in = function(menu, ch, actor, content, panel)
  local act_x = config.act_x[ch]
  local act_y = config.act_y[ch]
  local out_x = config.out_x[ch]
  local out_y = config.out_y[ch]
  menu[actor]:set_visible(true)
  menu[actor]:tween("Linear", "Pos2D", ffi.new("value2", out_x, out_y), ffi.new("value2", act_x, act_y), 200)
end

actor.slide_out = function(menu, ch, actor, content, panel)
  local act_x = config.act_x[ch]
  local act_y = config.act_y[ch]
  local out_x = config.out_x[ch]
  local out_y = config.out_y[ch]
  menu[actor]:set_visible(true)
  menu[actor]:tween("Linear", "Pos2D", ffi.new("value2", act_x, act_y), ffi.new("value2", out_x, out_y), 200)
end

actor.fade_in = function(menu, ch, actor, content, panel)
  menu[actor]:set_visible(true)
  menu[actor]:tween("Linear", "Alpha", 0, 255, 1000)
end

actor.fade_out = function(menu, ch, actor, content, panel)
  menu[actor]:set_visible(true)
  menu[actor]:tween("Linear", "Alpha", 255, 0, 1000)
end

actor.shake = function(menu, ch, actor, content, panel)
  local act_x = config.act_x[ch]
  local act_y = config.act_y[ch]
  menu[actor]:set_visible(true)
  menu[actor]:tween("OElastic", "Pos2D", ffi.new("value2", act_x-20, act_y), ffi.new("value2", act_x, act_y), 100, 5)
end

----

word.size_L = function(menu, ch, actor, content, panel)
  menu[content]:set_scale( 1.7 )
end

word.size_M = function(menu, ch, actor, content, panel)
  menu[content]:set_scale( 1 )
end

word.size_S = function(menu, ch, actor, content, panel)
  menu[content]:set_scale( 0.7 )
end

word.shake = function(menu, ch, actor, content, panel)
  local con_x = config.con_x[ch]
  local con_y = config.con_y[ch]
  menu[content]:set_scale( 1.7 )
  menu[content]:tween("OElastic", "Pos2D", ffi.new("value2", con_x-20, con_y), ffi.new("value2", con_x, con_y), 100, 5)
end


return {
  actor = actor,
  word  = word
}