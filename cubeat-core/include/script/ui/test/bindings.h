#ifndef _SHOOTING_CUBES_SCRIPT_UI_TEST_BINDINGS_
#define _SHOOTING_CUBES_SCRIPT_UI_TEST_BINDINGS_

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#define APIEXPORT __declspec(dllexport)
#else
#define APIEXPORT
#endif
typedef void (*PSC_OBJCALLBACK)(pSprite*);

typedef void (*PSC_OBJCALLBACK_WITH_PARA)(pSprite*, int, int);

APIEXPORT pScene*  TestUI_get_ui_scene(TestUI*);

APIEXPORT pSprite* Sprite_create(char const*, pScene*, int, int, bool);
APIEXPORT pSprite* Sprite_create_from_sprite(char const*, pSprite*, int, int, bool);

APIEXPORT void     Sprite_set_texture(pSprite*, const char*);
APIEXPORT void     Sprite_set_center_aligned(pSprite*, bool);
APIEXPORT void     Sprite_set_depth(pSprite*, double);
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
APIEXPORT void     Sprite_set_frame(pSprite*, double);
APIEXPORT void     Sprite_set_visible(pSprite*, bool);
APIEXPORT void     Sprite_set_size(pSprite*, double, double);

APIEXPORT int      Sprite_get_pos_x(pSprite*);
APIEXPORT int      Sprite_get_pos_y(pSprite*);
APIEXPORT int      Sprite_get_size_x(pSprite*);
APIEXPORT int      Sprite_get_size_y(pSprite*);
APIEXPORT int      Sprite_get_screen_pos_x(pSprite*);
APIEXPORT int      Sprite_get_screen_pos_y(pSprite*);

APIEXPORT void     Sprite_on_release(pSprite* self, Button const*, PSC_OBJCALLBACK);
APIEXPORT void     Sprite_on_press(pSprite* self, Button const*, PSC_OBJCALLBACK);
APIEXPORT void     Sprite_on_up(pSprite* self, Button const*, PSC_OBJCALLBACK);
APIEXPORT void     Sprite_on_down(pSprite* self, Button const*, PSC_OBJCALLBACK);
APIEXPORT void     Sprite_on_enter_focus(pSprite* self, Input const*, PSC_OBJCALLBACK_WITH_PARA);
APIEXPORT void     Sprite_on_leave_focus(pSprite* self, Input const*, PSC_OBJCALLBACK_WITH_PARA);

APIEXPORT void     Sprite_on_tween_line_pos(pSprite* self, double, double, double, int, PSC_OBJCALLBACK, int);
APIEXPORT void     Sprite_on_tween_line_rotation(pSprite* self, double, double, double, double, int, PSC_OBJCALLBACK, int);
APIEXPORT void     Sprite_on_tween_line_scale(pSprite* self, double, double, double, double, int, PSC_OBJCALLBACK, int);
APIEXPORT void     Sprite_on_tween_line_color_diffuse(pSprite* self, int, double, int, PSC_OBJCALLBACK, int);
APIEXPORT void     Sprite_on_tween_line_red(pSprite* self, int, double, int, PSC_OBJCALLBACK, int);
APIEXPORT void     Sprite_on_tween_line_green(pSprite* self, int, double, int, PSC_OBJCALLBACK, int);
APIEXPORT void     Sprite_on_tween_line_blue(pSprite* self, int, double, int, PSC_OBJCALLBACK, int);
APIEXPORT void     Sprite_on_tween_line_alpha(pSprite* self, int, double, int, PSC_OBJCALLBACK, int);

APIEXPORT pSpriteText*  SpriteText_create(char const*, pScene*, char const*, int, bool, int, int, int);
APIEXPORT pSpriteText*  SpriteText_create_from_sprite(char const*, pSprite*, char const*, int, bool, int, int, int);
APIEXPORT void          SpriteText_set_center_aligned(pSpriteText*, bool);
APIEXPORT void          SpriteText_set_depth(pSpriteText*, double);
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

APIEXPORT int           SpriteText_get_pos_x(pSpriteText*);
APIEXPORT int           SpriteText_get_pos_y(pSpriteText*);
APIEXPORT int           SpriteText_get_size_x(pSpriteText*);
APIEXPORT int           SpriteText_get_size_y(pSpriteText*);
APIEXPORT int           SpriteText_get_screen_pos_x(pSpriteText*);
APIEXPORT int           SpriteText_get_screen_pos_y(pSpriteText*);

APIEXPORT void          SpriteText_on_release(pSpriteText* self, Button const*, PSC_OBJCALLBACK);
APIEXPORT void          SpriteText_on_press(pSpriteText* self, Button const*, PSC_OBJCALLBACK);
APIEXPORT void          SpriteText_on_up(pSpriteText* self, Button const*, PSC_OBJCALLBACK);
APIEXPORT void          SpriteText_on_down(pSpriteText* self, Button const*, PSC_OBJCALLBACK);
APIEXPORT void          SpriteText_on_enter_focus(pSpriteText* self, Input const*, PSC_OBJCALLBACK_WITH_PARA);
APIEXPORT void          SpriteText_on_leave_focus(pSpriteText* self, Input const*, PSC_OBJCALLBACK_WITH_PARA);

APIEXPORT void          SpriteText_on_tween_line_pos(pSpriteText* self, double, double, double, int, PSC_OBJCALLBACK, int);
APIEXPORT void          SpriteText_on_tween_line_rotation(pSpriteText* self, double, double, double, double, int, PSC_OBJCALLBACK, int);
APIEXPORT void          SpriteText_on_tween_line_scale(pSpriteText* self, double, double, double, double, int, PSC_OBJCALLBACK, int);
APIEXPORT void          SpriteText_on_tween_line_color_diffuse(pSpriteText* self, int, double, int, PSC_OBJCALLBACK, int);
APIEXPORT void          SpriteText_on_tween_line_red(pSpriteText* self, int, double, int, PSC_OBJCALLBACK, int);
APIEXPORT void          SpriteText_on_tween_line_green(pSpriteText* self, int, double, int, PSC_OBJCALLBACK, int);
APIEXPORT void          SpriteText_on_tween_line_blue(pSpriteText* self, int, double, int, PSC_OBJCALLBACK, int);
APIEXPORT void          SpriteText_on_tween_line_alpha(pSpriteText* self, int, double, int, PSC_OBJCALLBACK, int);

APIEXPORT Input*    Input_get_input1();
APIEXPORT Input*    Input_get_input2();
APIEXPORT Button const* Input_get_trig1(Input* p);
APIEXPORT Button const* Input_get_trig2(Input* p);
APIEXPORT Button const* Input_get_wep1(Input* p);
APIEXPORT Button const* Input_get_wep2(Input* p);
APIEXPORT Button const* Input_get_wep3(Input* p);
APIEXPORT Button const* Input_get_haste(Input* p);
APIEXPORT Button const* Input_get_pause(Input* p);
APIEXPORT int       Input_get_cursor_x(Input* p);
APIEXPORT int       Input_get_cursor_y(Input* p);

APIEXPORT void     Scene__gc(pScene*);
APIEXPORT void     Sprite__gc(pSprite*);
APIEXPORT void     SpriteText__gc(pSpriteText*);

#endif //_SHOOTING_CUBES_SCRIPT_UI_TEST_BINDINGS_

