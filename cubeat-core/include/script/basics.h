#ifndef _SHOOTING_CUBES_SCRIPT_BASICS_
#define _SHOOTING_CUBES_SCRIPT_BASICS_

typedef struct { double x, y; } v2;
typedef struct { double x, y, z; } v3;

typedef struct pSprite pSprite;

typedef void (*PSC_OBJCALLBACK)(pSprite*);
typedef void (*PSC_OBJCALLBACK_WITH_PARA)(pSprite*, int, int);

#endif //_SHOOTING_CUBES_SCRIPT_BASICS_
