#ifndef _SHOOTING_CUBES_SCRIPT_UI_TEST_BINDINGS_
#define _SHOOTING_CUBES_SCRIPT_UI_TEST_BINDINGS_

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#define APIEXPORT __declspec(dllexport)
#else
#define APIEXPORT
#endif

typedef void (*PSC_OBJCALLBACK)(pSprite*);

APIEXPORT pScene*  TestUI_get_ui_scene(TestUI*);

APIEXPORT pSprite* Sprite_create(char const*, pScene*, int, int, bool);

APIEXPORT void     Sprite_set_texture(pSprite*, const char*);
APIEXPORT void     Sprite_set_center_aligned(pSprite*, bool);
APIEXPORT void     Sprite_move_to(pSprite*, int, int);
APIEXPORT void     Sprite_move_tween(pSprite*, int, int, int);
APIEXPORT void     Sprite_texture_flipH(pSprite*);
APIEXPORT void     Sprite_texture_flipV(pSprite*);

APIEXPORT void     Sprite_set_pos(pSprite*, double, double);
APIEXPORT void     Sprite_set_rotate(pSprite*, double, double, double);
APIEXPORT void     Sprite_set_scale(pSprite*, double, double, double);
APIEXPORT void     Sprite_set_color_diffuse(pSprite*, int);
APIEXPORT void     Sprite_set_gradient_diffuse(pSprite*, int);
APIEXPORT void     Sprite_set_gradient_emissive(pSprite*, int);
APIEXPORT void     Sprite_set_red(pSprite*, int);
APIEXPORT void     Sprite_set_green(pSprite*, int);
APIEXPORT void     Sprite_set_blue(pSprite*, int);
APIEXPORT void     Sprite_set_redE(pSprite*, int);
APIEXPORT void     Sprite_set_greenE(pSprite*, int);
APIEXPORT void     Sprite_set_blueE(pSprite*, int);
APIEXPORT void     Sprite_set_alpha(pSprite*, int);
APIEXPORT void     Sprite_set_frame(pSprite*, float);
APIEXPORT void     Sprite_set_visible(pSprite*, bool);
APIEXPORT void     Sprite_set_size(pSprite*, double, double);
APIEXPORT void     Sprite_on_release(pSprite* self, PSC_OBJCALLBACK);
APIEXPORT void     Sprite_on_press(pSprite* self, PSC_OBJCALLBACK);

APIEXPORT pSpriteText*  SpriteText_create(char const*, pScene*, char const*, int, bool, int, int, int);
APIEXPORT void          SpriteText_set_pos(pSpriteText*, double, double);

APIEXPORT void     Scene__gc(pScene*);
APIEXPORT void     Sprite__gc(pSprite*);
APIEXPORT void     SpriteText__gc(pSpriteText*);

#endif //_SHOOTING_CUBES_SCRIPT_UI_TEST_BINDINGS_

