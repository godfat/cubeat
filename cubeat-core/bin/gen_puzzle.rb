#!/usr/bin/env ruby

require 'rubygems'
require 'puzzle_generator'

opts = {}
ARGV.map{ |arg|
  puts(arg.tr('=', ':')+',')
  arg.split '=' }.each{ |k, v| opts[k.to_sym] = v.to_i }

puts 'cube_colors:'
p PuzzleGenerator::Puzzle.new(:level => opts[:level], :timeout => 7200).generate.transpose.reverse

# puts \
# 'color_amounts: 4,
# chain_amounts: 3,
# starting_line: 4,
# chaining_duration: 500,
# cube_dying_duration: 250,
# cube_sinking_duration: 500,
# cube_dropping_duration: 250,
# speed: 0.75,
# damage_factor: 1.0,
# width: 6,
# height: 10,
# dropping_creatable: 0,
# garbage_dumpable: 0,
# cube_colors:'
#
# p PuzzleGenerator::Puzzle.new(:timeout => 7200, :level => ARGV.first.to_i).generate.transpose.reverse
