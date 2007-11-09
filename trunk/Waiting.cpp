
/* 
  2007.11.9
  Nothing changed.

  These included file are going to be modified:
  1. Map.hpp
  2. Square.hpp 
*/

#include "Waiting.hpp"
#include "Map.hpp" // for next_state, ms

State* Waiting::cycle(){
    ++delay_;
	if( !need_check_chain_property() )
	    ++owner()->lose_chain_delay();
    State* result = map().next_state(owner());
    if(result == NULL) return this;
    else return result;
}

bool Waiting::need_check() const{ return not_checked_ && delay_ >= map().ms().delay; }
bool Waiting::need_check_chain_property() const{ 
	return !chain_property_checked_ && owner()->lose_chain_delay() >= map().ms().frequency; 
}
