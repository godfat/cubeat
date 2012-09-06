#include "view/Sprite.hpp"
#include "EventDispatcher.hpp"
#include "ctrl/TimerDispatcher.hpp"

#include <cstdio>

using namespace psc;
using namespace ctrl;

typedef std::tr1::shared_ptr<int> pHandle;

extern "C" {
#include "script/event/bindings.h"
}

int Event_get_time_of(char const* timer_name) {
    return EventDispatcher::i().get_timer_dispatcher(timer_name)->get_time();
}

void Event_on_timer(char const* timer_name, PSC_CALLBACK cb, pHandle* h, int dur, int loop) {
    EventDispatcher::i().get_timer_dispatcher(timer_name)->subscribe(cb, *h, dur, loop);
}

pHandle* Handle_create() {
    pHandle* p = new pHandle(new int);
    return p;
}

void Handle__gc(pHandle* self) {
    (*self).reset(); // for safe delete.
}
