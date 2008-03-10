
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
#include <utility>

using std::tr1::tie;
using std::tr1::get;
using std::make_pair;

using irr::ITimer;

using namespace irr;
using namespace core;
using namespace scene;

using namespace psc;
using namespace ctrl;

void free_func_do_nothing_deleter(void*){ std::cerr << "EventDispatcher: deleter: do nothing.\n"; }

EventDispatcher::pvoid EventDispatcher::self_ = pvoid();

EventDispatcher::EventDispatcher()
{
}

EventDispatcher& EventDispatcher::subscribe_btn_event
    (BtnCallback const& cb, wpvoid const& obj, Button const* btn, BSTATE const& state)
{
    btn_listeners_.push_back( tie( cb, btn, state, obj ) );
    return *this;
}

EventDispatcher& EventDispatcher::subscribe_timer
    (TimerCallback const& cb, wpvoid const& obj, int const& duration, int loop)
{
    const int zero = 0;
    newly_created_timers_.push_back( tie( cb, duration, zero, loop, obj ) );
    return *this;
}

EventDispatcher& EventDispatcher::subscribe_timer
    (TimerCallback const& cb, int const& duration, int loop)
{
    if(!self_) self_ = pvoid(&EventDispatcher::i(), &free_func_do_nothing_deleter);
    return subscribe_timer(cb, self_, duration, loop);
}

EventDispatcher& EventDispatcher::subscribe_obj_event
    (ObjCallback const& ocb, view::pSprite const& obj, Button const* btn)
{   //SceneListenerPair::first = const wpScene, SceneListenerPair::second = ObjListener
    view::wpScene scene = obj->scene();
    if( scene_listeners_.find( scene ) != scene_listeners_.end() )
        scene_listeners_[ scene ].push_back( tie( ocb, btn, obj ) );
    else {
        typedef std::pair<SceneListener::iterator, bool> InsertionPair;
        InsertionPair p = scene_listeners_.insert( make_pair( scene, ObjListener() ) );
        (p.first)->second.push_back( tie(ocb, btn, obj) );
    }
    return *this;
}

//TODO:
//Event dispatching need to be extended to other button state: Release, Up, Down
void EventDispatcher::dispatch_obj()
{
    BOOST_FOREACH(SceneListenerPair& slp, scene_listeners_)
    {   //SceneListenerPair::first = const wpScene, SceneListenerPair::second = ObjListener
        view::pScene scene = slp.first.lock();
        if( !scene ) {
            scene_expired_.push_back( slp.first );
            continue;
        }
        ObjListener& listeners       = slp.second;
        ISceneCollisionManager* colm = scene->getCollisionMgr();
        std::map<Input const*, ISceneNode*> pickmap;

        BOOST_FOREACH(Input const* input, Input::getInputs()) {
            position2di pos(input->cursor().x(), input->cursor().y());
            ISceneNode* picked = colm->getSceneNodeFromScreenCoordinatesBB(pos, 1, true);
            if( picked )
                pickmap.insert( std::make_pair(input, picked) );
        }
        for(ObjListener::iterator o = listeners.begin(), oend = listeners.end(); o != oend; ++o) {
            if( view::pSprite sv = get<OE::SPRITE>(*o).lock() ) { //sprite not expired
                Button const* btn = get<OE::BTN>(*o);
                if( btn->state() != BTN_PRESS )                   //not right state
                    continue;

                if( pickmap[ btn->owner() ] == sv->body() ) {
                    get<OE::OBJ_CB>(*o)(sv);                      //lastly, fire up callback
                    std::cout << "dispatcher trace: " << pickmap[ btn->owner() ] << "\n";
                    break;
                }
            }
            else
                obj_events_to_be_deleted_.insert( std::make_pair(slp.first, o) );
        }
    }
    cleanup_obj_event();
}

void EventDispatcher::dispatch_btn(){
    for(BtnListener::iterator b = btn_listeners_.begin(), bend = btn_listeners_.end();
        b != bend; ++b) {
        if( get<BCALLEE>(*b).lock() ) {
            Button const* btn = get<BUTTON>(*b);
            if( btn->state() != get<STATE>(*b) ) continue;
            get<BCALLBACK>(*b)( btn->owner()->cursor().x(), btn->owner()->cursor().y() );
        }
        else btn_events_to_be_deleted_.push_back(b);
    }
    cleanup_btn_event();
}

void EventDispatcher::dispatch_timer(){

    cleanup_timer_and_init_newly_created_timer();

    std::time_t now = IrrDevice::i().d()->getTimer()->getTime();
    for(TimerList::iterator t = timers_.begin(), tend = timers_.end(); t != tend; ++t) {
        if( get<TCALLEE>(*t).lock() ) {
            if( now - get<LASTTIME>(*t) >= get<DURATION>(*t) ) {
                get<TCALLBACK>(*t)();
                get<LASTTIME>(*t) = now;
                int& looptimes = get<LOOP>(*t);
                if( looptimes == 0 ) {
                    timers_to_be_deleted_.push_back(t);
                } else if( looptimes > 0 ) looptimes -= 1;
            }
        }
        else timers_to_be_deleted_.push_back(t);
    }
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
    BOOST_FOREACH(TimerList::iterator t, timers_to_be_deleted_) {
        timers_.erase(t);
    }
    timers_to_be_deleted_.clear();

    // init newly created
    std::time_t init_time = IrrDevice::i().d()->getTimer()->getTime();
    BOOST_FOREACH(Timer& timer, newly_created_timers_){
        get<LASTTIME>(timer) = init_time;
    }

    timers_.insert(timers_.end(),
                   newly_created_timers_.begin(),
                   newly_created_timers_.end());
    newly_created_timers_.clear();
}

void EventDispatcher::cleanup_obj_event()
{
    BOOST_FOREACH(ObjEventRemovalPair& o, obj_events_to_be_deleted_) {
        scene_listeners_[ o.first ].erase( o.second );
    }
    obj_events_to_be_deleted_.clear();

    BOOST_FOREACH(SceneListener::key_type& s, scene_expired_) {
        scene_listeners_.erase(s);
    }
    scene_expired_.clear();
}

void EventDispatcher::cleanup_btn_event()
{
    BOOST_FOREACH(BtnListener::iterator b, btn_events_to_be_deleted_) {
        btn_listeners_.erase(b);
    }
    btn_events_to_be_deleted_.clear();
}
