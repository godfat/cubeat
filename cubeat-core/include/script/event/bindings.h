#ifndef _SHOOTING_CUBES_SCRIPT_EVENT_BINDINGS_
#define _SHOOTING_CUBES_SCRIPT_EVENT_BINDINGS_

#include "script/basics.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#define APIEXPORT __declspec(dllexport)
#else
#define APIEXPORT
#endif

// Another binding file would be better?
APIEXPORT int      Event_get_time_of(char const*);
APIEXPORT pHandle* Event_on_timer(char const*, PSC_CALLBACK, int time, int loop);
APIEXPORT void     Handle__gc(pHandle*);

#endif //_SHOOTING_CUBES_SCRIPT_EVENT_BINDINGS_
