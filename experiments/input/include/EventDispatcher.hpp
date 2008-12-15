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

//#include <boost/tuple/tuple.hpp>
//#include <boost/tuple/tuple_comparison.hpp>
#include <boost/tr1/tuple.hpp>
#include <boost/function.hpp>
#include <map>
#include <vector>

class InteractiveObject;
using std::tr1::tuple;
using std::tr1::tuple_element;

typedef boost::function<void(int x, int y)>                  BtnCallbackType;
typedef boost::function<void(InteractiveObject* /*,Event*/)> ObjCallbackType;

//typedef tuple<InteractiveObject*, ObjCallbackType>           ObjEvent;
typedef tuple<Button const*, BSTATE, BtnCallbackType>        BtnEvent;

typedef std::vector< BtnEvent >                              BtnListener;

//typedef std::map< tuple_element<0, ObjEvent>::type, 
//                  tuple_element<1, ObjEvent>::type >         ObjListener;

class EventDispatcher
{
public:
    static EventDispatcher& i() {
        static EventDispatcher singleton;
        return singleton;
    }

    void subscribe_btn_event(Button const*, BSTATE, BtnCallbackType);
    void subscribe_obj_event(InteractiveObject*, ObjCallbackType);

    void dispatch();

private:
    enum blah { BUTTON = 0, STATE = 1, BCALLBACK = 2 };
    EventDispatcher(){}
    EventDispatcher(EventDispatcher const&);

    BtnListener btn_listeners_;       
//    ObjListener obj_listeners_;       
};

#endif