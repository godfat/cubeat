
/*2007.11.9 
  Nothing changed.
*/     

#ifndef _SHOOTING_CUBE_DYING_HPP_
#define _SHOOTING_CUBE_DYING_HPP_

#include "State.hpp"

class Square;

class Dying: public State{
public:
    virtual ~Dying(){}

	virtual State* init2();
    virtual State* cycle();

private:
    #ifndef NDEBUG
    virtual void do_init(){ std::clog << "Dying init\n"; }
    virtual void do_suicide(){ std::clog << "Dying died\n"; };
    #endif

	int dying_step_;
};

#endif
