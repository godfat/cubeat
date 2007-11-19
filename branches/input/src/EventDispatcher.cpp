
/* EventDispatcher Implement,
   blah
*/

#include "../include/InteractiveObject.hpp"
#include "../include/EventDispatcher.hpp"
#include "../include/Input.hpp"

using boost::tuples::tie;

void EventDispatcher::
     subscribe_btn_event(Button const* btn, BSTATE state, CallbackType cb, PickingCallback pcb)
{
    listeners_.push_back( tie( btn, state, cb, pcb ) );
}

void EventDispatcher::
     subscribe_btn_event(InteractiveObject* obj, Button const* btn, BSTATE state, ObjCallbackType ocb)
{
    obj_listeners_.insert( make_pair( tie( obj, btn, state ), ocb ) );
}

void EventDispatcher::dispatch()
{
    for( Listener::const_iterator it = listeners_.begin();
         it != listeners_.end(); ++it )
    {
        Button const* btn = it->get<0>();
        if( btn->state() != it->get<1>() ) continue;
            
        it->get<2>()();

        if( it->get<3>() == NULL ) continue;

        InterObjList const& list = it->get<3>()( btn->owner()->cursor().x(), 
                                                 btn->owner()->cursor().y() );

        for( InterObjList::const_iterator it2 = list.begin();
             it2 != list.end() ; ++it2 )
        {
            ObjCallbackType cb =
                obj_listeners_[ tie(*it2, it->get<0>(), it->get<1>()) ];

            if( cb == NULL ) continue;

            cb( *it2 );   //anything else?
        }
    }
}
