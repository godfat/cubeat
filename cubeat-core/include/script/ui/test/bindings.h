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
APIEXPORT void     Sprite_on_release(pSprite* self, InputButton const*, PSC_OBJCALLBACK);
APIEXPORT void     Sprite_on_press(pSprite* self, InputButton const*, PSC_OBJCALLBACK);
APIEXPORT void     Sprite_on_up(pSprite* self, InputButton const*, PSC_OBJCALLBACK);
APIEXPORT void     Sprite_on_down(pSprite* self, InputButton const*, PSC_OBJCALLBACK);

APIEXPORT pSpriteText*  SpriteText_create(char const*, pScene*, char const*, int, bool, int, int, int);
APIEXPORT void          SpriteText_set_center_aligned(pSpriteText*, bool);
APIEXPORT void          SpriteText_change_text(pSpriteText*, char const*);
APIEXPORT void          SpriteText_show_number(pSpriteText*, int, unsigned int);
APIEXPORT char const*   SpriteText_get_text(pSpriteText*);
APIEXPORT int           SpriteText_get_font_size(pSpriteText*);

APIEXPORT void          SpriteText_set_pos(pSpriteText*, double, double);
APIEXPORT void          SpriteText_set_rotate(pSpriteText*, double, double, double);
APIEXPORT void          SpriteText_set_scale(pSpriteText*, double, double, double);
APIEXPORT void          SpriteText_set_red(pSpriteText*, int);
APIEXPORT void          SpriteText_set_green(pSpriteText*, int);
APIEXPORT void          SpriteText_set_blue(pSpriteText*, int);
APIEXPORT void          SpriteText_set_alpha(pSpriteText*, int);
APIEXPORT void          SpriteText_set_visible(pSpriteText*, bool);
APIEXPORT void          SpriteText_on_release(pSpriteText* self, InputButton const*, PSC_OBJCALLBACK);
APIEXPORT void          SpriteText_on_press(pSpriteText* self, InputButton const*, PSC_OBJCALLBACK);

APIEXPORT Input*    Input_get_input1();
APIEXPORT Input*    Input_get_input2();
APIEXPORT InputButton const* Input_get_trig1(Input* p);
APIEXPORT InputButton const* Input_get_trig2(Input* p);
APIEXPORT InputButton const* Input_get_wep1(Input* p);
APIEXPORT InputButton const* Input_get_wep2(Input* p);
APIEXPORT InputButton const* Input_get_wep3(Input* p);
APIEXPORT InputButton const* Input_get_haste(Input* p);
APIEXPORT InputButton const* Input_get_pause(Input* p);

APIEXPORT void     Scene__gc(pScene*);
APIEXPORT void     Sprite__gc(pSprite*);
APIEXPORT void     SpriteText__gc(pSpriteText*);

#endif //_SHOOTING_CUBES_SCRIPT_UI_TEST_BINDINGS_

