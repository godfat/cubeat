local ffi   = require 'ffi'
local jit   = require 'jit'
local C     = ffi.C
local view  = require 'rc/script/ui/view'
local ui    = require 'rc/script/ui/ui'
local event = require 'rc/script/event/event'
require 'rc/script/strict'
require 'rc/script/ai/ai' -- we have to use AI script here if we ant to control AI player
                          -- but then we'll also need an AI handle... : / 
local function setcmd(buf, type, delay, x, y)
  buf.x, buf.y, buf.delay, buf.type = x, y, delay, type
end

local demo_ 
local scene_ 
local game_scene_
local into_emergency_ = false

local tut_phase_ = 1
local tut_actions_ = {}
local tut_checkpoints_ = {}
local haste_counter_ = 0
local ask_panel_ = nil

local retry_button_ = nil
local skip_button_  = nil
local retry_bg_     = nil
local skip_bg_      = nil

local monocube_hint_= nil
local gauge_hint1_  = nil
local gauge_hint2_  = nil
local heat_hint_    = nil

local map_blocker1_ = nil
local map_blocker2_ = nil
local main_blocker_ = nil
local main_text_    = nil

local function set_phase(ph)
  tut_phase_ = ph
end

local function show_retry_skip(flag)
  flag = flag == nil and true or flag
  retry_button_:set_visible(flag)
  skip_button_:set_visible(flag)
  retry_bg_:set_visible(flag)
  skip_bg_:set_visible(flag)
end

local function retry_func()
  if tut_phase_ < 8 then 
    tut_phase_ = 1 
    haste_counter_ = 0
    demo_:init_map_with_config(0, 'tutorial.zzml')
    demo_:init_map_with_config(1, 'tutorial.zzml')
    demo_:set_map_garbage_amount(0, 0)
    demo_:set_map_garbage_amount(1, 0)
    demo_:reset_map_record(0)
    demo_:set_map_dropping(0, false)
    demo_:set_map_dropping(1, false)
    show_main_blocker()
  elseif tut_phase_ >= 8 and tut_phase_ < 11 then
    tut_phase_ = 8
    haste_counter_ = 350
    demo_:init_map_with_config(0, 'tutorial.zzml')
    demo_:set_map_garbage_amount(0, 3)
    demo_:set_map_dropping(0, false)
    demo_:reset_map_record(0)
  else
    tut_phase_ = 11 
    into_emergency_ = false
  end
  show_retry_skip(false)
end

local function show_main_blocker()
  if main_blocker_ then main_blocker_:remove(); main_blocker_ = nil end
  if main_text_    then main_text_:remove(); main_text_ = nil end
  
  local text = tut_phase_ < 11 and 
               'チュートリアル　パート１' or 
               'チュートリアル　パート２'
  
  main_blocker_ = ui.new_image{ parent = scene_, 
                                path='blahblah', w=1280, h=420, center = true,
                                x=640, y=360, depth=-10, alpha=128, visible=true } 
  main_blocker_:set_color(0,0,0)
  
  main_text_ = ui.new_text{ parent = scene_, 
                            title=text, x=640, y=360, size=32, center = true,
                            depth=-35, font="rounded-mplus-1m-medium" }
  main_text_:set_scale(2.0)
  
  event.on_timer("game", function()
    main_blocker_:tween("Linear", "Alpha", 128, 0, 500)
    main_text_:tween("Linear", "Alpha", 128, 0, 500)
  end, 2000)
  event.on_timer("game", function()
    main_blocker_:set_visible(false)
    main_text_:set_visible(false)
  end, 2500)
end

local function init(demo)
  demo_  = demo
  scene_ = demo_:get_ui_scene()
  game_scene_ = demo_:get_game_scene()
  
  -- prepare a dialogue for tutorial
  ask_panel_ = ui.new_askbox{ parent=scene_, w=1280, h=420, font="rounded-mplus-1m-medium",
                              title="Tutorial",
                              cb={function(self)
                                demo_:eventual_resume()
                                tut_phase_ = tut_phase_ + 1
                                ask_panel_:set_visible(false)
                                if tut_phase_ <= 16 then show_retry_skip(true) end
                              end} }
  ask_panel_:set_visible(false)
    
  map_blocker1_ = ui.new_image{ parent = game_scene_, 
                                path='blahblah', w=384, h=640, center = true,
                                x=287, y=362, depth=-20, alpha=128, visible=false }
  map_blocker1_:set_color(0,0,0)
  
  map_blocker2_ = ui.new_image{ parent = game_scene_, 
                                path='blahblah', w=384, h=640, center = true,
                                x=994, y=362, depth=-20, alpha=128, visible=false }
  map_blocker2_:set_color(0,0,0)
  
  retry_button_ = ui.new_text{ parent = scene_, title='Retry', x=7, y=680, size=32, depth=-20 }
  retry_button_:set_scale(1.0)
  retry_button_:on_press(function(self)
    retry_func()
  end)
  
  retry_bg_ = ui.new_image{ parent = scene_, path='blahblah', w=95, h=38, 
                            x=0, y=682, depth=-10, alpha=128, visible=true }
  retry_bg_:set_color(0,0,0)
                            
  skip_button_ = ui.new_text{ parent = scene_, title='Skip', x=1200, y=680, size=32, depth=-20 }
  skip_button_:set_scale(1.0)
  skip_button_:on_press(function(self)
    if tut_phase_ < 11 then 
      tut_phase_ = 11 
      into_emergency_ = false
    else 
      demo_:endgame(1) 
    end
    show_retry_skip(false)
  end)
  skip_bg_ = ui.new_image{ parent = scene_, path='blahblah', w=95, h=38, 
                           x=1185, y=682, depth=-10, alpha=128, visible=true }
  skip_bg_:set_color(0,0,0)
    
  show_retry_skip(false)
    
  if tut_phase_ < 2 then
    show_main_blocker()
  end
end

local function update()
  if tut_phase_ > #tut_actions_ then return end
  if tut_checkpoints_[tut_phase_]() then
    ask_panel_:set_visible(true)
    demo_:eventual_pause()
    tut_actions_[tut_phase_]()
  end
end

local function cleanup()
  tut_phase_ = 1
  haste_counter_ = 0
  into_emergency_ = false
  ask_panel_:set_visible(false)
  ask_panel_:remove_cb()
  map_blocker1_:remove()
  map_blocker2_:remove()
  main_blocker_:remove()
  retry_button_:remove()
  skip_button_:remove()
  retry_bg_:remove()
  skip_bg_:remove()
  gauge_hint1_:remove()
  gauge_hint2_:remove()
  heat_hint_:remove()
  ask_panel_ = nil
  map_blocker1_ = nil
  map_blocker2_ = nil
  main_blocker_ = nil
  retry_button_ = nil
  skip_button_ = nil
  retry_bg_ = nil
  skip_bg_ = nil
  gauge_hint1_ = nil
  gauge_hint2_ = nil
  heat_hint_ = nil
end

----------/////////////----------

tut_checkpoints_[1] = function()
  haste_counter_ = haste_counter_ + 1
  if haste_counter_ > 140 then return true end
  return false
end

-- we need an introduction to player map and player cursor before everything.

tut_actions_[1] = function()
  map_blocker1_:set_visible(true)
  map_blocker2_:set_visible(false)
  show_retry_skip(false)
  --ask_panel_:set_title("Tutorial about haste")
  ask_panel_:set_title("1-1 お手本（右）\n\nキューブは上から落ちてくる！\nマウス右クリックを長押しするとスピードが加速する")
  
  -- should not just show the text here. it should directly be the indication of your map and your cursor
  -- a highlight/mask effect 
  -- but in this tutorial we should avoid of speaking: "You are RED, you shoot left"
  -- instead, say: "Player 1 is RED, and Player 1's stage is on the left" 
  -- so we don't deny the possibility to "shoot others"
  
  demo_:set_map_dropping(0, false)
  demo_:set_map_dropping(1, true)

  local aiplayer = demo_:get_ai_player()
  local cmdbuf   = ffi.new("LuaAICommand", {0, 0, 0, C.AI_NONE}) 
  setcmd(cmdbuf, C.AI_HASTE, 1000, 0, 0)
  aiplayer:push_command(cmdbuf)
  setcmd(cmdbuf, C.AI_HASTE, 0, 0, 0)
  aiplayer:push_command(cmdbuf)
  
  haste_counter_ = 0
end
--
tut_checkpoints_[2] = function()
  haste_counter_ = haste_counter_ + 1
  if haste_counter_ > 150 then return true end
  return false
end

tut_actions_[2] = function()
  map_blocker1_:set_visible(false)
  map_blocker2_:set_visible(true)
  show_retry_skip(false)
  --ask_panel_:set_title("Tutorial about haste:\nYour turn")
  ask_panel_:set_title("1-1 プレイヤー操作（左）\n\n右クリックを長押しして\nキューブを早く落ちましょう")
  
  demo_:set_map_dropping(0, true)
  demo_:set_map_dropping(1, false)
  haste_counter_ = 0
end
--
tut_checkpoints_[3] = function()
  if view.is_haste_button_down(view.Input1) then
    haste_counter_ = haste_counter_ + 1
  end
  if haste_counter_ > 60 then 
    return true 
  end
  return false
end

tut_actions_[3] = function()
  map_blocker1_:set_visible(true)
  map_blocker2_:set_visible(false)
  show_retry_skip(false)
  --ask_panel_:set_title("Tutorial about shooting")
  ask_panel_:set_title("1-2 お手本\n\nキューブを左クリックすると消せる！")
  
  demo_:set_map_dropping(0, false)
  demo_:reset_map_record(0)
  
  -- AI setup shooting here   
  local aiplayer = demo_:get_ai_player()
  local cmdbuf   = ffi.new("LuaAICommand", {0, 0, 0, C.AI_NONE}) 
  setcmd(cmdbuf, C.AI_SHOOT, 500, 3, 2)
  aiplayer:push_command(cmdbuf)
  haste_counter_ = 0;
end
--
tut_checkpoints_[4] = function()
  haste_counter_ = haste_counter_ + 1
  if haste_counter_ > 150 then return true end
  return false
end

tut_actions_[4] = function()
  map_blocker1_:set_visible(false)
  map_blocker2_:set_visible(true)
  show_retry_skip(false)
  --ask_panel_:set_title("Tutorial about shooting:\nYour turn")
  ask_panel_:set_title("1-2 プレイヤー操作\n\n左クリックしてキューブを消しましょう")
  
  haste_counter_ = 0
end
--
tut_checkpoints_[5] = function()
  local num = demo_:get_map_cubes_cleared_data(0)[1] + demo_:get_map_cubes_cleared_data(0)[2] +
              demo_:get_map_cubes_cleared_data(0)[3] + demo_:get_map_cubes_cleared_data(0)[4]
  if num >= 1 then
    haste_counter_ = haste_counter_ + 1
  end    
  if haste_counter_ > 40 then return true end
  return false
end

tut_actions_[5] = function()
  map_blocker1_:set_visible(true)
  map_blocker2_:set_visible(false)
  show_retry_skip(false)
  --ask_panel_:set_title("Tutorial about shooting cubes to align a match")
  ask_panel_:set_title("1-3 お手本\n\n同じ色のキューブを縦か横\n三つ以上並べると、消滅する！")
  
  local aiplayer = demo_:get_ai_player()
  local cmdbuf   = ffi.new("LuaAICommand", {0, 0, 0, C.AI_NONE}) 
  setcmd(cmdbuf, C.AI_SHOOT, 1000, 0, 1)
  aiplayer:push_command(cmdbuf)
  setcmd(cmdbuf, C.AI_SHOOT, 500, 2, 1)
  aiplayer:push_command(cmdbuf)
  
  haste_counter_ = 0;
end
--
tut_checkpoints_[6] = function()
  haste_counter_ = haste_counter_ + 1
  if haste_counter_ > 250 then return true end
  return false
end

tut_actions_[6] = function()
  map_blocker1_:set_visible(false)
  map_blocker2_:set_visible(true)
  show_retry_skip(false)
  --ask_panel_:set_title("Tutorial about a match:\nYour turn")
  ask_panel_:set_title("1-3 プレイヤー操作\n\n縦か横に三つ以上並べて消滅しましょう")
  
  demo_:reset_map_record(0)
end
--
tut_checkpoints_[7] = function()
  if demo_:get_map_highest_chain(0) >= 1 then 
    return true
  end
  return false
end

tut_actions_[7] = function() 
  map_blocker1_:set_visible(true)
  map_blocker2_:set_visible(false)
  show_retry_skip(false)
  --ask_panel_:set_title("Tutorial about chains")
  ask_panel_:set_title("1-4 お手本\n\n「チェーン消滅」するとモノキューブが\n対戦相手のフィールドにを落ちる")
  
  demo_:reset_map_record(0)
  
  demo_:set_map_dropping(1, true)

  local aiplayer = demo_:get_ai_player()
  local cmdbuf   = ffi.new("LuaAICommand", {0, 0, 0, C.AI_NONE}) 
  setcmd(cmdbuf, C.AI_SHOOT, 1000, 4, 0)
  aiplayer:push_command(cmdbuf)
  setcmd(cmdbuf, C.AI_SHOOT, 500, 5, 0)
  aiplayer:push_command(cmdbuf)
  setcmd(cmdbuf, C.AI_SHOOT, 500, 5, 0)
  aiplayer:push_command(cmdbuf)
  
  haste_counter_ = 0;
end
--
tut_checkpoints_[8] = function()
  haste_counter_ = haste_counter_ + 1
  if haste_counter_ > 350 then return true end
  return false
end

tut_actions_[8] = function()
  map_blocker1_:set_visible(false)
  map_blocker2_:set_visible(true)
  show_retry_skip(false)
  --ask_panel_:set_title("Tutorial about chains:\nYour turn")
  ask_panel_:set_title("1-4 プレイヤー操作(1)\n\n「チェーン消滅」をしましょう")
  
  demo_:set_map_dropping(1, false)
  
  -- do this so only the garbage will drop, there will be no new cubes
  event.on_timer("game", function()
    demo_:set_map_dropping(0, true)
  end, 500)
  event.on_timer("game", function()
    demo_:set_map_dropping(0, false) 
  end, 1000)
end
--
tut_checkpoints_[9] = function()
  demo_:hide_map_warning(1)
  if demo_:get_map_highest_chain(0) >= 2 then 
    return true
  end
  return false
end

tut_actions_[9] = function()
  show_retry_skip(false)
  demo_:hide_map_warning(1)
  --ask_panel_:set_title("Tutorial about chains:\nDon't forget to restore the broken cubes")
  ask_panel_:set_title("1-4 プレイヤー操作(2)\n\nモノキューブは消滅できないが、\n左クリックすると普通のキューブに戻れる\n全てのモノキューブを普通のキューブに戻りましょう")
 
  monocube_hint_ = ui.new_image{ parent = scene_, path = 'cubes/cube1', 
                                 w = 96, h = 96, x = 310, y = 205,
                                 depth = -50 }
                             
  -- do this so only the garbage will drop, there will be no new cubes
  demo_:set_map_dropping(1, true)
  event.on_timer("game", function()
    demo_:set_map_dropping(1, false)
    demo_:hide_map_warning(1)
  end, 2500)
  
  local aiplayer = demo_:get_ai_player()
  local cmdbuf   = ffi.new("LuaAICommand", {0, 0, 0, C.AI_NONE}) 
  setcmd(cmdbuf, C.AI_HASTE, 0, 0, 0)
  aiplayer:push_command(cmdbuf)
  setcmd(cmdbuf, C.AI_HASTE, 0, 0, 0)
  aiplayer:push_command(cmdbuf)
  setcmd(cmdbuf, C.AI_HASTE, 0, 0, 0)
  aiplayer:push_command(cmdbuf)
  
  haste_counter_ = 0
end
--
tut_checkpoints_[10] = function()
  monocube_hint_:set_visible(false)
  demo_:hide_map_warning(1)
  if demo_:get_map_broken_num(0) == 0 and demo_:get_map_broken_num(1) > 0 then 
    haste_counter_ = haste_counter_ + 1
  end
  if haste_counter_ > 30 then return true end
  return false
end

tut_actions_[10] = function()
  show_retry_skip(false)
  demo_:hide_map_warning(1)
  ask_panel_:set_title("よくできました！パート１完了。")
  haste_counter_ = 0
end
--
tut_checkpoints_[11] = function()
  return true
end

tut_actions_[11] = function()

  -- fuck this exception...
  demo_:eventual_resume()
  tut_phase_ = tut_phase_ + 1
  ask_panel_:set_visible(false)
  
  map_blocker1_:set_visible(true)
  map_blocker2_:set_visible(false)
  
  demo_:init_map_with_config(0, 'tutorial2-2.zzml')
  demo_:init_map_with_config(1, 'tutorial2.zzml')
  demo_:set_map_garbage_amount(0, 0)
  demo_:set_map_garbage_amount(1, 0)
  demo_:set_map_dropping(0, false)
  demo_:set_map_dropping(1, false)
  
  show_main_blocker()
  
  haste_counter_ = 0
end
--
tut_checkpoints_[12] = function()
  haste_counter_ = haste_counter_ + 1
  if haste_counter_ > 160 then return true end
  return false
end

tut_actions_[12] = function()
  show_retry_skip(false)
  --ask_panel_:set_title("Tutorial about overheat:\nDon't do it!")
  ask_panel_:set_title("2-1 お手本(1)\n\nキューブをクリックと加速は「ヒートケージ」\nをたまる、最大になるとオーバーヒートになって、\n動けなくなるが、時間が経つと回復する")
  
  heat_hint_ = ui.new_image{ parent = scene_, path = 'heat_cursor', 
                             w = 108, h = 108, x = 775, y = 175,
                             depth = -50 }
  
  demo_:set_map_dropping(1, true)
  
  local aiplayer = demo_:get_ai_player()
  local cmdbuf   = ffi.new("LuaAICommand", {0, 0, 0, C.AI_NONE}) 
  
  aiplayer:set_interval(200)
  
  setcmd(cmdbuf, C.AI_SHOOT, 1000, 0, 7)
  aiplayer:push_command(cmdbuf)
  for i = 1, 9 do 
    setcmd(cmdbuf, C.AI_SHOOT, 0, i % 6, 7 - math.floor(i/6) )  -- pre-determined width. no way to know here.
    aiplayer:push_command(cmdbuf)  
  end  
  
  haste_counter_ = 0
end
--
tut_checkpoints_[13] = function()
  heat_hint_:set_visible(false)
  haste_counter_ = haste_counter_ + 1
  if haste_counter_ > 270 then return true end
  return false
end

tut_actions_[13] = function()
  show_retry_skip(false)
  --ask_panel_:set_title("Tutorial about emergency")
  ask_panel_:set_title("2-1 お手本(2)\n\nキューブがフィールドの最上段まで\n詰まれると、「緊急状態」に入る")
  --、フィールドの上下の緊急ゲージが溜め始める
  
  gauge_hint1_ = ui.new_image{ parent = scene_, path = 'danger_gauge', 
                               w = 163, h = 31, x = 820, y = 85,
                               depth = -20 }
  gauge_hint1_:set_rotation(22.5)
  
  gauge_hint2_ = ui.new_image{ parent = scene_, path = 'danger_gauge', 
                               w = 163, h = 31, x = 825, y = 610,
                               depth = -20 }
  gauge_hint2_:set_rotation(-22.5)
  
  local aiplayer = demo_:get_ai_player()
  aiplayer:set_interval(375)
  haste_counter_ = 0
end
--
tut_checkpoints_[14] = function() 
  haste_counter_ = haste_counter_ + 1
  if haste_counter_ > 130 then return true end
  return false
end

tut_actions_[14] = function()
  show_retry_skip(false)
  --ask_panel_:set_title("Tutorial about emergency: about countdown lock")
  ask_panel_:set_title("2-1 お手本(3)\n\nキューブを並べて消滅するとゲージは一旦止る。\n")
  
  local aiplayer = demo_:get_ai_player()
  aiplayer:start_thinking()  
  
  event.on_timer("game", function()
    aiplayer:stop_thinking()
  end, 1500)
  
  haste_counter_ = 0
end
--
tut_checkpoints_[15] = function()
  gauge_hint1_:set_visible(false)
  gauge_hint2_:set_visible(false)
  
  haste_counter_ = haste_counter_ + 1
  if haste_counter_ > 240 then return true end
  return false
end

tut_actions_[15] = function()
  map_blocker1_:set_visible(false)
  map_blocker2_:set_visible(true)
  show_retry_skip(false)
  --ask_panel_:set_title("Tutorial about emergency: your turn")
  ask_panel_:set_title("2-1 プレイヤー操作\n\n緊急状態を解除しましょう")
  
  demo_:set_map_dropping(0, true)
  demo_:set_map_dropping(1, false)
  
  local aiplayer = demo_:get_ai_player()
  aiplayer:stop_thinking()
  
  haste_counter_ = 0
end
--

tut_checkpoints_[16] = function()
  if demo_:get_map_warning_level(0) > 3 then
    into_emergency_ = true
  end
  if into_emergency_ == true and demo_:get_map_warning_level(0) < 1 then
    haste_counter_ = haste_counter_ + 1
  end
  if haste_counter_ > 10 then return true end
  return false
end

tut_actions_[16] = function()
  map_blocker1_:set_visible(true)
  map_blocker2_:set_visible(false)
  show_retry_skip(false)
  -- ask_panel_:set_title("Tutorial about win/lose")
  ask_panel_:set_title("2-2 お手本\n\n緊急ゲージがフィールドの両端までたまると\nゲームオーバーになる")
  
  demo_:set_map_dropping(0, false)
  demo_:set_map_dropping(1, true)
  
  haste_counter_ = 0
end

return {
  init = init, 
  update = update, 
  set_phase = set_phase,
  cleanup = cleanup
}
