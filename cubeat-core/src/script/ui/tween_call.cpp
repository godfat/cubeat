#include "script/ui/tween_call.h"
#include "Accessors.hpp"
#include "EasingEquations.hpp"

using namespace ctrl;
using namespace accessor;
using namespace easing;

void tween_call(pSprite* self, const char* Eq, const char* Accessor, double s, double e, unsigned int duration, int loop, std::tr1::function<void()> call, int delay)
{
    if(strcmp("Red", Accessor)==0)
    {
        if(strcmp("Linear", Eq)==0)
            (*self)->tween<Linear, Red>(s, e, duration, loop, call, delay);
        else if(strcmp("OElastic", Eq)==0)
            (*self)->tween<OElastic, Red>(s, e, duration, loop, call, delay);
        else if(strcmp("ISine", Eq)==0)
            (*self)->tween<ISine, Red>(s, e, duration, loop, call, delay);
        else if(strcmp("OSine", Eq)==0)
            (*self)->tween<OSine, Red>(s, e, duration, loop, call, delay);
    }
    else if(strcmp("Green", Accessor)==0)
    {
        if(strcmp("Linear", Eq)==0)
            (*self)->tween<Linear, Green>(s, e, duration, loop, call, delay);
        else if(strcmp("OElastic", Eq)==0)
            (*self)->tween<OElastic, Green>(s, e, duration, loop, call, delay);
        else if(strcmp("ISine", Eq)==0)
            (*self)->tween<ISine, Green>(s, e, duration, loop, call, delay);
        else if(strcmp("OSine", Eq)==0)
            (*self)->tween<OSine, Green>(s, e, duration, loop, call, delay);
    }
    else if(strcmp("Blue", Accessor)==0)
    {
        if(strcmp("Linear", Eq)==0)
            (*self)->tween<Linear, Blue>(s, e, duration, loop, call, delay);
        else if(strcmp("OElastic", Eq)==0)
            (*self)->tween<OElastic, Blue>(s, e, duration, loop, call, delay);
        else if(strcmp("ISine", Eq)==0)
            (*self)->tween<ISine, Blue>(s, e, duration, loop, call, delay);
        else if(strcmp("OSine", Eq)==0)
            (*self)->tween<OSine, Blue>(s, e, duration, loop, call, delay);
    }
    else if(strcmp("Alpha", Accessor)==0)
    {
        if(strcmp("Linear", Eq)==0)
            (*self)->tween<Linear, Alpha>(s, e, duration, loop, call, delay);
        else if(strcmp("OElastic", Eq)==0)
            (*self)->tween<OElastic, Alpha>(s, e, duration, loop, call, delay);
        else if(strcmp("ISine", Eq)==0)
            (*self)->tween<ISine, Alpha>(s, e, duration, loop, call, delay);
        else if(strcmp("OSine", Eq)==0)
            (*self)->tween<OSine, Alpha>(s, e, duration, loop, call, delay);
        else if(strcmp("SineCirc", Eq)==0)
            (*self)->tween<SineCirc, Alpha>(s, e, duration, loop, call, delay);
    }
    else if(strcmp("PosX", Accessor)==0)
    {
        if(strcmp("Linear", Eq)==0)
            (*self)->tween<Linear, PosX>(s, e, duration, loop, call, delay);
        else if(strcmp("OElastic", Eq)==0)
            (*self)->tween<OElastic, PosX>(s, e, duration, loop, call, delay);
        else if(strcmp("ISine", Eq)==0)
            (*self)->tween<ISine, PosX>(s, e, duration, loop, call, delay);
        else if(strcmp("OSine", Eq)==0)
            (*self)->tween<OSine, PosX>(s, e, duration, loop, call, delay);
        else if(strcmp("SineCirc", Eq)==0)
            (*self)->tween<SineCirc, PosX>(s, e, duration, loop, call, delay);
    }
    else if(strcmp("PosY", Accessor)==0)
    {
        if(strcmp("Linear", Eq)==0)
            (*self)->tween<Linear, PosY>(s, e, duration, loop, call, delay);
        else if(strcmp("OElastic", Eq)==0)
            (*self)->tween<OElastic, PosY>(s, e, duration, loop, call, delay);
        else if(strcmp("ISine", Eq)==0)
            (*self)->tween<ISine, PosY>(s, e, duration, loop, call, delay);
        else if(strcmp("OSine", Eq)==0)
            (*self)->tween<OSine, PosY>(s, e, duration, loop, call, delay);
        else if(strcmp("SineCirc", Eq)==0)
            (*self)->tween<SineCirc, PosY>(s, e, duration, loop, call, delay);
    }
}

void tween_call(pSprite* self, const char* Eq ,const char* Accessor, void* s, void* e, unsigned int duration, int loop, std::tr1::function<void()> call, int delay)
{
    if(strcmp("Pos2D", Accessor)==0)
    {
        value2* start   = static_cast<value2*>(s);
        value2* end     = static_cast<value2*>(e);
        if(strcmp("Linear", Eq)==0)
        {
            (*self)->tween<Linear, Pos2D>(vec2(start->x, start->y), vec2(end->x, end->y), duration, loop, call, delay);
        }
        else if(strcmp("OElastic", Eq)==0)
        {
            (*self)->tween<OElastic, Pos2D>(vec2(start->x, start->y), vec2(end->x, end->y), duration, loop, call, delay);
        }
        else if(strcmp("ISine", Eq)==0)
        {
            (*self)->tween<ISine, Pos2D>(vec2(start->x, start->y), vec2(end->x, end->y), duration, loop, call, delay);
        }
        else if(strcmp("OSine", Eq)==0)
        {
            (*self)->tween<OSine, Pos2D>(vec2(start->x, start->y), vec2(end->x, end->y), duration, loop, call, delay);
        }
        else if(strcmp("SineCirc", Eq)==0)
        {
            (*self)->tween<SineCirc, Pos2D>(vec2(start->x, start->y), vec2(end->x, end->y), duration, loop, call, delay);
        }
    }
    else if(strcmp("Rotation", Accessor)==0)
    {
        value3* start   = static_cast<value3*>(s);
        value3* end     = static_cast<value3*>(e);
        if(strcmp("Linear", Eq)==0)
        {
            (*self)->tween<Linear, Rotation>(vec3(start->x, start->y, start->z), vec3(end->x, end->y, end->z), duration, loop, call, delay);
        }
        else if(strcmp("OElastic", Eq)==0)
        {
            (*self)->tween<OElastic, Rotation>(vec3(start->x, start->y, start->z), vec3(end->x, end->y, end->z), duration, loop, call, delay);
        }
        else if(strcmp("ISine", Eq)==0)
        {
            (*self)->tween<ISine, Rotation>(vec3(start->x, start->y, start->z), vec3(end->x, end->y, end->z), duration, loop, call, delay);
        }
        else if(strcmp("OSine", Eq)==0)
        {
            (*self)->tween<OSine, Rotation>(vec3(start->x, start->y, start->z), vec3(end->x, end->y, end->z), duration, loop, call, delay);
        }
        else if(strcmp("SineCirc", Eq)==0)
        {
            (*self)->tween<SineCirc, Rotation>(vec3(start->x, start->y, start->z), vec3(end->x, end->y, end->z), duration, loop, call, delay);
        }
    }
    else if(strcmp("Scale", Accessor)==0)
    {
        value3* start   = static_cast<value3*>(s);
        value3* end     = static_cast<value3*>(e);
        if(strcmp("Linear", Eq)==0)
        {
            (*self)->tween<Linear, Scale>(vec3(start->x, start->y, start->z), vec3(end->x, end->y, end->z), duration, loop, call, delay);
        }
        else if(strcmp("OElastic", Eq)==0)
        {
            (*self)->tween<OElastic, Scale>(vec3(start->x, start->y, start->z), vec3(end->x, end->y, end->z), duration, loop, call, delay);
        }
        else if(strcmp("ISine", Eq)==0)
        {
            (*self)->tween<ISine, Scale>(vec3(start->x, start->y, start->z), vec3(end->x, end->y, end->z), duration, loop, call, delay);
        }
        else if(strcmp("OSine", Eq)==0)
        {
            (*self)->tween<OSine, Scale>(vec3(start->x, start->y, start->z), vec3(end->x, end->y, end->z), duration, loop, call, delay);
        }
        else if(strcmp("SineCirc", Eq)==0)
        {
            (*self)->tween<SineCirc, Scale>(vec3(start->x, start->y, start->z), vec3(end->x, end->y, end->z), duration, loop, call, delay);
        }
    }
}
