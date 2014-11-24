local ffi     = require 'ffi'
local C       = ffi.C
local view    = require 'rc/script/ui/view'
local ui      = require 'rc/script/ui/ui'
local switch  = require 'rc/script/ui/demo/switch/switch'
local talk    = require 'rc/script/ui/demo/talk/talk'

local ftime_ = {}

local function check_file_time(root, t)

  if ftime_[root] == nil then
    ftime_[root] = t
  end
  
  if ftime_[root] ~= t then
    print('---- refresh ----')
    
    local success = false
    if root=='rc/script/ui/demo/select/config.lua' then
      print('---- select config ----')
      package.loaded[string.sub(root,1,-5)] = nil --unrequire
      
      success = switch.refresh_page('rc/script/ui/demo/select/select.lua')
      
    elseif  root=='rc/script/ui/demo/talk/config.lua' or
            root=='rc/script/ui/demo/talk/script_TW.lua' or
            root=='rc/script/ui/demo/talk/script_JP.lua' or
            root=='rc/script/ui/demo/talk/script_EN.lua' or
            root=='rc/script/ui/demo/talk/script_end_TW.lua' or
            root=='rc/script/ui/demo/talk/script_end_JP.lua' or
            root=='rc/script/ui/demo/talk/script_end_EN.lua' then
      print('---- talk config ----')
      package.loaded[string.sub(root,1,-5)] = nil --unrequire
      package.loaded['rc/script/ui/demo/talk/effect'] = nil
      
      success = switch.refresh_page('rc/script/ui/demo/talk/talk.lua')
      
    elseif root == 'rc/script/ui/demo/staffroll/script.lua' then
      package.loaded[string.sub(root,1,-5)] = nil --unrequire
      success = switch.refresh_page('rc/script/ui/demo/staffroll/staffroll.lua')
      
    elseif root == 'rc/script/ui/demo/switch/master_menu.lua' then
      success = switch.refresh_master_menu()
    else
      success = switch.refresh_page(root)
    end
    
    if success then ftime_[root] = t end
  end

  print(root, ftime_[root])
end

return {
  check_file_time = check_file_time
}