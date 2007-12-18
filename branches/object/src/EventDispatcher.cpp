
/* EventDispatcher Implement,
   blah
*/

#include "../include/EventDispatcher.hpp"
#include "../include/Input.hpp"
#include "../include/IrrDevice.hpp"

#include <boost/foreach.hpp>
#include <algorithm>

using std::tr1::tie;
using std::tr1::get;

using irr::ITimer;

EventDispatcher& 
EventDispatcher::subscribe_btn_event(BtnCallback cb, Button const* btn, BSTATE state)
{
    btn_listeners_.push_back( tie( cb, btn, state ) );
    return *this;
}

EventDispatcher& 
EventDispatcher::subscribe_timer(TimerCallback cb, int duration, bool loop)
{
    irr::u32 now = IrrDevice::i()->getTimer()->getRealTime();
    timers_.push_back( tie( cb, duration, now, loop ) );
    return *this;
}

/*
EventDispatcher& 
EventDispatcher::subscribe_obj_event(InteractiveObject* obj, ObjCallback ocb)
{
    obj_listeners_.insert( make_pair( obj, ocb ) );
    return this*;
}*/

void EventDispatcher::dispatch()
{
    BOOST_FOREACH(BtnEvent& b, btn_listeners_) {
        Button const* btn = get<BUTTON>(b);            
        if( btn->state() != get<STATE>(b) ) continue; 
        get<BCALLBACK>(b)( btn->owner()->cursor().x(), btn->owner()->cursor().y() ); 
    }

    ITimer* irrTimer = IrrDevice::i()->getTimer();
    for(Timers::iterator t = timers_.begin(); t != timers_.end(); ++t) {
        irr::u32 now = irrTimer->getRealTime();
        if( now - get<LASTTIME>(*t) >= get<DURATION>(*t) ) {
            get<TCALLBACK>(*t)(); 
            get<LASTTIME>(*t) = now;
            if( get<LOOP>(*t) == false ) {
                timers_to_be_deleted.push_back(t);
            }
        }
    }

    cleanup();
}

void EventDispatcher::cleanup()
{
    BOOST_FOREACH(Timers::iterator t, timers_to_be_deleted) {
        timers_.erase(t);
    }
    timers_to_be_deleted.clear();
}