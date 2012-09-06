#include "view/Scene.hpp"
#include "view/Sprite.hpp"
#include "view/SpriteText.hpp"
#include "Accessors.hpp"
#include "EasingEquations.hpp"
#include "Input.hpp"
#include "Conf.hpp"
#include "script/ui/tween_call.h"

using namespace psc;
using namespace view;
using namespace ctrl;
using namespace accessor;
using namespace easing;

extern "C"{
#include "script/ui/bindings.h"
}

typedef void (*PSC_OBJCALLBACK)(pSprite*);
typedef void (*PSC_OBJCALLBACK_INT2)(pSprite*, int, int);

static void delegate_for_cb_from_lua(pSprite& sp, PSC_OBJCALLBACK func) {
    func(&sp);
}

static void delegate_for_cb_from_lua_with_parameter(pSprite& sp, PSC_OBJCALLBACK_INT2 func, int a, int b) {
    func(&sp, a, b);
}

int Get_SCREEN_W() {
    int w = Conf::i().SCREEN_W();
    return w;
}

int Get_SCREEN_H() {
    int h = Conf::i().SCREEN_H();
    return h;
}

void Sprite_set_texture(pSprite* self, const char* path) {
    (*self)->setTexture(path);
}

void Sprite_set_pos(pSprite* self, double x, double y) {
    (*self)->set<Pos2D>(vec2(x, y));
}

void Sprite_set_rotation(pSprite* self, double deg) {
    (*self)->set<Rotation>(vec3(0, 0, deg));
}

void Sprite_set_size(pSprite* self, double w, double h) {
    (*self)->set<Size2D>(vec2(w, h));
}

void Sprite_set_depth(pSprite* self, double depth) {
    (*self)->setDepth(depth);
}

void Sprite_set_color(pSprite* self, int r, int g, int b) {
    (*self)->set<Red>(r);
    (*self)->set<Green>(g);
    (*self)->set<Blue>(b);
}

void Sprite_set_red(pSprite* self, int r) {
    (*self)->set<Red>(r);
}

void Sprite_set_green(pSprite* self, int g) {
    (*self)->set<Green>(g);
}

void Sprite_set_blue(pSprite* self, int b) {
    (*self)->set<Blue>(b);
}

void Sprite_set_alpha(pSprite* self, int alpha) {
    (*self)->set<Alpha>(alpha);
}

void Sprite_set_visible(pSprite* self, bool visible) {
    (*self)->set<Visible>(visible);
}

void Sprite_set_center_aligned(pSprite* self, bool center) {
    (*self)->setCenterAligned(center);
}

void Sprite_tween(pSprite* self, const char* Eq, const char* Accessor, void* s, void* e, unsigned int duration, int loop, PSC_OBJCALLBACK cb, int delay) {
    std::tr1::function<void()> call = 0;
    if( cb ) {
        using std::tr1::ref;
        call = bind(delegate_for_cb_from_lua, ref(*self), cb);
    }
    tween_call(self, Eq, Accessor, s, e, duration, loop, call, delay);
}

void Sprite_tween_1d(pSprite* self, const char* Eq, const char* Accessor, double s, double e, unsigned int duration, int loop, PSC_OBJCALLBACK cb, int delay) {
    std::tr1::function<void()> call = 0;
    if( cb ) {
        using std::tr1::ref;
        call = bind(delegate_for_cb_from_lua, ref(*self), cb);
    }
    tween_call(self, Eq, Accessor, s, e, duration, loop, call, delay);
}

void Sprite_texture_flipH(pSprite* self) {
    (*self)->textureFlipH();
}

void Sprite_texture_flipV(pSprite* self) {
    (*self)->textureFlipV();
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

void SpriteText_change_text(pSpriteText* self, char const* text) {
    (*self)->changeText(text);
}

void SpriteText_set_pos(pSpriteText* self, double x, double y) {
    (*self)->set<Pos2D>(vec2(x, y));
}

void SpriteText_set_scale(pSpriteText* self, double uniform_scale) {
    (*self)->set<Scale>(vec3(uniform_scale, uniform_scale, uniform_scale));
}

void SpriteText_set_depth(pSpriteText* self, double depth) {
    (*self)->setDepth(depth);
}

void SpriteText_set_color(pSpriteText* self, int r, int g, int b) {
    (*self)->set<Red>(r);
    (*self)->set<Green>(g);
    (*self)->set<Blue>(b);
}

void SpriteText_set_red(pSpriteText* self, int r) {
    (*self)->set<Red>(r);
}

void SpriteText_set_green(pSpriteText* self, int g) {
    (*self)->set<Green>(g);
}

void SpriteText_set_blue(pSpriteText* self, int b) {
    (*self)->set<Blue>(b);
}

void SpriteText_set_alpha(pSpriteText* self, int alpha) {
    (*self)->set<Alpha>(alpha);
}

void SpriteText_set_visible(pSpriteText* self, bool visible) {
    (*self)->set<Visible>(visible);
    (*self)->setPickable( visible ? true : false ); // I had to add this? why?
}

void SpriteText_set_center_aligned(pSpriteText* self, bool center) {
    (*self)->setCenterAligned(center);
}

pSprite* Sprite_create(char const* name, pObject* parent, int w, int h, bool center) {
    pSprite* sp = new pSprite;
    *sp = Sprite::create(name, *parent, w, h, center);
    return sp;
}

pSpriteText* SpriteText_create(char const* text, pObject* parent, char const* f, int size, bool center, int r, int g, int b) {
    pSpriteText* sp = new pSpriteText;
    *sp = SpriteText::create(text, *parent, f, size, center, data::Color(r,g,b));
    return sp;
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

int Input_get_cursor_x(Input* p) {
    return p->cursor().x();
}

int Input_get_cursor_y(Input* p) {
    return p->cursor().y();
}

void Scene__gc(pScene* self) {
    delete self;
}

void Sprite__gc(pSprite* self) {
    delete self;
}

void SpriteText__gc(pSpriteText* self) {
    delete self;
}
