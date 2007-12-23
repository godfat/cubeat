
/* EventDispatcher Implement,
   blah
*/

#include "../include/EventDispatcher.hpp"
#include "../include/Input.hpp"
#include "../include/IrrDevice.hpp"
#include "../include/SpriteView.hpp"

#include <boost/foreach.hpp>
#include <algorithm>

using std::tr1::tie;
using std::tr1::get;

using irr::ITimer;

using namespace irr;
using namespace core;
using namespace scene;

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

EventDispatcher& 
EventDispatcher::subscribe_obj_event(ObjCallback ocb, Button const* btn, SpriteView* obj)
{
    obj_listeners_.push_back( tie( ocb, btn, obj ) );
    return *this;
}

void EventDispatcher::dispatch_obj(){
    //Pick
    BOOST_FOREACH(ObjEvent& o, obj_listeners_){
        ISceneManager* smgr = IrrDevice::i()->getSceneManager();
        ISceneCollisionManager* colm = smgr->getSceneCollisionManager();

        Button const* btn = get<OE::BTN>(o);
        ISceneNode* picked = colm->getSceneNodeFromScreenCoordinatesBB(position2di(
            btn->owner()->cursor().x(),
            btn->owner()->cursor().y()), 1, true);

        SpriteView* sv = get<OE::SPRITE>(o);
        if( picked == sv->body() && btn->state() == BTN_PRESS ) {
            get<OE::OBJ_CB>(o)(sv);
        }
    }
}

void EventDispatcher::dispatch()
{
    BOOST_FOREACH(BtnEvent& b, btn_listeners_) {
        Button const* btn = get<BUTTON>(b);            
        if( btn->state() != get<STATE>(b) ) continue; 
        get<BCALLBACK>(b)( btn->owner()->cursor().x(), btn->owner()->cursor().y() ); 
    }

    dispatch_obj();

    ITimer* irrTimer = IrrDevice::i()->getTimer();
    for(Timers::iterator t = timers_.begin(), tend = timers_.end(); t != tend; ++t) {
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