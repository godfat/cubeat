local ffi   = require 'ffi'
local C     = ffi.C
local view  = require 'rc/script/ui/view'
local ui    = require 'rc/script/ui/ui'
local switch= require 'rc/script/ui/demo/switch/switch'

local record        = require 'rc/script/ui/demo/challenge/record'
local achieve_list  = require 'rc/script/ui/demo/challenge/achievement_list'
local achieve_desc  = require 'rc/script/ui/demo/challenge/achievement_desc_EN'

local root_

local Input1      = C.Input_get_input1()
local Input2      = C.Input_get_input2()

local function enter_text(tip, hint)
  return function(self)
            tip:change_text(hint)
            tip:set_visible(true)
            
            -- local pos_x = C.Input_get_cursor_x(Input1)
            -- local pos_y = C.Input_get_cursor_y(Input1)
            tip:set_pos(0, 180)
            
            local s1 = ffi.new("v3", 0.1, 0.1, 0.1)
            local e1 = ffi.new("v3", 0.8, 0.8, 1)
            tip:tween("OElastic", "Scale", s1, e1, 500)
         end
end

local function leave_text(tip)
  return function(self) tip:set_visible(false) end
end

local function init(demo, parent)
  local menu = {}
  
  local c_w = view.GET_SCREEN_W()/2
  local c_h = view.GET_SCREEN_H()/2
  
  root_ = view.new_sprite("blahblah", parent, 0, 0, true)
  root_:set_pos(c_w, c_h)
  
  menu.bg = ui.new_image9s{ parent=root_, path='textarea2', x=0, y=0, 
                            w=1200, h=640, w1=34, w2=32, h1=38, h2=35, center=true }
  
  --menu.square = ui.new_image{ parent=root_, path='blahblah', x=0, y=60, w=1280, h=600 }
  --menu.square:set_alpha(128)
  --menu.square:set_color(0,0,0)
  
  menu.left_btn   = ui.new_text{ parent=root_, x=-340, y=0, size=32, title='<<', center=true }
  menu.left_btn:set_scale(2)
  menu.left_btn:set_depth(-30)
  
  menu.right_btn  = ui.new_text{ parent=root_, x=340, y=0, size=32, title='>>', center=true }
  menu.right_btn:set_scale(2)
  menu.right_btn:set_depth(-30)
  
  menu.btn_back = ui.new_text{ parent=root_, x=0, y=240, size=40, title='back', depth=-200, center=true }
  menu.btn_back:set_depth(-30)
  
  local achieve_size= table.getn(achieve_list)
  local page_size   = math.ceil(achieve_size/9)
  
  for i=1,page_size do
    menu["page_" .. tostring(i)] = ui.new_text{ parent=root_, x=0, y=-260, size=40, title="Page " .. tostring(i), center=true }
    if i~=1 then menu["page_" .. tostring(i)]:set_visible(false) end
  end
  
  menu["tip"] = ui.new_text{parent=root_, x=160, y=-260, size=40, title="tip", center=true}
  menu["tip"]:set_visible(false)
  
  for i, v in ipairs(achieve_list) do
    local name, hint
    local rec_value = record.load_raw("achieve_" .. tostring(achieve_list[i]))
    if not achieve_desc[achieve_list[i]].secret or rec_value then
      name  = achieve_desc[achieve_list[i]].name
      hint  = achieve_desc[achieve_list[i]].hint
    else
      name = "???"; hint = "???"
    end
    menu["achevement_" .. tostring(i)] = ui.new_text{ parent=root_, x=0, y= -240+((i-1)%9+1)*40, size=24, title=name, depth=-300, center=true }
    
    if not rec_value then 
      menu["achevement_" .. tostring(i)]:set_alpha(150) 
    end
    
    menu["achevement_" .. tostring(i)]:on_enter_focus( enter_text(menu["tip"], hint), Input1 )
    menu["achevement_" .. tostring(i)]:on_leave_focus( leave_text(menu["tip"]), Input1 )
    if i > 9 then menu["achevement_" .. tostring(i)]:set_visible(false) end
  end
  
  --[[
  for i=0,4 do
    menu["title_" .. tostring(i*10)] = ui.new_text{ parent=root_, x=640, y=180, size=40, title="Title " .. tostring(i+1), center=true }
    if i~=0 then menu["title_" .. tostring(i*10)]:set_visible(false) end
  end
  
  for i=1,9 do
    menu["achevement_" .. tostring(   i)] = ui.new_text{ parent=root_, x=640, y= 200+i*40, size=32, title='Achievement ' .. tostring(   i), center=true }
    menu["achevement_" .. tostring(10+i)] = ui.new_text{ parent=root_, x=640, y= 200+i*40, size=32, title='Achievement ' .. tostring(10+i), center=true, visible=false }
    menu["achevement_" .. tostring(20+i)] = ui.new_text{ parent=root_, x=640, y= 200+i*40, size=32, title='Achievement ' .. tostring(20+i), center=true, visible=false }
    menu["achevement_" .. tostring(30+i)] = ui.new_text{ parent=root_, x=640, y= 200+i*40, size=32, title='Achievement ' .. tostring(30+i), center=true, visible=false }
    menu["achevement_" .. tostring(40+i)] = ui.new_text{ parent=root_, x=640, y= 200+i*40, size=32, title='Achievement ' .. tostring(40+i), center=true, visible=false }
  end
  --]]
  
  local current_page = 1

  menu.left_btn:on_press(function(self)
    for i=1,9 do
      if menu["achevement_" .. tostring((current_page-1)*9+i)] then
        menu["achevement_" .. tostring((current_page-1)*9+i)]:set_visible(false)
      end
    end
    menu["page_" .. tostring(current_page)]:set_visible(false)
    
    current_page = current_page - 1
    
    if current_page==0 then current_page=page_size end
    for i=1,9 do
      if menu["achevement_" .. tostring((current_page-1)*9+i)] then
        menu["achevement_" .. tostring((current_page-1)*9+i)]:set_visible(true)
      end
    end
    menu["page_" .. tostring(current_page)]:set_visible(true)
  end)
  
  menu.right_btn:on_press(function(self)
    for i=1,9 do
      if menu["achevement_" .. tostring((current_page-1)*9+i)] then
        menu["achevement_" .. tostring((current_page-1)*9+i)]:set_visible(false)
      end
    end
    menu["page_" .. tostring(current_page)]:set_visible(false)
    
    current_page = current_page + 1
    
    if current_page>page_size then current_page=1 end
    for i=1,9 do
      if menu["achevement_" .. tostring((current_page-1)*9+i)] then
        menu["achevement_" .. tostring((current_page-1)*9+i)]:set_visible(true)
      end
    end
    menu["page_" .. tostring(current_page)]:set_visible(true)
  end)

  menu.btn_back:on_press(function(self)
    switch.load_page('extramenu')
  end)
  
  return menu
end

return {
  init = init
}