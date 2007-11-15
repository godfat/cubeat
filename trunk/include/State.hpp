
/*2007.11.9 
  Nothing changed.
*/  

#ifndef _SHOOTING_CUBE_STATE_HPP_
#define _SHOOTING_CUBE_STATE_HPP_

#include "PooledObject.hpp"

#ifndef NDEBUG
#include <iostream>
#endif

#include <cassert>

class Map;
class Square;

class State: public PooledObject{
public:
    State(): inited_(false)
    {}
	
	State* init(Square* const owner){ owner_ = owner; inited_ = true; return this; }

	virtual State* init2() { return this; }

	virtual ~State(){}
    virtual State* cycle() = 0;

protected:
    Map& map();
    Map const& map() const;
    Square* owner(){ assert(inited_); return owner_; }
    Square const* owner() const{ assert(inited_); return owner_; }

private:
    virtual void do_init(){
        #ifndef NDEBUG
        std::clog << "State init\n";
        #endif
    }
    virtual void do_suicide(){
        #ifndef NDEBUG
        std::clog << "State died\n";
        #endif
        inited_ = false;
    };

private:
    Square* owner_;
    bool inited_;
};

#endif
