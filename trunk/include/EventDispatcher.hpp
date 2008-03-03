#ifndef _SHOOTING_CUBES_EVENT_DISPATCHER_
#define _SHOOTING_CUBES_EVENT_DISPATCHER_

/* EventDispatcher:
   A singleton to dispatch user-defined events like on_press,
   on_release, on_blahblah.

   TODO:
   * currently none?
*/

#include "Button.hpp"
#include "all_fwd.hpp"

#include <tr1/memory>
#include <tr1/tuple>
#include <tr1/functional>
#include <map>
#include <ctime>
#include <vector>
#include <list>

namespace psc {

namespace ctrl {

class EventDispatcher
{
    typedef std::tr1::function<void(int x, int y)>                         BtnCallback;
    typedef std::tr1::tuple<BtnCallback, Button const*, BSTATE, view::wpObject> BtnEvent;
    typedef std::list<BtnEvent>                                            BtnListener;
    typedef std::list< BtnListener::iterator >                             BtnEventRemoval;

    typedef std::tr1::function<void()>                                     TimerCallback;
    typedef std::tr1::tuple<TimerCallback, std::time_t, std::time_t, bool> Timer;
    typedef std::list<Timer>                                               TimerList;
    typedef std::list< TimerList::iterator >                               TimerRemoval;

    typedef std::tr1::function<void( view::pSprite& )>                     ObjCallback;
    typedef std::tr1::tuple<ObjCallback, Button const*, view::wpSprite>    ObjEvent;
    typedef std::list<ObjEvent>                                            ObjListener;
    typedef std::pair<view::wpScene const, ObjListener::iterator>          ObjEventRemovalPair;
    typedef std::map <view::wpScene, ObjListener::iterator>                ObjEventRemoval;
    typedef std::pair<view::wpScene const, ObjListener>                    SceneListenerPair;
    typedef std::map <view::wpScene, ObjListener>                          SceneListener;
    typedef std::list<SceneListener::key_type>                             SceneListenerRemoval;

public:
    static EventDispatcher& i() {
        static EventDispatcher singleton;
        return singleton;
    }

    EventDispatcher&
    subscribe_btn_event(BtnCallback const&, Button const*, BSTATE const&, view::pObject const&);
    EventDispatcher&
    subscribe_timer(TimerCallback const&, int const&, bool loop = false);
    EventDispatcher&
    subscribe_obj_event(ObjCallback const&, Button const*, view::pSprite const&);

    void dispatch();

private:
    enum { BCALLBACK = 0, BUTTON = 1, STATE = 2, SUBSCRIBER = 3 };
    enum { TCALLBACK = 0, DURATION = 1, LASTTIME = 2, LOOP = 3 };
    enum { TIMER_TIME, TIMER };
    struct OE{enum{OBJ_CB, BTN, SPRITE};};
    EventDispatcher();
    void dispatch_btn();
    void dispatch_obj();
    void dispatch_timer();
    void cleanup_timer_and_init_newly_created_timer();
    void cleanup_obj_event();
    void cleanup_btn_event();

    BtnListener          btn_listeners_;
    BtnEventRemoval      btn_events_to_be_deleted_;
    TimerList            timers_, newly_created_timers_;
    TimerRemoval         timers_to_be_deleted_;
    SceneListener        scene_listeners_;
    ObjEventRemoval      obj_events_to_be_deleted_;
    SceneListenerRemoval scene_expired_;
};

}  //ctrl
}  //psc

#endif
