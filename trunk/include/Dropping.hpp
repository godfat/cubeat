
/*2007.11.9 
  Nothing changed.
*/     

#ifndef _SHOOTING_CUBE_DROPPING_HPP_
#define _SHOOTING_CUBE_DROPPING_HPP_

#include "State.hpp"

class Square;

class Dropping: public State
{
public:
    virtual ~Dropping(){}
    virtual State* cycle();

private:
    #ifndef NDEBUG
    virtual void do_init(){ std::clog << "State init\n"; }
    virtual void do_suicide(){ std::clog << "State died\n"; }
    #endif
};

#endif
