
/*2007.11.9 
  Nothing changed.

  These included file are going to be modified:
  1. Map.hpp
  2. Square.hpp
*/  

#include "State.hpp"
#include "Square.hpp"

class Map;

Map& State::map(){ assert(inited_); return owner_->map(); }
Map const& State::map() const{ assert(inited_); return owner_->map(); }
