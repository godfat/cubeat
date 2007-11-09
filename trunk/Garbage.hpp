
/* 2007.11.9
   Nothing changed.

   Never used.
*/

#ifndef _SHOOTING_CUBE_GARBAGE_HPP_
#define _SHOOTING_CUBE_GARBAGE_HPP_

#include "Type.hpp"

class Garbage: public Type{
public:
    bool onClick(Square& square){
        //square.type_ -> suicide();
        //square.type_ = SF::Instance().Create<Normal>();
    }
};

#endif
