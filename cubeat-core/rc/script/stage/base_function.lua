require 'rc/script/strict'
local ffi   = require 'ffi'
local C     = ffi.C
local view  = require 'rc/script/ui/view'

local layers = {}

local function three_stage_alpha_tween(sp, total_time, fade_time)
  local delayed_fade = function(self)
    local delay = total_time - fade_time*2
    sp:tween("Linear", "Alpha", 255, 0, fade_time, 0, function(self) self:remove() end, delay)
  end
  sp:tween("Linear", "Alpha", 0, 255, fade_time, 0, delayed_fade)
end

local function new_layer(o)
  local index = #layers+1
  if not o.parent then error(o.name.." has no parent!") end 
  layers[index] = view.new_sprite(o.name, o.parent, o.width, o.height, o.center)
  layers[index]:set_depth( o.layer and (-100*o.layer) or (-100*index) )
  return layers[index]
end

local function cleanup(texture_release)
  for i = 1, #layers do
    layers[i]:remove(texture_release)
    layers[i] = nil
  end
end

return {
  new_layer = new_layer,
  three_stage_alpha_tween = three_stage_alpha_tween,
  cleanup = cleanup
}
