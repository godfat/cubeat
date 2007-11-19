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

#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_comparison.hpp>
#include <boost/function.hpp>
#include <map>
#include <vector>

class InteractiveObject;
using boost::tuples::tuple;

typedef std::vector< InteractiveObject* >                           InterObjList;
typedef boost::function<void()>                                     CallbackType;
typedef boost::function<void(InteractiveObject* /*,Event*/)>        ObjCallbackType;
typedef boost::function<InterObjList const(int x, int y)>           PickingCallback;

typedef tuple<Button const*, BSTATE, CallbackType, PickingCallback> CBTuple;
typedef std::vector< CBTuple >                                      Listener;
typedef std::map< 
            tuple<InteractiveObject*, Button const*, BSTATE>, 
            ObjCallbackType >                                       ObjListener;

class EventDispatcher
{
public:
    static EventDispatcher& i() {
        static EventDispatcher singleton;
        return singleton;
    }

    void subscribe_btn_event(Button const*, BSTATE, CallbackType, PickingCallback);
    void subscribe_btn_event(InteractiveObject*, Button const*, BSTATE, ObjCallbackType);

    void dispatch();

private:
    EventDispatcher(){}
    EventDispatcher(EventDispatcher const&);

    Listener    listeners_;              //for global / scene event listening
    ObjListener obj_listeners_;        //for obj specific (on_object) event dispatch
};

#endif