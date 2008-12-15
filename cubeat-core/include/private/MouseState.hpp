#ifndef _SHOOTING_CUBES_PRIVATE_MOUSESTATE_FOR_MANYMOUSE_
#define _SHOOTING_CUBES_PRIVATE_MOUSESTATE_FOR_MANYMOUSE_

//This is for multiple mice support, using ManyMouse library
//you must have the lib in order to compile this struct

#ifdef _USE_MANYMOUSE_

#include "manymouse/manymouse.h"
#include <string>
#include <tr1/memory>

namespace psc {
namespace ctrl {

struct MouseState
{
    typedef std::tr1::shared_ptr<MouseState> pointer_type;
    typedef std::tr1::weak_ptr<MouseState> wpointer_type;

    static pointer_type create() {
        return pointer_type(new MouseState);
    }

    int          device_id;
    bool         connected;
    std::string  name;
    int          x, y;
    unsigned int buttons;

private:
    MouseState():device_id(-1), connected(false), name(""), x(0), y(0), buttons(0){}
};

typedef MouseState::pointer_type pMouseState;
typedef MouseState::wpointer_type wpMouseState;

} //psc
} //ctrl

#endif //_USE_MANYMOUSE_
#endif //_SHOOTING_CUBES_PRIVATE_MOUSESTATE_FOR_MANYMOUSE_
