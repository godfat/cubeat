#ifndef _SHOOTING_CUBES_EVENT_DISPATCHER_
#define _SHOOTING_CUBES_EVENT_DISPATCHER_

/* EventDispatcher:
   A singleton to dispatch user-defined events like on_press,
   on_release, on_blahblah.

   TODO:
   * un-subscribe events
   * and some other things.
*/

#include "Button.hpp"

#include <boost/tr1/tuple.hpp>
#include <boost/function.hpp>
#include <map>
#include <ctime>
#include <vector>
#include <list>

using std::tr1::tuple;
using std::tr1::tuple_element;

//typedef boost::function<void(InteractiveObject* /*,Event*/)> ObjCallback;
//typedef tuple<InteractiveObject*, ObjCallbackType>           ObjEvent;
//typedef std::map< tuple_element<0, ObjEvent>::type, 
//                  tuple_element<1, ObjEvent>::type >         ObjListener;

class EventDispatcher
{
    typedef boost::function<void(int x, int y)>            BtnCallback;
    typedef boost::function<void()>                        TimerCallback;
    typedef tuple<BtnCallback, Button const*, BSTATE>      BtnEvent;
    typedef tuple<TimerCallback, std::time_t, std::time_t, bool> Timer;
    typedef std::vector< BtnEvent >                        BtnListener;
    typedef std::list< Timer >                             Timers;
    typedef std::vector< Timers::iterator >                TimerRemoval;

public:
    static EventDispatcher& i() {
        static EventDispatcher singleton;
        return singleton;
    }

    EventDispatcher& subscribe_btn_event(BtnCallback, Button const*, BSTATE);
    EventDispatcher& subscribe_timer(TimerCallback, int, bool loop = false);
//  EventDispatcher& subscribe_obj_event(InteractiveObject*, ObjCallback);

    void dispatch();

private:
    enum blah { BCALLBACK = 0, BUTTON = 1, STATE = 2 };
    enum bleh { TCALLBACK = 0, DURATION = 1, LASTTIME = 2, LOOP = 3 };
    EventDispatcher(){}
    EventDispatcher(EventDispatcher const&);

    void cleanup();

    BtnListener  btn_listeners_;   
    Timers       timers_;
    TimerRemoval timers_to_be_deleted;
//  ObjListener obj_listeners_;       
};

#endif