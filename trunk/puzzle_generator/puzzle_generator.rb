
# alpha at 2007.10.14

require File.join(File.dirname(__FILE__), 'puzzle_generator', 'puzzle')

require 'rubygems'
require 'facets'   # for Hash#reverse_merge
require 'facets/timer'

module PuzzleGenerator

  def self.generate_chained_map option = {}; generate_klass ChainedMap, option; end
  def self.generate_klass klass, option = {}
    option.reverse_merge! :timeout => 5
    generate(option[:timeout]){ klass.new option }
  end

  LastTriedInfo = {}
  def self.generate timeout = 5, &generator
    timer = Timer.new(timeout).start
    tried_times = 1
    begin
      result = generator.call
      until result.result_map.kind_of? Array
        tried_times += 1
        result = generator.call
      end
    ensure
      timer.stop
      LastTriedInfo.merge! :tried_times => tried_times, :tried_duration => timer.total_time
    end
    result
  end

end
