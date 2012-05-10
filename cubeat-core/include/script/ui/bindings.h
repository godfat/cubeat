#ifndef _SHOOTING_CUBES_SCRIPT_UI_BINDINGS_H_
#define _SHOOTING_CUBES_SCRIPT_UI_BINDINGS_H_

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#define APIEXPORT __declspec(dllexport)
#else
#define APIEXPORT
#endif

typedef void (*PSC_OBJCALLBACK)(pSprite*);
void delegate_for_cb_from_lua(pSprite, PSC_OBJCALLBACK);

APIEXPORT void      Sprite_set_texture(pSprite*, const char*);
APIEXPORT void      Sprite_set_pos(pSprite*, double, double);
APIEXPORT void      Sprite_set_size(pSprite*, double, double);
APIEXPORT void      Sprite_set_depth(pSprite*, double);
APIEXPORT void      Sprite_set_alpha(pSprite*, int);
APIEXPORT void      Sprite_set_visible(pSprite*, bool);
APIEXPORT void      Sprite_set_center_aligned(pSprite*, bool);
APIEXPORT void      Sprite_on_tween_line_alpha(pSprite* self, int, double, int, PSC_OBJCALLBACK, int);

APIEXPORT void      SpriteText_set_pos(pSpriteText*, double, double);
APIEXPORT void      SpriteText_set_depth(pSpriteText*, double);
APIEXPORT void      SpriteText_set_alpha(pSpriteText*, int);
APIEXPORT void      SpriteText_set_visible(pSpriteText*, bool);
APIEXPORT void      SpriteText_set_center_aligned(pSpriteText*, bool);
APIEXPORT void      SpriteText_on_tween_line_alpha(pSpriteText* self, int, double, int, PSC_OBJCALLBACK, int);

APIEXPORT pSprite* Sprite_create(char const*, pObject*, int, int, bool);
APIEXPORT pSpriteText*  SpriteText_create(char const*, pObject*, char const*, int, bool, int, int, int);

APIEXPORT void     Scene__gc(pScene*);
APIEXPORT void     Sprite__gc(pSprite*);
APIEXPORT void     SpriteText__gc(pSpriteText*);

#endif // _SHOOTING_CUBES_SCRIPT_UI_BINDINGS_H_
