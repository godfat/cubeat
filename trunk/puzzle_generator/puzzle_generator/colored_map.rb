
require File.join(File.dirname(__FILE__), 'misc')

module PuzzleGenerator
  class ColoredMap
    include DisplayMap, MapUtils
    def initialize chained_map, colors = (1..chained_map.option[:colors]).to_a
      @result_map = []
      @maps = []
      @chained_map = chained_map
      @option = @chained_map.option

      debug_init if PuzzleGenerator.debug

      chained_map.maps.each_with_index{ |map, index|
        @maps << map.clone_with_map{ |color|
          color == 0 ? 0 : colors[index % @option[:colors]]
        }
      }
      resolve_map
      @result_map = if check_no_invoke && strip_answer && check_answer_correctness
                      put_answer_back; @result_map
                    else
                      GenerationFailed.new("ColoredMap: last result_map: #{@result_map.inspect}")
                    end
    end

    private
    def debug_init
      @debug_answer = @chained_map.maps.last.each_with_index_2d{ |i, x, y| break i if i != 0 }
    end
    def check_no_invoke
      m = Map.new :data => @result_map, :option => @option
      m.each_with_index_2d{ |i, x, y|
        return false if check_left_chain( m, x, y) ||
                        check_right_chain(m, x, y) ||
                        check_up_chain(   m, x, y) ||
                        check_down_chain( m, x, y)
      }
      true
    end
    def strip_answer
      @x, @y = @chained_map.maps.last.each_with_index_2d{ |i, x, y| break [x, y] if i != 0 }
      # map = Map.new :data => @result_map, :option => @option
      # return false unless !check_left_chain( map, @x, @y) &&
      #                     !check_right_chain(map, @x, @y) &&
      #                     !check_up_chain(   map, @x, @y) &&
      #                     !check_down_chain( map, @x, @y)
      @answer_color = @result_map[@x][@y]
      @result_map[@x][@y] = 0
      true
    end
    def put_answer_back
      @result_map[@x][@y] = @debug_answer || @answer_color
      @x, @y, @answer_color = nil
    end
  end

end
