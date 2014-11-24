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

local function init(demo, parent, data)
  local menu = {}
  root_         = view.new_sprite("blahblah", parent, 0, 0, false)
  demo_game_    = demo
  data_         = data
  roll_content_ = script.get_staffroll()
  
  menu.background = ui.new_image{ parent = root_, path='nothing', x=0, y=0, w=1280, h=768 }
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
      menu[k] = ui.new_text{ parent = menu.roll, x=v.x or 0, y=v.y or 0, size=32, title=v.text, size=v.size or default_size }
    -- image
    elseif v.img then
      menu[k] = ui.new_image{ parent = menu.roll, path=v.img, x=v.x or 0, y=v.y or 0, w=v.w or 0, h=v.h or 0, depth=v.depth or default_depth }
      if v.flip then menu[k]:texture_flipH() end
    end
  end
  
  -- this callback function use for staffroll_btn_ test in demo.lua
  local function back_to_title() switch.load_page('mainmenu', nil, nil) end
  local function cb() menu[table.getn(roll_content_)]:tween('Linear', 'Alpha', 255, 0, 3000, 0, back_to_title, 3000 ) end
  
  -- this callback function use for talk_end() in talk.lua when story end
  --local function cb() demo_game_:leave_and_cleanup() end
  
  local roll_distance = script.get_roll_distance()
  local roll_time     = script.get_roll_time()
  menu.roll:tween('Linear', 'Pos2D', ffi.new("value2", 0, 0), ffi.new("value2", 0, roll_distance), roll_time, 0, cb, 0)
  
  return menu
end

return {
  init  = init
}