
/* EventDispatcher Implement,
   blah
*/

#include "EventDispatcher.hpp"
#include "ctrl/TimerDispatcher.hpp"
#include "Input.hpp"
#include "IrrDevice.hpp"
#include "view/Sprite.hpp"
#include "view/Scene.hpp"

#include <boost/foreach.hpp>
#include <boost/tr1/functional.hpp>
#include <algorithm>
#include <iostream>
//#include <utility>

using std::tr1::tie;
using std::tr1::get;
using std::tr1::weak_ptr;
using std::tr1::shared_ptr;
using std::tr1::bind;
using namespace std::tr1::placeholders;
using std::make_pair;

using namespace psc;
using namespace ctrl;

void free_func_do_nothing_deleter(void*){ std::cerr << "EventDispatcher: deleter: do nothing" << std::endl; }

EventDispatcher::pvoid EventDispatcher::self_ = pvoid();

EventDispatcher::EventDispatcher()
{
    global_timer_ = TimerDispatcher::create("global");
    add_timer_dispatcher(global_timer_);
    std::cout << "EventDispatcher constructed..." << std::endl;
}

EventDispatcher::~EventDispatcher()
{
    std::cout << "EventDispatcher destructing..." << std::endl;
}

void EventDispatcher::add_timer_dispatcher(pTimerDispatcher const& t)
{
    timer_dispatchers_[ t->get_name() ] = t; //converts to wpTimerDispatcher
}

pTimerDispatcher EventDispatcher::get_timer_dispatcher(std::string const& name)
{
    TimerDispatcherMap::iterator it = timer_dispatchers_.find( name );
    if( it == timer_dispatchers_.end() ) {
        printf("Warning: bad timer dispatcher, resolving to global one..\n");
        return timer_dispatchers_["global"].lock();
    }
    return it->second.lock();
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
//    for(BtnListener::iterator b = btn_listeners_.begin(), bend = btn_listeners_.end();
//        b != bend; ++b) {
//        if( get<BE::BTN>(*b) == btn && get<BE::STATE>(*b) == state ) {
//            if( cb ) {
//                get<BE::BTN_CB>(*b) = cb;      //update new callback
//                get<BE::CALLEE>(*b) = obj;     //update new callee
//            }
//            else btn_events_to_be_deleted_.push_back(b); //push empty callback to delete it.
//            return *this;
//        }
//    }
    btn_listeners_.push_back( tie( cb, btn, state, obj ) );
    return *this;
}

EventDispatcher& EventDispatcher::subscribe_btn_event
    (BtnCallback const& cb, Button const* btn, BSTATE const& state)
{
    if(!self_) self_ = pvoid(&EventDispatcher::i(), &free_func_do_nothing_deleter);
    return subscribe_btn_event(cb, self_, btn, state);
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

EventDispatcher& EventDispatcher::clear_btn_event()
{
    for(BtnListener::iterator b = btn_listeners_.begin(), bend = btn_listeners_.end();
        b != bend; ++b) {
        btn_events_to_be_deleted_.push_back(b);
    }
    cleanup_btn_event();
    return *this;
}

//note: bad way to do this... but normal way has no effect... don't know why...
EventDispatcher& EventDispatcher::clear_obj_event(view::wpScene const& scene)
{
//    ObjListener& listeners = scene_listeners_[ scene ];
//    for(ObjListener::iterator o = listeners.begin(), oend = listeners.end(); o != oend; ++o) {
//        obj_events_to_be_deleted_.insert( std::make_pair(scene, o) );
//    }
//    cleanup_obj_event();
    if( view::pScene s = scene.lock() ) {
        scene_listeners_[ s ] = ObjListener();
    }
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
        view::pSprite sv = get<FE::CALLEE>(*f).lock();      //sprite not expired
        if( sv && *get<FE::FOCUS_CB>(*f) ) {                //and callback is not NULL
            Input const* input = get<FE::INPUT>(*f);
            ObjList olist;
            if( get<FE::STATE>(*f) == FOCUS_ENTER )
                olist = focus_objs_[input];
            else if( get<FE::STATE>(*f) == FOCUS_LEAVE )
                olist = leave_focus_objs_[input];

            ObjList::iterator it = find_if(olist.begin(), olist.end(), bind(&cmp, _1, sv));
            if( it != olist.end() ) {
                (*get<FE::FOCUS_CB>(*f))(sv, input->cursor().x(), input->cursor().y());
                //std::cout << "focus event dispatched: " << sv << "\n";
            }
        }
        else focus_events_to_be_deleted_.push_back(f);
    }
}


/// This is for caching picked SceneNode for different Inputs
/// SceneNode is collected scene by scene.

void EventDispatcher::obj_picking(view::pScene const& scene)
{
    BOOST_FOREACH(Input const* input, InputMgr::i().getInputs()) {
        if( pickmap_.find(input) == pickmap_.end() )
            pickmap_.insert( make_pair(input, ObjList()) );

        ObjList const& picked = scene->pick(vec2(input->cursor().x(), input->cursor().y()));
        ObjList& pick_input = pickmap_[input];
        pick_input.insert(pick_input.end(), picked.begin(), picked.end());

        scene->update_focus_objs_by_input(input); //must have some better place to put this code.
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
                //std::cout << "dispatcher trace: " << sv << "\n";
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

/// Main Loop of Event Dispatcher

void EventDispatcher::tick_timers()
{
    //tick timers regarding their own status respectively.
    BOOST_FOREACH(TimerDispatcherPair& tdp, timer_dispatchers_) {
        pTimerDispatcher td = tdp.second.lock();
        if( !td ) {
            timer_expired_.push_back( tdp.first );
            continue;
        }
        td->tick();
    }
}

void EventDispatcher::dispatch_timer()
{
    BOOST_FOREACH(TimerDispatcherPair& tdp, timer_dispatchers_) {
        pTimerDispatcher td = tdp.second.lock();
        if( !td ) {
            timer_expired_.push_back( tdp.first );
            continue;
        }
        td->dispatch();
    }
}

void EventDispatcher::cleanup_timer_dispatcher()
{
    BOOST_FOREACH(TimerDispatcherMap::key_type& s, timer_expired_) {
        timer_dispatchers_.erase(s);
    }
    timer_expired_.clear();
}

void EventDispatcher::dispatch()
{
    //profile these...
    int t1 = 0, t2 = 0, t3 = 0, t4 = 0, t5 = 0;

    tick_timers();

    dispatch_btn();
    dispatch_obj();
    dispatch_timer();

    if( t5 - t1 > 30 ) {
        printf(" -- spike distributions: %03d %03d %03d %03d\n", t2 - t1, t3 - t2, t4 - t3, t5 - t4);
    }

    cleanup_timer_dispatcher();
}


/// Cleanup method for Event Dispatcher

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
