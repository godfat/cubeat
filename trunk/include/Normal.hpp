
/* 2007.11.9
   Nothing changed.

   Never used.
*/

#ifndef _SHOOTING_CUBE_NORMAL_HPP_
#define _SHOOTING_CUBE_NORMAL_HPP_

#include "Type.hpp"

class Normal: public Type{
public:
    virtual ~Normal(){}

private:
    #ifndef NDEBUG
    virtual void do_init(){ std::clog << "Normal init\n"; }
    virtual void do_suicide(){ std::clog << "Normal died\n"; };
    #endif
};

#endif
