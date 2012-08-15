local ffi     = require 'ffi'
local C       = ffi.C
local view    = require 'rc/script/ui/view'
local ui      = require 'rc/script/ui/ui'
local config  = require 'rc/script/ui/demo/talk/config'


local function show(menu, ch, actor, content, panel)
  menu[actor]:set_visible(true)
end

local function hide(menu, ch, actor, content, panel)
  menu[actor]:set_visible(false)
end

local function slide_in(menu, ch, actor, content, panel)
  local act_x = config.act_x[ch]
  local act_y = config.act_y[ch]
  local out_x = config.out_x[ch]
  local out_y = config.out_y[ch]
  menu[actor]:tween("Linear", "Pos2D", ffi.new("value2", out_x, out_y), ffi.new("value2", act_x, act_y), 200)
end

local function slide_out(menu, ch, actor, content, panel)
  local act_x = config.act_x[ch]
  local act_y = config.act_y[ch]
  local out_x = config.out_x[ch]
  local out_y = config.out_y[ch]
  menu[actor]:tween("Linear", "Pos2D", ffi.new("value2", act_x, act_y), ffi.new("value2", out_x, out_y), 200)
end

local function fade_in(menu, ch, actor, content, panel)
  menu[actor]:tween("Linear", "Alpha", 0, 255, 1000)
end

local function fade_out(menu, ch, actor, content, panel)
  menu[actor]:tween("Linear", "Alpha", 255, 0, 1000)
end

local function shake_actor(menu, ch, actor, content, panel)
  local act_x = config.act_x[ch]
  local act_y = config.act_y[ch]
  menu[actor]:tween("OElastic", "Pos2D", ffi.new("value2", act_x-20, act_y), ffi.new("value2", act_x, act_y), 100, 5)
end

----

local function word_L(menu, ch, actor, content, panel)
  menu[content]:set_scale( 2 )
end

local function word_M(menu, ch, actor, content, panel)
  menu[content]:set_scale( 1 )
end

local function word_S(menu, ch, actor, content, panel)
  menu[content]:set_scale( 0.7 )
end

local function shake_word(menu, ch, actor, content, panel)
  local con_x = config.con_x[ch]
  local con_y = config.con_y[ch]
  menu[content]:set_scale(2)
  menu[content]:tween("OElastic", "Pos2D", ffi.new("value2", con_x-20, con_y), ffi.new("value2", con_x, con_y), 100, 5)
end


return {
  show      = show,
  hide      = hide,
  slide_in  = slide_in,
  slide_out = slide_out,
  fade_in   = fade_in,
  fade_out  = fade_out,
  shake_actor = shake_actor,
  ----
  word_L    = word_L,
  word_M    = word_M,
  word_S    = word_S,
  shake_word= shake_word,
}