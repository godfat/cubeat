#ifndef _SHOOTING_CUBES_SCRIPT_UI_BINDINGS_H_
#define _SHOOTING_CUBES_SCRIPT_UI_BINDINGS_H_

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#define APIEXPORT __declspec(dllexport)
#else
#define APIEXPORT
#endif

APIEXPORT void     Sprite_set_texture(pSprite*, const char*);
APIEXPORT void     Sprite_set_pos(pSprite*, double, double);
APIEXPORT void     Sprite_set_size(pSprite*, double, double);
APIEXPORT void     Sprite_set_depth(pSprite*, double);
APIEXPORT void     Sprite_set_alpha(pSprite*, int);
APIEXPORT void     Sprite_set_visible(pSprite*, bool);
APIEXPORT void     Sprite_set_center_aligned(pSprite*, bool);

APIEXPORT pSprite* Sprite_create(char const*, pObject*, int, int, bool);

APIEXPORT void     Scene__gc(pScene*);
APIEXPORT void     Sprite__gc(pSprite*);

#endif // _SHOOTING_CUBES_SCRIPT_UI_BINDINGS_H_
