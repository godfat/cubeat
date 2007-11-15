
/*2007.11.9 
  Nothing changed.

  These included file are going to be modified:
  1. Map.hpp
  2. Square.hpp
*/  

#include "Dropping.hpp"
#include "Square.hpp"
#include "Map.hpp" // for next_state

State* Dropping::cycle(){
    owner()->drop_a_frame();
    if( owner()->is_not_reached() ){
        return this;
    }
    else{
        owner()->addjust();
        State* state = map().next_state(owner());
        if( state == NULL )
            return this;
        else
            return state;
    }
}
