
/*2007.11.9 
  Most of the things remain unchanged.
*/     

#ifndef _SHOOTING_CUBE_EXPLODING_HPP_
#define _SHOOTING_CUBE_EXPLODING_HPP_

#include "State.hpp"

class Square;

class Exploding: public State{
public:
    virtual ~Exploding(){}

	virtual State* init2();
    virtual State* cycle();

private:
    #ifndef NDEBUG
    virtual void do_init(){ std::clog << "Exploding init\n"; }
    virtual void do_suicide(){ std::clog << "Exploding died\n"; };
    #endif

    void explosion_setup(){}
    void explosion_loop(){}
    void explosion_cleanup(){}

	bool explosion_started;
	int exploding_step_;
};

#endif
