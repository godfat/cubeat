
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
  result = "public:\n"
  pros.each{ |p|
    # getter
    result << "    #{type} #{p}() const{ return #{p}_; }\n"
  }
  result
end

def _writer type, *pros
  result = "public:\n"
  pros.each{ |p|
    # setter
    result << "    #{$__class__}& #{p}(#{type} const& new_#{p}){ #{p}_ = new_#{p}; return *this; }\n"
  }
  result
end

def _member type, *pros
  result = "private:\n"
  result << "    #{type} #{pros.map{|p|"#{p}_"}.join(", ")};\n"
end
