
#ifndef _SHOOTING_CUBES_INTERACTIVE_OBJECT_
#define _SHOOTING_CUBES_INTERACTIVE_OBJECT_

#include "EventDispatcher.hpp"
#include "Button.hpp"

#include <iostream>

class InteractiveObject
{
public:
    InteractiveObject():no_(1) {
        /* nothing for now */
    }

    void hit_reaction(/* something */) {
        std::cout << "I am No." << no_ << ", HIT!\n";
    }
/*
    void set_something_callback(ObjCallbackType ocb) {
        EventDispatcher::i().subscribe_obj_event(this, ocb);
    }*/

private:
    int no_;
};

#endif