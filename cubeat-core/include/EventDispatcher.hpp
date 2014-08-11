#ifndef _SHOOTING_CUBES_EVENT_DISPATCHER_
#define _SHOOTING_CUBES_EVENT_DISPATCHER_

#include "ButtonEnum.hpp"
#include "all_fwd.hpp"
#include "utils/Replay.hpp"

#include <boost/tr1/memory.hpp>
#include <boost/tr1/tuple.hpp>
#include <boost/tr1/functional.hpp>
#include <map>
#include <ctime>
#include <vector>
#include <string>
#include <list>

namespace psc {

namespace utils {
class Timer;
}

namespace ctrl {

//actually I am starting to doubt why Dispatchers are in ctrl namespace
//whatever.

class Input;
class Button;
class TimerDispatcher;
typedef std::tr1::shared_ptr<TimerDispatcher> pTimerDispatcher;
typedef std::tr1::weak_ptr<TimerDispatcher> wpTimerDispatcher;

class EventDispatcher
{
    typedef std::tr1::shared_ptr<void>                                     pvoid;
    typedef std::tr1::weak_ptr<void>                                       wpvoid;

    typedef std::tr1::function<void(int x, int y)>                         BtnCallback;
    typedef std::tr1::tuple<BtnCallback, Button const*, BSTATE, wpvoid>    BtnEvent;
    typedef std::list<BtnEvent>                                            BtnListener;
    typedef std::list< BtnListener::iterator >                             BtnEventRemoval;

    typedef std::tr1::function<void( view::pSprite& )>                     ObjCallback;
    typedef std::tr1::tuple<ObjCallback const*, Button const*, BSTATE, view::wpSprite> ObjEvent;
    typedef std::list<ObjEvent>                                            ObjListener;
    typedef std::pair<view::wpScene const, ObjListener::iterator>          ObjEventRemovalPair;
    typedef std::map <view::wpScene, ObjListener::iterator>                ObjEventRemoval;
    typedef std::pair<view::wpScene const, ObjListener>                    SceneListenerPair;
    typedef std::map <view::wpScene, ObjListener>                          SceneListener;
    typedef std::list<SceneListener::key_type>                             SceneListenerRemoval;

    typedef std::list<view::wpObject>                                      ObjList;
    typedef std::pair<Input const* const, ObjList>                         PickingMapPair;
    typedef std::map <Input const*, ObjList>                               PickingMap;

    typedef std::tr1::function<void(view::pSprite&, int x, int y)>         FocusCallback;
    typedef std::tr1::tuple<FocusCallback const*, Input const*, FSTATE, view::wpSprite> FocusEvent;
    typedef std::list<FocusEvent>                                          FocusListener;
    typedef std::list<FocusListener::iterator>                             FocusEventRemoval;

    typedef std::pair<std::string const, wpTimerDispatcher>                TimerDispatcherPair;
    typedef std::map <std::string, wpTimerDispatcher>                      TimerDispatcherMap;
    typedef std::list<TimerDispatcherMap::key_type>                        TimerDispatcherRemoval;

public:
    static EventDispatcher& i() {
        static EventDispatcher singleton;
        return singleton;
    }

    void             add_timer_dispatcher(pTimerDispatcher const& t);
    pTimerDispatcher get_timer_dispatcher(std::string const& name); // for those needing timer.

    /// Object binding version
    EventDispatcher&
    subscribe_btn_event(BtnCallback const&, wpvoid const&, Button const*, BSTATE const&);
    EventDispatcher&
    subscribe_obj_event(ObjCallback const*, view::pSprite const&, Button const*, BSTATE const&);
    EventDispatcher&
    subscribe_focus_event(FocusCallback const*, view::pSprite const&, Input const*, FSTATE const&);

    /// Free function version
    EventDispatcher&
    subscribe_btn_event(BtnCallback const&, Button const*, BSTATE const&);

    /// Clear all: clean up events, usually use when menu change or stage change.
    EventDispatcher& clear_btn_event();
    EventDispatcher& clear_obj_event(view::wpScene const& scene);

    void dispatch();
    void tick_timers(utils::Replay& replay);
    ~EventDispatcher();

private:
    struct BE{enum{BTN_CB, BTN, STATE, CALLEE};};
    struct FE{enum{FOCUS_CB, INPUT, STATE, CALLEE};};
    struct OE{enum{OBJ_CB, BTN, STATE, CALLEE};};
    EventDispatcher();
    void dispatch_btn();
    void dispatch_obj();
    void dispatch_focus();  //this is not an independent dispatching, it depends on dispatch_obj
    void dispatch_timer();
    void cleanup_obj_event();
    void cleanup_btn_event();
    void cleanup_timer_dispatcher();
    void obj_picking(view::pScene const&);
    void obj_listening(view::pScene const&, ObjListener&);
    void update_focus_objs();

private:
    BtnListener            btn_listeners_;
    BtnEventRemoval        btn_events_to_be_deleted_;
    SceneListener          scene_listeners_;
    ObjEventRemoval        obj_events_to_be_deleted_;
    SceneListenerRemoval   scene_expired_;
    PickingMap             pickmap_, last_pickmap_, focus_objs_, leave_focus_objs_;
    FocusListener          focus_listeners_;
    FocusEventRemoval      focus_events_to_be_deleted_;
    TimerDispatcherMap     timer_dispatchers_;
    TimerDispatcherRemoval timer_expired_;

    pTimerDispatcher       global_timer_;

    static pvoid self_; //point to &EventDispatcher::i()
};

}  //ctrl
}  //psc

#endif
