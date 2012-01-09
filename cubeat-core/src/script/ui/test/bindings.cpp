#include "view/Scene.hpp"
#include "view/Sprite.hpp"
#include "view/SpriteText.hpp"
#include "Accessors.hpp"
#include "Input.hpp"
#include "test/test_ui.hpp"

using namespace psc;
using namespace view;
using namespace ctrl;
using namespace accessor;

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

pSprite* Sprite_create(char const* name, pScene* s, int w, int h, bool center) {
    pSprite* sp = new pSprite;
    *sp = Sprite::create(name, *s, w, h, center);
    return sp;
}

void Sprite_set_texture(pSprite* self, const char* path) {
    (*self)->setTexture(path);
}

void Sprite_set_center_aligned(pSprite* self, bool center) {
    (*self)->setCenterAligned(center);
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

void Sprite_set_frame(pSprite* self, float x) {
    (*self)->set<Frame>(x);
}

void Sprite_set_visible(pSprite* self, bool x) {
    (*self)->set<Visible>(x);
}

void Sprite_set_size(pSprite* self, double x, double y) {
    (*self)->set<Size2D>(vec2(x, y));
}

void delegate_for_cb_from_lua(pSprite sp, PSC_OBJCALLBACK cb) {
    cb(&sp);
}

void Sprite_on_release(pSprite* self, PSC_OBJCALLBACK func) {
    (*self)->onRelease( &InputMgr::i().getInputByIndex(0)->trig1() ) =
        bind(delegate_for_cb_from_lua, _1, func);
}

void Sprite_on_press(pSprite* self, PSC_OBJCALLBACK func) {
    (*self)->onPress( &InputMgr::i().getInputByIndex(0)->trig1() ) =
        bind(delegate_for_cb_from_lua, _1, func);
}

pSpriteText* SpriteText_create(char const* text, pScene* s, char const* f, int size, bool center, int r, int g, int b) {
    pSpriteText* sp = new pSpriteText;
    *sp = SpriteText::create(text, *s, f, size, center, data::Color(r,g,b));
    return sp;
}

void SpriteText_set_center_aligned(pSpriteText* self, bool center) {
    (*self)->setCenterAligned(center);
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

InputButton const* Input_get_trig1(){
    return &InputMgr::i().getInputByIndex(0)->trig1();
}

InputButton const* Input_get_trig2(){
    return &InputMgr::i().getInputByIndex(0)->trig2();
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
