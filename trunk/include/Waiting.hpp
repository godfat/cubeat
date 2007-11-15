
/* 2007.11.9
   Nothing changed.
*/

#ifndef _SHOOTING_CUBE_WAITING_HPP_
#define _SHOOTING_CUBE_WAITING_HPP_

#include "State.hpp"

class Waiting: public State
{
public:
    virtual ~Waiting(){}
    virtual State* cycle();

    // begin only used by Map::next_state
	void checked(){ not_checked_ = false; }
	void chain_property_checked(){ chain_property_checked_ = true; }
    bool need_check() const;
	bool need_check_chain_property() const;
    // end

	void uncheck(){ not_checked_ = true; }

private:
    virtual void do_init(){
        #ifndef NDEBUG
        std::clog << "Waiting init\n";
        #endif
        not_checked_ = true;
		chain_property_checked_ = false;
        delay_ = 0;
    }
    virtual void do_suicide(){
        #ifndef NDEBUG
        std::clog << "Waiting died\n";
        #endif
    }

private:
	bool chain_property_checked_;
    bool not_checked_;
    int delay_;
};

#endif
