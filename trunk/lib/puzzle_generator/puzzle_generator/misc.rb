
require 'rubygems'
require 'facets'          # for Array#deep_clone
# require 'ludy/ludy_ext' # for Kernel#curry

module PuzzleGenerator
  Up, Right, Left = (0..2).to_a
  DefaultOption = {:width => 6, :height => 10, :level => 4, :colors => 4,
                   :invoke => 3, :invoke_max => 5, :timeout => 5}
  class GenerationFailed < RuntimeError; end
  class << self
    attr_writer :debug
    def debug; @debug || false; end
  end

  module DisplayMap
    attr_reader :result_map
    def display_map
      result_map.transpose.reverse_each{ |row| puts row.map{ |color| '%2d' % color }.join(' ') }
    end
  end

  module MapUtils
    def make_map_array; (Array.new(@option[:width])).map{ [0]*@option[:height] }; end
    def resolve_map result_map = @result_map, maps = @maps
      result_map.replace maps.inject(make_map_array){ |result, map|
        combine_map result, map
      }
    end
    def combine_map result, map
      result.zip(map.to_a).map{ |columns|
        orig, last = columns
        orig.combine last
      }
    end
    def check_answer_correctness result_map = @result_map
      map = Map.new @option.merge(:data => result_map.deep_clone)
      drop_blocks map # because of answer is stripped

      @chained = true
      while @chained
        @chained = false
        destory_chains map
        drop_blocks    map
      end
      @chained = nil

      map.all?{ |i| i == 0 }
    end
    def destory_chains map
      map.each_column_with_index{ |column, x|
        column.each_with_index{ |value, y|
          next if value == 0

          left  = check_left_chain  map, x, y
          right = check_right_chain map, x, y
          up    = check_up_chain    map, x, y
          down  = check_down_chain  map, x, y

          # left.fill  0 unless left.nil?
          # right.fill 0 unless right.nil?
          # up.fill    0 unless up.nil?
          # down.fill  0 unless down.nil?
          left.size.times{ |offset|
            map[x-offset, y] = 0
          } unless left.nil?
          right.size.times{ |offset|
            map[x+offset, y] = 0
          } unless right.nil?
          up.size.times{ |offset|
            map[x, y+offset] = 0
          } unless up.nil?
          down.size.times{ |offset|
            map[x, y-offset] = 0
          } unless down.nil?

          @chained ||= left || right || up || down
        }
      }
    end
    def drop_blocks map
      map.each_column{ |column| column.map!{ |i| i==0 ? nil : i }.compact!.pad!(@option[:height], 0) }
    end
    def check_left_chain map, x, y
      # this should be rewrited
      return nil if map[x, y] == 0
      left = x - @option[:invoke] + 1
      return nil if left < 0
      # chain = map[left..x, y]
      # chain if chain.all?{ |i| i == map[x, y] }
      # map[0..x, y].inject([]){ |result, value| result << value if value == map[x, y] }
      do_check_chain map[0..x, y].reverse, map[x, y]
    end
    def check_right_chain map, x, y
      return nil if map[x, y] == 0
      right = x + @option[:invoke] - 1
      return nil if right >= @option[:width]
      # chain = map[x..right, y]
      # chain if chain.all?{ |i| i == map[x, y] }
      do_check_chain map[x...@option[:width], y], map[x, y]
    end
    def check_up_chain map, x, y
      return nil if map[x, y] == 0
      up = y + @option[:invoke] - 1
      return nil if up >= @option[:height]
      # chain = map[x, y..up]
      # chain if chain.all?{ |i| i == map[x, y] }
      do_check_chain map[x, y...@option[:height]], map[x, y]
    end
    def check_down_chain map, x, y
      return nil if map[x, y] == 0
      down = y - @option[:invoke] - 1
      return nil if down < 0
      # chain = map[x, down..y]
      # chain if chain.all?{ |i| i == map[x, y] }
      do_check_chain map[x, 0..y].reverse, map[x, y]
    end
    # def check_chain target_color, result, value; result << value if value == target_color; end
    def do_check_chain target, target_color
      # target.inject([], &method(:check_chain).curry[target_color])
      chain = target.inject([]){ |result, value|
        if value == target_color
          result << value
        else
          break result
        end
      }
      chain.size >= @option[:invoke] ? chain : nil
    end
  end

end
