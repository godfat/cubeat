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

pHandle* Event_on_timer(char const* timer_name, PSC_CALLBACK cb, int dur, int loop) {
    pHandle* h = new pHandle(new int); //dummy for timer collecting
    EventDispatcher::i().get_timer_dispatcher(timer_name)->subscribe(cb, *h, dur, loop);
    return h;
}

void Handle__gc(pHandle* self) {
    (*self).reset(); // for safe delete.
}
