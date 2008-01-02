
def accessor type, *pros
  (_reader type, *pros) +
  (_writer type, *pros) +
  (_member type, *pros)
end

def reader type, *pros
  (_reader type, *pros) +
  (_member type, *pros)
end

def writer type, *pros
  (_writer type, *pros) +
  (_member type, *pros)
end

def _reader type, *pros
  result = "#{@prefix}public:\n"
  pros.each{ |p|
    # getter
    result << "#{@prefix}#{@indent}#{type} #{p}() const{ return #{p}_; }\n"
  }
  result
end

def _writer type, *pros
  result = "#{@prefix}public:\n"
  pros.each{ |p|
    # setter
    result << "#{@prefix}#{@indent}#{@class}& #{p}(#{type} const& new_#{p}){ #{p}_ = new_#{p}; return *this; }\n"
  }
  result
end

def _member type, *pros
  result = "#{@prefix}private:\n"
  result << "#{@prefix}#{@indent}#{type} #{pros.map{|p|"#{p}_"}.join(", ")};\n"
end
