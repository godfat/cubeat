local ffi   = require 'ffi'
local C     = ffi.C
local view  = require 'rc/script/ui/view'
local ui    = require 'rc/script/ui/ui'

local ask_panel_ = nil

local function update(demo, scene, data)
    ask_panel_=ui.new_askbox{ parent=scene, w=800, h=480,
                              title="You only made "..data.." matches.\nTry a different strategy next time.",
                              cb={function(self)
                                    demo:eventual_resume()
                                    demo:endgame(0)
                                  end} }
    demo:eventual_pause()
end

return {
  update  = update
}