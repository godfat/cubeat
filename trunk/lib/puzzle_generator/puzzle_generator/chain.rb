
require File.join(File.dirname(__FILE__), 'misc')

module PuzzleGenerator

  class Chain
    include Enumerable
    def initialize position = [0, 0], direct = Up, invoke = DefaultOption[:invoke]
      @direct = direct
      x, y = position
      @data = case @direct
                when Up   : ([x]*invoke).zip((y...y+invoke).to_a)
                when Right: (x...x+invoke).to_a.zip([y]*invoke)
                when Left : (x-invoke+1..x).to_a.zip([y]*invoke)
              end
    end
    def <=> rhs; @data <=> rhs.instance_variable_get('@data'); end
    def == rhs; (self <=> rhs) == 0; end
    def each &block; @data.each &block; self end
    def [] index; @data[index]; end
    def to_a; @data.clone; end
    def up?; @direct == Up; end
    def bound_ok? max_width, max_height
      not @data.find{ |i|
        i.first >= max_width ||
        i.last >= max_height ||
        i.first < 0          ||
        i.last < 0
      }
    end
  end

end
