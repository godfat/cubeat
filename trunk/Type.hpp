
/* 2007.11.9
   Nothing changed.

   Never used.
*/

#ifndef _SHOOTING_CUBE_TYPE_HPP_
#define _SHOOTING_CUBE_TYPE_HPP_

#include "PooledObject.hpp"

class Type: public PooledObject{
private:
    #ifndef NDEBUG
    virtual void do_init(){ std::clog << "Type init\n"; }
    virtual void do_suicide(){ std::clog << "Type died\n"; };
    #else
    virtual void do_init(){}
    virtual void do_suicide(){};
    #endif
};

#endif
