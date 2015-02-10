local ffi     = require 'ffi'
local C       = ffi.C
local view    = require 'rc/script/ui/view'
local ui      = require 'rc/script/ui/ui'
local switch  = require 'rc/script/ui/demo/switch/switch'
local script  = require 'rc/script/ui/demo/staffroll/script'

local root_         = nil
local demo_game_    = nil
local data_         = nil
local roll_content_ = {}
local character_id_ = nil

local function init(demo, parent, data)
  local menu = {}
  root_         = view.new_sprite("blahblah", parent, 0, 0, false)
  demo_game_    = demo
  data_         = data
  roll_content_ = script.get_staffroll()
  character_id_ = data_.character or ( 1 and print("-------- No Character ID !!!! --------") )
  
  menu.background = ui.new_image{ parent = root_, path='nothing', x=0, y=0, w=1280, h=768, depth=-400 }
  menu.background:set_red(0)
  menu.background:set_blue(0)
  menu.background:set_green(0)
  
  -- roll root, all content's parent
  menu.roll = view.new_sprite("blahblah", root_, 0, 0, false)
  
  -- create roll content
  local default_size  = script.get_default_size()
  local default_depth = script.get_default_depth()
  for k,v in pairs(roll_content_) do
    -- text
    if v.text then
      menu[k] = ui.new_text{ parent = menu.roll, x=v.x or 0, y=v.y or 0, size=32, title=v.text, size=v.size or default_size, depth=v.depth or default_depth }
      menu[k]:set_visible(false)
    -- image
    elseif v.img then
      menu[k] = ui.new_image{ parent = menu.roll, path=v.img, x=v.x or 0, y=v.y or 0, w=v.w or 0, h=v.h or 0, depth=v.depth or default_depth }
      if v.flip then menu[k]:texture_flipH() end
      menu[k]:set_visible(false)
    end
  end
  
  -- create endcg, then hide it.
  local cg_path = 'endcg/char' .. tostring(character_id_)
  menu.endcg = ui.new_image{ parent = root_, path = cg_path, x=0, y=0, w=view.GET_SCREEN_W(), h=view.GET_SCREEN_H(), depth=-500 }
  menu.endcg:set_visible(false)
  
  -- create callback functions
  local function set_endcg_onpress()
    menu.endcg:on_press( function(self) 
      switch.load_page('mainmenu', nil, nil) 
      demo:leave_and_cleanup(false) -- cleanup again so there's a transition back to mainmenu
    end )
  end
  local function show_cg()
    menu.endcg:set_visible(true)
    menu.endcg:tween('Linear', 'Alpha', 0, 255, 3000, 0, set_endcg_onpress, 0)
  end
  local function fade_out()
    menu[table.getn(roll_content_)]:tween('Linear', 'Alpha', 255, 0, 3000, 0, show_cg, 3000 )
  end
  local function startroll()
    for k,v in pairs(menu) do
      if k~="endcg" then v:set_visible(true) end
    end
    local roll_distance = script.get_roll_distance()
    local roll_time     = script.get_roll_time(character_id_)
    menu.roll:tween('Linear', 'Pos2D', ffi.new("value2", 0, 0), ffi.new("value2", 0, roll_distance),
                    roll_time, 0, fade_out, 0)
  end
  
  menu.background:tween('Linear', 'Alpha', 0, 128, 3000, 0, startroll, 0)
  
  return menu
end

return {
  init  = init
}