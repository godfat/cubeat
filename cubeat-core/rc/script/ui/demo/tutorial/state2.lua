local ffi   = require 'ffi'
local C     = ffi.C
local view  = require 'rc/script/ui/view'
local ui    = require 'rc/script/ui/ui'

local ask_panel_ = nil

local function update(demo, scene, data)
    if data > 4000 then
      ask_panel_=ui.new_askbox{ parent=scene, w=800, h=480,
                                title="Tutorial finished.\nSeems like you are doing just fine.",
                                cb={function(self)
                                      demo:eventual_resume()
                                      demo:endgame(1)
                                    end} }
    else
      ask_panel_=ui.new_askbox{ parent=scene, w=800, h=480,
                                title="You survived tutorial, but try harder\nto make bigger matches and bigger chains.",
                                cb={function(self)
                                      demo:eventual_resume()
                                      demo:endgame(1)
                                    end} }
    end
    demo:eventual_pause()
end

return {
  update  = update
}