
/* EventDispatcher Implement,
   blah
*/

#include "EventDispatcher.hpp"
#include "Input.hpp"
#include "IrrDevice.hpp"
#include "view/Sprite.hpp"
#include "view/Scene.hpp"

#include <boost/foreach.hpp>
#include <tr1/functional>
#include <algorithm>
#include <iostream>
#include <utility>

using std::tr1::tie;
using std::tr1::get;
using std::tr1::weak_ptr;
using std::tr1::shared_ptr;
using std::tr1::bind;
using namespace std::tr1::placeholders;
using std::make_pair;

using irr::ITimer;

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
    BOOST_FOREACH(BtnEvent& b, btn_listeners_)
        if( get<BE::BTN>(b) == btn && get<BE::STATE>(b) == state ) {
            get<BE::BTN_CB>(b) = cb;    //update new callback
            get<BE::CALLEE>(b) = obj;     //update new callee
            return *this;
        }
    btn_listeners_.push_back( tie( cb, btn, state, obj ) );
    return *this;
}

EventDispatcher& EventDispatcher::subscribe_btn_event
    (BtnCallback const& cb, Button const* btn, BSTATE const& state)
{
    if(!self_) self_ = pvoid(&EventDispatcher::i(), &free_func_do_nothing_deleter);
    return subscribe_btn_event(cb, self_, btn, state);
}

EventDispatcher& EventDispatcher::subscribe_timer
    (TimerCallback const& cb, wpvoid const& obj, int const& duration, int loop)
{
    int const zero = 0;
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
    (ObjCallback const* ocb, view::pSprite const& obj, Button const* btn, BSTATE const& state)
{
    view::wpScene scene = obj->scene();
    if( scene_listeners_.find( scene ) == scene_listeners_.end() )
        scene_listeners_.insert( make_pair( scene, ObjListener() ) );
    scene_listeners_[ scene ].push_back( tie(ocb, btn, state, obj) );
    return *this;
}

EventDispatcher& EventDispatcher::subscribe_focus_event
    (FocusCallback const* fcb, view::pSprite const& obj, Input const* input, FSTATE const& state)
{
    focus_listeners_.push_back( tie(fcb, input, state, obj) );
    return *this;
}


/// This is the Main Loop for Object Event Dispatching

void EventDispatcher::dispatch_obj()
{
    pickmap_.clear(); //clear (last frame's) picked object

    BOOST_FOREACH(SceneListenerPair& slp, scene_listeners_) {
        view::pScene scene = slp.first.lock();
        if( !scene ) {
            scene_expired_.push_back( slp.first );
            continue;
        }
        obj_picking(scene);
        obj_listening(scene, slp.second);
    }
    update_focus_objs();
    dispatch_focus();
    last_pickmap_ = pickmap_;

    cleanup_obj_event();
}

//void out(view::wpObject const& wpo) {
//    std::cout << wpo.lock() << " ";
//}


/// This updates the newly focused or lost focused objects
/// according to different inputs.

void EventDispatcher::update_focus_objs()
{
    leave_focus_objs_.clear();
    focus_objs_.clear();
    BOOST_FOREACH(PickingMapPair& pmp, pickmap_)
    {
        pmp.second.sort();
        ObjList const& old_list = last_pickmap_[pmp.first];
        ObjList const& new_list = pmp.second;
        ObjList::const_iterator o = old_list.begin(), oe = old_list.end();
        ObjList::const_iterator n = new_list.begin(), ne = new_list.end();
//        std::cout << "old: "; std::for_each(o, oe, out); std::cout << "\n";
//        std::cout << "new: "; std::for_each(n, ne, out); std::cout << "\n";
        ObjList leave_objs_list;
        ObjList focus_objs_list;
        while( o != oe && n != ne ) {
            if( !(*o < *n) && !(*n < *o) ) ++o, ++n;
            else if( *o < *n )
                leave_objs_list.push_back(*o), ++o;
            else if( *n < *o )
                focus_objs_list.push_back(*n), ++n;
        }
        while( o != oe ) leave_objs_list.push_back(*o), ++o;
        while( n != ne ) focus_objs_list.push_back(*n), ++n;
        leave_focus_objs_.insert( make_pair(pmp.first, leave_objs_list) );
        focus_objs_      .insert( make_pair(pmp.first, focus_objs_list) );
    }
}

inline bool cmp(view::wpObject const& t, view::pObject const& u) { return t.lock() == u; }

/// This is the focus dispatching loop,
/// it is dependent of dispatch_obj's picking.

void EventDispatcher::dispatch_focus()
{
    for(FocusListener::iterator f = focus_listeners_.begin(), fend = focus_listeners_.end();
        f != fend; ++f) {
        if( view::pSprite sv = get<FE::CALLEE>(*f).lock() ) {
            Input const* input = get<FE::INPUT>(*f);
            ObjList olist;
            if( get<FE::STATE>(*f) == FOCUS_ENTER )
                olist = focus_objs_[input];
            else if( get<FE::STATE>(*f) == FOCUS_LEAVE )
                olist = leave_focus_objs_[input];

            ObjList::iterator it = find_if(olist.begin(), olist.end(), bind(&cmp, _1, sv));
            if( it != olist.end() ) {
                (*get<FE::FOCUS_CB>(*f))(sv, input->cursor().x(), input->cursor().y());
                std::cout << "focus event dispatched: " << sv << "\n";
            }
        }
        else focus_events_to_be_deleted_.push_back(f);
    }
//    BOOST_FOREACH(PickingMapPair& pmp, leave_focus_objs_) {
//        ObjList& leave_list = pmp.second;
//        BOOST_FOREACH(view::wpObject& obj, leave_list)
//            std::cout << obj.lock() << " leaves focus of input: " << pmp.first << "\n";
//    }
//    BOOST_FOREACH(PickingMapPair& pmp, focus_objs_) {
//        ObjList& focus_list = pmp.second;
//        BOOST_FOREACH(view::wpObject& obj, focus_list)
//            std::cout << obj.lock() << " get focus by input: " << pmp.first << "\n";
//    }
}


/// This is for caching picked SceneNode for different Inputs
/// SceneNode is collected scene by scene.

void EventDispatcher::obj_picking(view::pScene const& scene)
{
    BOOST_FOREACH(Input const* input, Input::getInputs()) {
        if( pickmap_.find(input) == pickmap_.end() )
                pickmap_.insert( make_pair(input, ObjList()) );

        view::wpObject picked = scene->pick(input->cursor().x(), input->cursor().y());

        if( picked.lock() )
            pickmap_[input].push_back(picked);
    }
}

/// This is for dispatching events in a specific Scene
/// This procedure is executed per frame, and per Scene
/// Beware: now function callback is store as pointer
void EventDispatcher::obj_listening(view::pScene const& scene, ObjListener& listeners)
{
    for(ObjListener::iterator o = listeners.begin(), oend = listeners.end(); o != oend; ++o) {
        view::pSprite sv = get<OE::CALLEE>(*o).lock();      //sprite not expired
        if( sv && *get<OE::OBJ_CB>(*o) ) {                  //and callback is not NULL
            Button const* btn = get<OE::BTN>(*o);
            if( btn->state() != get<OE::STATE>(*o) )        //not right state
                continue;

            ObjList& olist = pickmap_[ btn->owner() ];
            ObjList::iterator it = find_if(olist.begin(), olist.end(), bind(&cmp, _1, sv));
            if( it != olist.end() ) {
                (*get<OE::OBJ_CB>(*o))(sv);                 //lastly, fire up callback
                std::cout << "dispatcher trace: " << sv << "\n";
            }
        }
        else obj_events_to_be_deleted_.insert( std::make_pair(scene, o) );
    }
}


/// This is the Main Loop for Global Button Events.

void EventDispatcher::dispatch_btn(){
    for(BtnListener::iterator b = btn_listeners_.begin(), bend = btn_listeners_.end();
        b != bend; ++b) {
        if( get<BE::CALLEE>(*b).lock() ) {
            Button const* btn = get<BE::BTN>(*b);
            if( btn->state() != get<BE::STATE>(*b) ) continue;
            get<BE::BTN_CB>(*b)( btn->owner()->cursor().x(), btn->owner()->cursor().y() );
        }
        else btn_events_to_be_deleted_.push_back(b);
    }
    cleanup_btn_event();
}


/// This is the Main Loop of Timer

void EventDispatcher::dispatch_timer(){

    cleanup_timer_and_init_newly_created_timer();

    std::time_t now = IrrDevice::i().d()->getTimer()->getTime();
    for(TimerList::iterator t = timers_.begin(), tend = timers_.end(); t != tend; ++t) {
        if( get<TE::CALLEE>(*t).lock() ) {
            if( now - get<TE::LASTTIME>(*t) >= get<TE::DURATION>(*t) ) {
                get<TE::TIMER_CB>(*t)();
                get<TE::LASTTIME>(*t) = now;
                int& looptimes = get<TE::LOOP>(*t);
                if( looptimes == 0 ) {
                    timers_to_be_deleted_.push_back(t);
                } else if( looptimes > 0 ) looptimes -= 1;
            }
        }
        else timers_to_be_deleted_.push_back(t);
    }
}


/// Main Loop of Event Dispatcher

void EventDispatcher::dispatch()
{
    dispatch_btn();
    dispatch_obj();
    dispatch_timer();
}


/// Cleanup method for Event Dispatcher

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
        get<TE::LASTTIME>(timer) = init_time;
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

    BOOST_FOREACH(FocusListener::iterator f, focus_events_to_be_deleted_) {
        focus_listeners_.erase(f);
    }
    focus_events_to_be_deleted_.clear();

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
