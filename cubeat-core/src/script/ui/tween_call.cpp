#include "script/ui/tween_call.h"
#include "Accessors.hpp"
#include "EasingEquations.hpp"

using namespace ctrl;
using namespace accessor;
using namespace easing;

void tween_call(pSprite* self, const char* Eq ,const char* Accessor, void* s, void* e, unsigned int duration, int loop, std::tr1::function<void()> call, int delay)
{
    if(strcmp("Linear", Eq)==0)
    {
        if(strcmp("Pos2D", Accessor)==0)
        {
            value2* start   = static_cast<value2*>(s);
            value2* end     = static_cast<value2*>(e);
            (*self)->tween<Linear, Pos2D>(vec2(start->x, start->y), vec2(end->x, end->y), duration, loop, call, delay);
        }
        else if(strcmp("Rotation", Accessor)==0)
        {
            value3* start   = static_cast<value3*>(s);
            value3* end     = static_cast<value3*>(e);
            (*self)->tween<Linear, Rotation>(vec3(start->x, start->y, start->z), vec3(end->x, end->y, end->z), duration, loop, call, delay);
        }
        else if(strcmp("Scale", Accessor)==0)
        {
            value3* start   = static_cast<value3*>(s);
            value3* end     = static_cast<value3*>(e);
            (*self)->tween<Linear, Scale>(vec3(start->x, start->y, start->z), vec3(end->x, end->y, end->z), duration, loop, call, delay);
        }
        else if(strcmp("Red", Accessor)==0)
        {
            value1* start   = static_cast<value1*>(s);
            value1* end     = static_cast<value1*>(e);
            (*self)->tween<Linear, Red>(start->x, end->x, duration, loop, call, delay);
        }
        else if(strcmp("Green", Accessor)==0)
        {
            value1* start   = static_cast<value1*>(s);
            value1* end     = static_cast<value1*>(e);
            (*self)->tween<Linear, Green>(start->x, end->x, duration, loop, call, delay);
        }
        else if(strcmp("Blue", Accessor)==0)
        {
            value1* start   = static_cast<value1*>(s);
            value1* end     = static_cast<value1*>(e);
            (*self)->tween<Linear, Blue>(start->x, end->x, duration, loop, call, delay);
        }
        else if(strcmp("Alpha", Accessor)==0)
        {
            value1* start   = static_cast<value1*>(s);
            value1* end     = static_cast<value1*>(e);
            (*self)->tween<Linear, Alpha>(start->x, end->x, duration, loop, call, delay);
        }
    }
    else if(strcmp("OElastic", Eq)==0)
    {
        if(strcmp("Pos2D", Accessor)==0)
        {
            value2* start   = static_cast<value2*>(s);
            value2* end     = static_cast<value2*>(e);
            (*self)->tween<OElastic, Pos2D>(vec2(start->x, start->y), vec2(end->x, end->y), duration, loop, call, delay);
        }
        else if(strcmp("Rotation", Accessor)==0)
        {
            value3* start   = static_cast<value3*>(s);
            value3* end     = static_cast<value3*>(e);
            (*self)->tween<OElastic, Rotation>(vec3(start->x, start->y, start->z), vec3(end->x, end->y, end->z), duration, loop, call, delay);
        }
        else if(strcmp("Scale", Accessor)==0)
        {
            value3* start   = static_cast<value3*>(s);
            value3* end     = static_cast<value3*>(e);
            (*self)->tween<OElastic, Scale>(vec3(start->x, start->y, start->z), vec3(end->x, end->y, end->z), duration, loop, call, delay);
        }
        else if(strcmp("Red", Accessor)==0)
        {
            value1* start   = static_cast<value1*>(s);
            value1* end     = static_cast<value1*>(e);
            (*self)->tween<OElastic, Red>(start->x, end->x, duration, loop, call, delay);
        }
        else if(strcmp("Green", Accessor)==0)
        {
            value1* start   = static_cast<value1*>(s);
            value1* end     = static_cast<value1*>(e);
            (*self)->tween<OElastic, Green>(start->x, end->x, duration, loop, call, delay);
        }
        else if(strcmp("Blue", Accessor)==0)
        {
            value1* start   = static_cast<value1*>(s);
            value1* end     = static_cast<value1*>(e);
            (*self)->tween<OElastic, Blue>(start->x, end->x, duration, loop, call, delay);
        }
        else if(strcmp("Alpha", Accessor)==0)
        {
            value1* start   = static_cast<value1*>(s);
            value1* end     = static_cast<value1*>(e);
            (*self)->tween<OElastic, Alpha>(start->x, end->x, duration, loop, call, delay);
        }
    }
    else if(strcmp("ISine", Eq)==0)
    {
        if(strcmp("Pos2D", Accessor)==0)
        {
            value2* start   = static_cast<value2*>(s);
            value2* end     = static_cast<value2*>(e);
            (*self)->tween<ISine, Pos2D>(vec2(start->x, start->y), vec2(end->x, end->y), duration, loop, call, delay);
        }
        else if(strcmp("Rotation", Accessor)==0)
        {
            value3* start   = static_cast<value3*>(s);
            value3* end     = static_cast<value3*>(e);
            (*self)->tween<ISine, Rotation>(vec3(start->x, start->y, start->z), vec3(end->x, end->y, end->z), duration, loop, call, delay);
        }
        else if(strcmp("Scale", Accessor)==0)
        {
            value3* start   = static_cast<value3*>(s);
            value3* end     = static_cast<value3*>(e);
            (*self)->tween<ISine, Scale>(vec3(start->x, start->y, start->z), vec3(end->x, end->y, end->z), duration, loop, call, delay);
        }
        else if(strcmp("Red", Accessor)==0)
        {
            value1* start   = static_cast<value1*>(s);
            value1* end     = static_cast<value1*>(e);
            (*self)->tween<ISine, Red>(start->x, end->x, duration, loop, call, delay);
        }
        else if(strcmp("Green", Accessor)==0)
        {
            value1* start   = static_cast<value1*>(s);
            value1* end     = static_cast<value1*>(e);
            (*self)->tween<ISine, Green>(start->x, end->x, duration, loop, call, delay);
        }
        else if(strcmp("Blue", Accessor)==0)
        {
            value1* start   = static_cast<value1*>(s);
            value1* end     = static_cast<value1*>(e);
            (*self)->tween<ISine, Blue>(start->x, end->x, duration, loop, call, delay);
        }
        else if(strcmp("Alpha", Accessor)==0)
        {
            value1* start   = static_cast<value1*>(s);
            value1* end     = static_cast<value1*>(e);
            (*self)->tween<ISine, Alpha>(start->x, end->x, duration, loop, call, delay);
        }
    }
    else if(strcmp("OSine", Eq)==0)
    {
        if(strcmp("Pos2D", Accessor)==0)
        {
            value2* start   = static_cast<value2*>(s);
            value2* end     = static_cast<value2*>(e);
            (*self)->tween<OSine, Pos2D>(vec2(start->x, start->y), vec2(end->x, end->y), duration, loop, call, delay);
        }
        else if(strcmp("Rotation", Accessor)==0)
        {
            value3* start   = static_cast<value3*>(s);
            value3* end     = static_cast<value3*>(e);
            (*self)->tween<OSine, Rotation>(vec3(start->x, start->y, start->z), vec3(end->x, end->y, end->z), duration, loop, call, delay);
        }
        else if(strcmp("Scale", Accessor)==0)
        {
            value3* start   = static_cast<value3*>(s);
            value3* end     = static_cast<value3*>(e);
            (*self)->tween<OSine, Scale>(vec3(start->x, start->y, start->z), vec3(end->x, end->y, end->z), duration, loop, call, delay);
        }
        else if(strcmp("Red", Accessor)==0)
        {
            value1* start   = static_cast<value1*>(s);
            value1* end     = static_cast<value1*>(e);
            (*self)->tween<OSine, Red>(start->x, end->x, duration, loop, call, delay);
        }
        else if(strcmp("Green", Accessor)==0)
        {
            value1* start   = static_cast<value1*>(s);
            value1* end     = static_cast<value1*>(e);
            (*self)->tween<OSine, Green>(start->x, end->x, duration, loop, call, delay);
        }
        else if(strcmp("Blue", Accessor)==0)
        {
            value1* start   = static_cast<value1*>(s);
            value1* end     = static_cast<value1*>(e);
            (*self)->tween<OSine, Blue>(start->x, end->x, duration, loop, call, delay);
        }
        else if(strcmp("Alpha", Accessor)==0)
        {
            value1* start   = static_cast<value1*>(s);
            value1* end     = static_cast<value1*>(e);
            (*self)->tween<OSine, Alpha>(start->x, end->x, duration, loop, call, delay);
        }
    }
    return;
}
