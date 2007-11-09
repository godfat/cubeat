
/*2007.11.9 
  cleaned up things which were coupled with Virtools SDK

  This file won't compile unless further changed made.
  
  These included file are going to be modified:
  1. Map.hpp
  2. Square.hpp
*/     

#include "Exploding.hpp"
#include "Square.hpp"
#include "Map.hpp"

State* Exploding::init2()
{
	exploding_step_ = 0;
	explosion_started = false;
	owner()->hide_outline();
	return this;
}

State* Exploding::cycle()
{
	if( exploding_step_ < map().ms().decay ) {
		if( !explosion_started ) {
			owner()->explosion_setup();
			explosion_started = true;
		}
		else {
			owner()->explosion_loop( exploding_step_ );
		}

		exploding_step_++;
		return this;
	}
	else {
		owner()->explosion_cleanup();
		return 0;  //really dead
	}
}
