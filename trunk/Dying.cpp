
/*2007.11.9 
  Nothing changed.

  These included file is going to be modified:
  1. Map.hpp
  2. Square.hpp
*/     

#include "Dying.hpp"
#include "Square.hpp"
#include "Map.hpp"

State* Dying::init2()
{
	dying_step_ = 0;
	owner()->hide_outline();
	return this;
}

State* Dying::cycle()
{
	if( dying_step_ < map().ms().decay ) {
		int alpha = (float)(map().ms().decay - dying_step_) / (float)map().ms().decay * 255.0;
		owner()->set_alpha( alpha );
		dying_step_++;
		return this;
	}
	else {
		return 0;  //really dead
	}
}
