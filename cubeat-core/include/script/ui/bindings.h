#ifndef _SHOOTING_CUBES_SCRIPT_UI_BINDINGS_H_
#define _SHOOTING_CUBES_SCRIPT_UI_BINDINGS_H_

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#define APIEXPORT __declspec(dllexport)
#else
#define APIEXPORT
#endif

typedef void (*PSC_OBJCALLBACK)(pSprite*);
typedef void (*PSC_OBJCALLBACK_WITH_PARA)(pSprite*, int, int);
void delegate_for_cb_from_lua(pSprite, PSC_OBJCALLBACK);
void delegate_for_cb_from_lua_with_parameter(pSprite, PSC_OBJCALLBACK_WITH_PARA, int, int);

APIEXPORT void      Sprite_set_texture(pSprite*, const char*);
APIEXPORT void      Sprite_set_pos(pSprite*, double, double);
APIEXPORT void      Sprite_set_size(pSprite*, double, double);
APIEXPORT void      Sprite_set_depth(pSprite*, double);
APIEXPORT void      Sprite_set_color(pSprite*, int, int, int);
APIEXPORT void      Sprite_set_red(pSprite*, int);
APIEXPORT void      Sprite_set_green(pSprite*, int);
APIEXPORT void      Sprite_set_blue(pSprite*, int);
APIEXPORT void      Sprite_set_alpha(pSprite*, int);
APIEXPORT void      Sprite_set_visible(pSprite*, bool);
APIEXPORT void      Sprite_set_center_aligned(pSprite*, bool);
APIEXPORT void      Sprite_on_tween_line_alpha(pSprite* self, int, double, int, PSC_OBJCALLBACK, int);
APIEXPORT void      Sprite_on_release(pSprite* self, Button const*, PSC_OBJCALLBACK);
APIEXPORT void      Sprite_on_press(pSprite* self, Button const*, PSC_OBJCALLBACK);
APIEXPORT void      Sprite_on_enter_focus(pSprite* self, Input const*, PSC_OBJCALLBACK_WITH_PARA);
APIEXPORT void      Sprite_on_leave_focus(pSprite* self, Input const*, PSC_OBJCALLBACK_WITH_PARA);

APIEXPORT void      SpriteText_set_pos(pSpriteText*, double, double);
APIEXPORT void      SpriteText_set_depth(pSpriteText*, double);
APIEXPORT void      SpriteText_set_color(pSpriteText*, int, int, int);
APIEXPORT void      SpriteText_set_red(pSpriteText*, int);
APIEXPORT void      SpriteText_set_green(pSpriteText*, int);
APIEXPORT void      SpriteText_set_blue(pSpriteText*, int);
APIEXPORT void      SpriteText_set_alpha(pSpriteText*, int);
APIEXPORT void      SpriteText_set_visible(pSpriteText*, bool);
APIEXPORT void      SpriteText_set_center_aligned(pSpriteText*, bool);
APIEXPORT void      SpriteText_on_tween_line_alpha(pSpriteText* self, int, double, int, PSC_OBJCALLBACK, int);

APIEXPORT pSprite* Sprite_create(char const*, pObject*, int, int, bool);
APIEXPORT pSpriteText*  SpriteText_create(char const*, pObject*, char const*, int, bool, int, int, int);

APIEXPORT Input*    Input_get_input1();
APIEXPORT Input*    Input_get_input2();
APIEXPORT Button const* Input_get_trig1(Input* p);
APIEXPORT Button const* Input_get_trig2(Input* p);

APIEXPORT void     Scene__gc(pScene*);
APIEXPORT void     Sprite__gc(pSprite*);
APIEXPORT void     SpriteText__gc(pSpriteText*);

#endif // _SHOOTING_CUBES_SCRIPT_UI_BINDINGS_H_