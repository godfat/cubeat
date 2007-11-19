
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
    void interact_with(Button const* btn, BSTATE state) {
        ObjCallbackType ocb = &InteractiveObject::hit_reaction;
        set_btn_callback(btn, state, ocb);
    }

private:
    void set_btn_callback(Button const* btn, BSTATE state, ObjCallbackType ocb) {
        EventDispatcher::i().subscribe_btn_event(this, btn, state, ocb);
    }
    void hit_reaction(/* something */) {
        std::cout << "I am No." << no_ << ", HIT!\n";
    }

    int no_;
};

#endif