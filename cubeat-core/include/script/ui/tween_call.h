#ifndef _TWEEN_CALL_H_
#define _TWEEN_CALL_H_

#include <boost/tr1/functional.hpp>
#include "view/Sprite.hpp"

using namespace psc;
using namespace view;

typedef struct { int x; }           value1;
typedef struct { double x, y; }     value2;
typedef struct { double x, y, z; }  value3;

void tween_call(pSprite*, const char*, const char*, double, double, unsigned int, int, std::tr1::function<void()>, int);
void tween_call(pSprite*, const char*, const char*, void*, void*, unsigned int, int, std::tr1::function<void()>, int);

#endif
