#include "view/Scene.hpp"
#include "view/Sprite.hpp"
#include "view/SpriteText.hpp"
#include "Accessors.hpp"
#include "EasingEquations.hpp"
#include "Input.hpp"

using namespace psc;
using namespace view;
using namespace ctrl;
using namespace accessor;
using namespace easing;

extern "C"{
#include "script/ui/bindings.h"
}

void delegate_for_cb_from_lua(pSprite sp, PSC_OBJCALLBACK func) {
    func(&sp);
}

void Sprite_set_texture(pSprite* self, const char* path) {
    (*self)->setTexture(path);
}

void Sprite_set_pos(pSprite* self, double x, double y) {
    (*self)->set<Pos2D>(vec2(x, y));
}

void Sprite_set_size(pSprite* self, double w, double h) {
    (*self)->set<Size2D>(vec2(w, h));
}

void Sprite_set_depth(pSprite* self, double depth) {
    (*self)->setDepth(depth);
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

void Sprite_on_tween_line_alpha(pSprite* self, int alpha, double duration, int loop, PSC_OBJCALLBACK cb, int delay) {
    std::tr1::function<void()> const& call = bind(delegate_for_cb_from_lua, (*self), cb);
    (*self)->tween<Linear, Alpha>(alpha, duration, loop, call, delay);
}

void Sprite_on_release(pSprite* self, Button const* btn, PSC_OBJCALLBACK func) {
    (*self)->onRelease( btn ) = bind(delegate_for_cb_from_lua, _1, func);
}

void Sprite_on_press(pSprite* self, Button const* btn, PSC_OBJCALLBACK func) {
    (*self)->onPress( btn ) = bind(delegate_for_cb_from_lua, _1, func);
}

void SpriteText_set_pos(pSpriteText* self, double x, double y) {
    (*self)->set<Pos2D>(vec2(x, y));
}

void SpriteText_set_depth(pSpriteText* self, double depth) {
    (*self)->setDepth(depth);
}

void SpriteText_set_alpha(pSpriteText* self, int alpha) {
    (*self)->set<Alpha>(alpha);
}

void SpriteText_set_visible(pSpriteText* self, bool visible) {
    (*self)->set<Visible>(visible);
}

void SpriteText_set_center_aligned(pSpriteText* self, bool center) {
    (*self)->setCenterAligned(center);
}

void SpriteText_on_tween_line_alpha(pSpriteText* self, int alpha, double duration, int loop, PSC_OBJCALLBACK cb, int delay) {
    std::tr1::function<void()> const& call = bind(delegate_for_cb_from_lua, (*self), cb);
    (*self)->tween<Linear, Alpha>(alpha, duration, loop, call, delay);
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

void Scene__gc(pScene* self) {
    delete self;
}

void Sprite__gc(pSprite* self) {
    delete self;
}

void SpriteText__gc(pSpriteText* self) {
    delete self;
}
