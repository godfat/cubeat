#include "view/Scene.hpp"
#include "view/Sprite.hpp"
#include "view/SpriteText.hpp"
#include "Accessors.hpp"
#include "EasingEquations.hpp"
#include "Input.hpp"
#include "test/test_ui.hpp"

using namespace psc;
using namespace view;
using namespace ctrl;
using namespace accessor;
using namespace easing;

using std::tr1::bind;
using namespace std::tr1::placeholders;

extern "C" {
#include "script/ui/test/bindings.h"
}

pScene* TestUI_get_ui_scene(TestUI* self) {
    pScene* s = new pScene;
    *s = self->getUIScene();
    return s;
}

pSprite* Sprite_create(char const* name, pObject* parent, int w, int h, bool center) {
    pSprite* sp = new pSprite;
    *sp = Sprite::create(name, *parent, w, h, center);
    return sp;
}

//pSprite* Sprite_create_from_sprite(char const* name, pSprite* s, int w, int h, bool center) {
//    pSprite* sp = new pSprite;
//    *sp = Sprite::create(name, *s, w, h, center);
//    return sp;
//}

void Sprite_set_texture(pSprite* self, const char* path) {
    (*self)->setTexture(path);
}

void Sprite_set_center_aligned(pSprite* self, bool center) {
    (*self)->setCenterAligned(center);
}

void Sprite_set_depth(pSprite* self, double depth) {
    (*self)->setDepth(depth);
}

void Sprite_move_to(pSprite* self, int x, int y) {
    (*self)->moveTo(x, y);
}

void Sprite_move_tween(pSprite* self, int x, int y, int t) {
    (*self)->moveTween(x, y, t);
}

void Sprite_texture_flipH(pSprite* self) {
    (*self)->textureFlipH();
}

void Sprite_texture_flipV(pSprite* self) {
    (*self)->textureFlipV();
}

void Sprite_set_pos(pSprite* self, double x, double y) {
    (*self)->set<Pos2D>(vec2(x, y));
}

void Sprite_set_rotate(pSprite* self, double x, double y, double z) {
    (*self)->set<Rotation>(vec3(x, y, z));
}

void Sprite_set_scale(pSprite* self, double x, double y, double z) {
    (*self)->set<Scale>(vec3(x, y, z));
}

void Sprite_set_color_diffuse(pSprite* self, int x) {
    (*self)->set<ColorDiffuse>(x);
}

void Sprite_set_gradient_diffuse(pSprite* self, int x) {
    (*self)->set<GradientDiffuse>(x);
}

void Sprite_set_gradient_emissive(pSprite* self, int x) {
    (*self)->set<GradientEmissive>(x);
}

void Sprite_set_red(pSprite* self, int x) {
    (*self)->set<Red>(x);
}

void Sprite_set_green(pSprite* self, int x) {
    (*self)->set<Green>(x);
}

void Sprite_set_blue(pSprite* self, int x) {
    (*self)->set<Blue>(x);
}

void Sprite_set_redE(pSprite* self, int x) {
    (*self)->set<RedE>(x);
}

void Sprite_set_greenE(pSprite* self, int x) {
    (*self)->set<GreenE>(x);
}

void Sprite_set_blueE(pSprite* self, int x) {
    (*self)->set<BlueE>(x);
}

void Sprite_set_alpha(pSprite* self, int x) {
    (*self)->set<Alpha>(x);
}

void Sprite_set_frame(pSprite* self, double x) {
    (*self)->set<Frame>(x);
}

void Sprite_set_visible(pSprite* self, bool x) {
    (*self)->set<Visible>(x);
}

void Sprite_set_size(pSprite* self, double x, double y) {
    (*self)->set<Size2D>(vec2(x, y));
}

int Sprite_get_pos_x(pSprite* self) {
    return (int)((*self)->get<Pos2D>().X);
}

int Sprite_get_pos_y(pSprite* self) {
    return (int)((*self)->get<Pos2D>().Y);
}

int Sprite_get_size_x(pSprite* self) {
    return (int)((*self)->get<Size2D>().X);
}

int Sprite_get_size_y(pSprite* self) {
    return (int)((*self)->get<Size2D>().Y);
}

int Sprite_get_screen_pos_x(pSprite* self) {
    return (int)((*self)->getScreenPos().X);
}

int Sprite_get_screen_pos_y(pSprite* self) {
    return (int)((*self)->getScreenPos().Y);
}

void delegate_for_cb_from_lua(pSprite sp, PSC_OBJCALLBACK cb) {
    cb(&sp);
}

void delegate_for_cb_from_lua_with_parameter(pSprite sp, PSC_OBJCALLBACK_INT2 cb, int a, int b) {
    cb(&sp, a, b);
}

void Sprite_on_release(pSprite* self, Button const* btn, PSC_OBJCALLBACK func) {
    (*self)->onRelease( btn ) = bind(delegate_for_cb_from_lua, _1, func);
}

void Sprite_on_press(pSprite* self, Button const* btn, PSC_OBJCALLBACK func) {
    (*self)->onPress( btn ) = bind(delegate_for_cb_from_lua, _1, func);
}

void Sprite_on_up(pSprite* self, Button const* btn, PSC_OBJCALLBACK func) {
    (*self)->onUp( btn ) = bind(delegate_for_cb_from_lua, _1, func);
}

void Sprite_on_down(pSprite* self, Button const* btn, PSC_OBJCALLBACK func) {
    (*self)->onDown( btn ) = bind(delegate_for_cb_from_lua, _1, func);
}

void Sprite_on_enter_focus(pSprite* self, Input const* p, PSC_OBJCALLBACK_INT2 func) {
    (*self)->onEnterFocus( p ) = bind(delegate_for_cb_from_lua_with_parameter, _1, func, _2, _3);
}

void Sprite_on_leave_focus(pSprite* self, Input const* p, PSC_OBJCALLBACK_INT2 func) {
    (*self)->onLeaveFocus( p ) = bind(delegate_for_cb_from_lua_with_parameter, _1, func, _2, _3);
}

void Sprite_on_tween_line_pos(pSprite* self, double x, double y, double duration, int loop, PSC_OBJCALLBACK cb, int delay) {
    std::tr1::function<void()> const& call = bind(delegate_for_cb_from_lua, (*self), cb);
    (*self)->tween<Linear, Pos2D>(vec2(x, y), duration, loop, call, delay);
}

void Sprite_on_tween_line_rotation(pSprite* self, double x, double y, double z, double duration, int loop, PSC_OBJCALLBACK cb, int delay) {
    std::tr1::function<void()> const& call = bind(delegate_for_cb_from_lua, (*self), cb);
    (*self)->tween<Linear, Rotation>(vec3(x, y, z), duration, loop, call, delay);
}

void Sprite_on_tween_line_scale(pSprite* self, double x, double y, double z, double duration, int loop, PSC_OBJCALLBACK cb, int delay) {
    std::tr1::function<void()> const& call = bind(delegate_for_cb_from_lua, (*self), cb);
    (*self)->tween<Linear, Scale>(vec3(x, y, z), duration, loop, call, delay);
}

void Sprite_on_tween_line_color_diffuse(pSprite* self, int x, double duration, int loop, PSC_OBJCALLBACK cb, int delay) {
    std::tr1::function<void()> const& call = bind(delegate_for_cb_from_lua, (*self), cb);
    (*self)->tween<Linear, ColorDiffuse>(x, duration, loop, call, delay);
}

void Sprite_on_tween_line_red(pSprite* self, int r, double duration, int loop, PSC_OBJCALLBACK cb, int delay) {
    std::tr1::function<void()> const& call = bind(delegate_for_cb_from_lua, (*self), cb);
    (*self)->tween<Linear, Red>(r, duration, loop, call, delay);
}

void Sprite_on_tween_line_green(pSprite* self, int g, double duration, int loop, PSC_OBJCALLBACK cb, int delay) {
    std::tr1::function<void()> const& call = bind(delegate_for_cb_from_lua, (*self), cb);
    (*self)->tween<Linear, Green>(g, duration, loop, call, delay);
}

void Sprite_on_tween_line_blue(pSprite* self, int b, double duration, int loop, PSC_OBJCALLBACK cb, int delay) {
    std::tr1::function<void()> const& call = bind(delegate_for_cb_from_lua, (*self), cb);
    (*self)->tween<Linear, Blue>(b, duration, loop, call, delay);
}

void Sprite_on_tween_line_alpha(pSprite* self, int a, double duration, int loop, PSC_OBJCALLBACK cb, int delay) {
    std::tr1::function<void()> const& call = bind(delegate_for_cb_from_lua, (*self), cb);
    (*self)->tween<Linear, Alpha>(a, duration, loop, call, delay);
}

pSpriteText* SpriteText_create(char const* text, pObject* parent, char const* f, int size, bool center, int r, int g, int b) {
    pSpriteText* sp = new pSpriteText;
    *sp = SpriteText::create(text, *parent, f, size, center, data::Color(r,g,b));
    return sp;
}

//pSpriteText* SpriteText_create_from_sprite(char const* text, pSprite* s, char const* f, int size, bool center, int r, int g, int b) {
//    pSpriteText* sp = new pSpriteText;
//    *sp = SpriteText::create(text, *s, f, size, center, data::Color(r,g,b));
//    return sp;
//}

void SpriteText_set_center_aligned(pSpriteText* self, bool center) {
    (*self)->setCenterAligned(center);
}

void SpriteText_set_depth(pSpriteText* self, double depth) {
    (*self)->setDepth(depth);
}

void SpriteText_change_text(pSpriteText* self, char const* text) {
    (*self)->changeText(text);
}

void SpriteText_show_number(pSpriteText* self, int num, unsigned int digit) {
    (*self)->showNumber(num, digit);
}

char const* SpriteText_get_text(pSpriteText* self) {
    return (*self)->getText().c_str();
}

int SpriteText_get_font_size(pSpriteText* self) {
    return (*self)->getFontSize();
}

void SpriteText_set_pos(pSpriteText* self, double x, double y) {
    (*self)->set<Pos2D>(vec2(x, y));
}

void SpriteText_set_rotate(pSpriteText* self, double x, double y, double z) {
    (*self)->set<Rotation>(vec3(x, y, z));
}

void SpriteText_set_scale(pSpriteText* self, double x, double y, double z) {
    (*self)->set<Scale>(vec3(x, y, z));
}

void SpriteText_set_red(pSpriteText* self, int x) {
    (*self)->set<Red>(x);
}

void SpriteText_set_green(pSpriteText* self, int x) {
    (*self)->set<Green>(x);
}

void SpriteText_set_blue(pSpriteText* self, int x) {
    (*self)->set<Blue>(x);
}

void SpriteText_set_alpha(pSpriteText* self, int x) {
    (*self)->set<Alpha>(x);
}

void SpriteText_set_visible(pSpriteText* self, bool x) {
    (*self)->set<Visible>(x);
}

int SpriteText_get_pos_x(pSpriteText* self) {
    return (int)((*self)->get<Pos2D>().X);
}

int SpriteText_get_pos_y(pSpriteText* self) {
    return (int)((*self)->get<Pos2D>().Y);
}

int SpriteText_get_size_x(pSpriteText* self) {
    return (int)((*self)->get<Size2D>().X);
}

int SpriteText_get_size_y(pSpriteText* self) {
    return (int)((*self)->get<Size2D>().Y);
}

int SpriteText_get_screen_pos_x(pSpriteText* self) {
    return (int)((*self)->getScreenPos().X);
}

int SpriteText_get_screen_pos_y(pSpriteText* self) {
    return (int)((*self)->getScreenPos().Y);
}

void SpriteText_on_release(pSpriteText* self, Button const* btn, PSC_OBJCALLBACK func) {
    (*self)->onRelease( btn ) = bind(delegate_for_cb_from_lua, _1, func);
}

void SpriteText_on_press(pSpriteText* self, Button const* btn, PSC_OBJCALLBACK func) {
    (*self)->onPress( btn ) = bind(delegate_for_cb_from_lua, _1, func);
}

void SpriteText_on_up(pSpriteText* self, Button const* btn, PSC_OBJCALLBACK func) {
    (*self)->onUp( btn ) = bind(delegate_for_cb_from_lua, _1, func );
}

void SpriteText_on_down(pSpriteText* self, Button const* btn, PSC_OBJCALLBACK func) {
    (*self)->onDown( btn ) = bind(delegate_for_cb_from_lua, _1, func );
}

void SpriteText_on_enter_focus(pSpriteText* self, Input const* p, PSC_OBJCALLBACK_INT2 func) {
    (*self)->onEnterFocus( p ) = bind(delegate_for_cb_from_lua_with_parameter, _1, func, _2, _3);
}

void SpriteText_on_leave_focus(pSpriteText* self, Input const* p, PSC_OBJCALLBACK_INT2 func) {
    (*self)->onLeaveFocus( p ) = bind(delegate_for_cb_from_lua_with_parameter, _1, func, _2, _3);
}

void SpriteText_on_tween_line_pos(pSpriteText* self, double x, double y, double duration, int loop, PSC_OBJCALLBACK cb, int delay) {
    std::tr1::function<void()> const& call = bind(delegate_for_cb_from_lua, (*self), cb);
    (*self)->tween<Linear, Pos2D>(vec2(x, y), duration, loop, call, delay);
}

void SpriteText_on_tween_line_rotation(pSpriteText* self, double x, double y, double z, double duration, int loop, PSC_OBJCALLBACK cb, int delay) {
    std::tr1::function<void()> const& call = bind(delegate_for_cb_from_lua, (*self), cb);
    (*self)->tween<Linear, Rotation>(vec3(x, y, z), duration, loop, call, delay);
}

void SpriteText_on_tween_line_scale(pSpriteText* self, double x, double y, double z, double duration, int loop, PSC_OBJCALLBACK cb, int delay) {
    std::tr1::function<void()> const& call = bind(delegate_for_cb_from_lua, (*self), cb);
    (*self)->tween<Linear, Scale>(vec3(x, y, z), duration, loop, call, delay);
}

void SpriteText_on_tween_line_color_diffuse(pSpriteText* self, int x, double duration, int loop, PSC_OBJCALLBACK cb, int delay) {
    std::tr1::function<void()> const& call = bind(delegate_for_cb_from_lua, (*self), cb);
    (*self)->tween<Linear, ColorDiffuse>(x, duration, loop, call, delay);
}

void SpriteText_on_tween_line_red(pSpriteText* self, int r, double duration, int loop, PSC_OBJCALLBACK cb, int delay) {
    std::tr1::function<void()> const& call = bind(delegate_for_cb_from_lua, (*self), cb);
    (*self)->tween<Linear, Red>(r, duration, loop, call, delay);
}

void SpriteText_on_tween_line_green(pSpriteText* self, int g, double duration, int loop, PSC_OBJCALLBACK cb, int delay) {
    std::tr1::function<void()> const& call = bind(delegate_for_cb_from_lua, (*self), cb);
    (*self)->tween<Linear, Green>(g, duration, loop, call, delay);
}

void SpriteText_on_tween_line_blue(pSpriteText* self, int b, double duration, int loop, PSC_OBJCALLBACK cb, int delay){
    std::tr1::function<void()> const& call = bind(delegate_for_cb_from_lua, (*self), cb);
    (*self)->tween<Linear, Blue>(b, duration, loop, call, delay);
}

void SpriteText_on_tween_line_alpha(pSpriteText* self, int a, double duration, int loop, PSC_OBJCALLBACK cb, int delay) {
    std::tr1::function<void()> const& call = bind(delegate_for_cb_from_lua, (*self), cb);
    (*self)->tween<Linear, Alpha>(a, duration, loop, call, delay);
}

Input* Input_get_input1() {
    return InputMgr::i().getInputByIndex(0);
}

Input* Input_get_input2() {
    return InputMgr::i().getInputByIndex(1);
}

Button const* Input_get_trig1(Input* p){
    return &p->trig1();
}

Button const* Input_get_trig2(Input* p){
    return &p->trig2();
}

Button const* Input_get_wep1(Input* p) {
    return &p->wep1();
}

Button const* Input_get_wep2(Input* p) {
    return &p->wep2();
}

Button const* Input_get_wep3(Input* p) {
    return &p->wep3();
}

Button const* Input_get_haste(Input* p) {
    return &p->haste();
}

Button const* Input_get_pause(Input* p) {
    return &p->pause();
}

int Input_get_cursor_x(Input* p) {
    return p->cursor().x();
}

int Input_get_cursor_y(Input* p) {
    return p->cursor().y();
}

void Scene__gc(pScene* self) {
    printf("start delete Scene\n");
    printf("Scene: %x, Scene's parent = %x \n", (*self)->body(), (*self)->body()->getParent());
    delete self;
}

void Sprite__gc(pSprite* self) {
    printf("start delete Sprite\n");
    printf("Sprite: %x, Sprite's parent = %x \n", (*self)->body(), (*self)->body()->getParent());
    delete self;
}

void SpriteText__gc(pSpriteText* self) {
    printf("start delete SpriteText\n");
    printf("SpriteText: %x, SpriteText's parent = %x \n", (*self)->body(), (*self)->body()->getParent());
    delete self;
}
