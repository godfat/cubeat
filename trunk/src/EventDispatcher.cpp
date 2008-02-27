
/* EventDispatcher Implement,
   blah
*/

#include "EventDispatcher.hpp"
#include "Input.hpp"
#include "IrrDevice.hpp"
#include "view/Sprite.hpp"
#include "view/Scene.hpp"

#include <boost/foreach.hpp>
#include <algorithm>
#include <iostream>

using std::tr1::tie;
using std::tr1::get;

using irr::ITimer;

using namespace irr;
using namespace core;
using namespace scene;

using namespace psc;
using namespace ctrl;

EventDispatcher::EventDispatcher()
{
}

EventDispatcher&
EventDispatcher::subscribe_btn_event(BtnCallback cb, Button const* btn, BSTATE state)
{
    btn_listeners_.push_back( tie( cb, btn, state ) );
    return *this;
}

EventDispatcher&
EventDispatcher::subscribe_timer(TimerCallback cb, int duration, bool loop)
{
    const int zero = 0;
    newly_created_timers_.push_back( tie( cb, duration, zero, loop ) );
    return *this;
}

EventDispatcher&
EventDispatcher::subscribe_obj_event(ObjCallback ocb, Button const* btn, view::pSprite obj)
{
    obj_listeners_.push_back( tie( ocb, btn, obj ) );
    return *this;
}

void EventDispatcher::dispatch_obj(){
    //Pick
    BOOST_FOREACH(ObjEvent& o, obj_listeners_){
        ISceneManager* smgr = IrrDevice::i().d()->getSceneManager();
        ISceneCollisionManager* colm = smgr->getSceneCollisionManager();

        Button const* btn = get<OE::BTN>(o);
        if( btn->state() != BTN_PRESS ) continue; //to improve efficiency

        view::pSprite& sv = get<OE::SPRITE>(o);

        //activate the correct scene here to make sure the picking is in the corresponding scene.
        sv->scene()->activate();

        ISceneNode* picked = colm->getSceneNodeFromScreenCoordinatesBB(position2di(
            btn->owner()->cursor().x(),
            btn->owner()->cursor().y()), 1, true);

        sv->scene()->deactivate(); //as soon as the picking is done we deactivate it.

        if( picked == sv->body() ) {
            get<OE::OBJ_CB>(o)(sv);
            std::cout << "dispatcher trace: " << picked << "\n";
            break; //add break to improve efficiency
        }
    }
    //Need to extend to other button state: Release, Up, Down
}

void EventDispatcher::dispatch_btn(){
    BOOST_FOREACH(BtnEvent& b, btn_listeners_) {
        Button const* btn = get<BUTTON>(b);
        if( btn->state() != get<STATE>(b) ) continue;
        get<BCALLBACK>(b)( btn->owner()->cursor().x(), btn->owner()->cursor().y() );
    }
}
void EventDispatcher::dispatch_timer(){
    irr::u32 now = IrrDevice::i().d()->getTimer()->getRealTime();
    for(TimerList::iterator t = timers_.begin(), tend = timers_.end(); t != tend; ++t) {
        if( now - get<LASTTIME>(*t) >= get<DURATION>(*t) ) {
            get<TCALLBACK>(*t)();
            get<LASTTIME>(*t) = now;
            if( get<LOOP>(*t) == false ) {
                timers_to_be_deleted.push_back(t);
            }
        }
    }

    cleanup_timer_and_init_newly_created_timer();
}

void EventDispatcher::dispatch()
{
    dispatch_btn();
    dispatch_obj();
    dispatch_timer();
}

void EventDispatcher::cleanup_timer_and_init_newly_created_timer()
{
    // clean up
    BOOST_FOREACH(TimerList::iterator t, timers_to_be_deleted) {
        timers_.erase(t);
    }
    timers_to_be_deleted.clear();

    // init newly created
    std::time_t init_time = std::time(0);
    BOOST_FOREACH(Timer& timer, newly_created_timers_){
        get<LASTTIME>(timer) = init_time;
    }

    timers_.insert(timers_.end(),
                   newly_created_timers_.begin(),
                   newly_created_timers_.end());
    newly_created_timers_.clear();
}
