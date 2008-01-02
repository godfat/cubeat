
def header_guard
  defined = "_SHOOTING_CUBES_#{@class.upcase}_#{rand.to_s[2..-1]}_"
"#ifndef #{defined}
#define #{defined}"
end
