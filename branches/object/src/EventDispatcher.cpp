
/* EventDispatcher Implement,
   blah
*/

#include "../include/EventDispatcher.hpp"
#include "../include/Input.hpp"

using std::tr1::tie;
using std::tr1::get;

void EventDispatcher::
     subscribe_btn_event(Button const* btn, BSTATE state, BtnCallbackType cb)
{
    btn_listeners_.push_back( tie( btn, state, cb ) );
}

/*
void EventDispatcher::
     subscribe_obj_event(InteractiveObject* obj, ObjCallbackType ocb)
{
    obj_listeners_.insert( make_pair( obj, ocb ) );
}*/

void EventDispatcher::dispatch()
{
    for( BtnListener::const_iterator it = btn_listeners_.begin();
         it != btn_listeners_.end(); ++it )
    {
        Button const* btn = get<BUTTON>(*it);            
        if( btn->state() != get<STATE>(*it) ) continue; 
            
        get<BCALLBACK>(*it)( btn->owner()->cursor().x(),   
                             btn->owner()->cursor().y() ); 
    }
}
