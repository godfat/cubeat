
#include "view/SFX.hpp"
#include "view/Sprite.hpp"
#include "view/Scene.hpp"
#include "Accessors.hpp"
#include "EasingEquations.hpp"
#include "IrrDevice.hpp"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;

using namespace psc;
using namespace view;
using namespace easing;
using namespace accessor;
using std::tr1::bind;

SFX::SFX()
{
}

SFX::~SFX()
{
}

void SFX::normal_weapon_vfx(view::pScene& s, vec2 const& pos)
{
    view::pSprite sp = view::Sprite::create("spark", s, 96, 96, true);
    sp->set<Pos2D>(pos);
    sp->tween<Linear, Alpha>(0, 200);
    sp->tween<OCirc, Scale>(vec3(0,0,0), vec3(1,1,1), 200, 0, bind(&view::Object::set<Visible>, sp, false));
}

void SFX::weapon_vfx1(view::pScene& s, vec2 const& pos)
{
    view::pSprite sp = view::Sprite::create("circle", s, 96, 96, true);
    sp->set<Pos2D>(pos).set<ColorDiffuseVec3>(vec3(0,255,255));
    sp->tween<SineCirc, Alpha>(128, 255, 400u);
    sp->tween<SineCirc, Scale>(vec3(0,0,0), 400u, 0, bind(&view::Object::set<Visible>, sp, false));
}

void SFX::weapon_vfx2(view::pScene& s, vec2 const& pos)
{
    view::pSprite sp1 = view::Sprite::create("plight", s, 96, 96, true);
    view::pSprite sp2 = view::Sprite::create("circle", s, 96, 96, true);
    sp1->set<Pos2D>(pos);
    sp2->set<Pos2D>(pos).set<Green>(100).set<Blue>(0);
    sp1->tween<IQuad, Green>(155, 300u);
    sp1->tween<IQuad, Blue>(0, 300u);
    sp1->tween<Linear, Alpha>(0, 300u);
    sp1->tween<OQuad, Scale>(vec3(0,0,0), vec3(1,1,1), 300u, 0, bind(&view::Object::set<Visible>, sp1, false));
    sp2->tween<OQuad, Green>(255, 300u);
    sp2->tween<OQuad, Blue>(150, 300u);
    sp2->tween<Linear, Alpha>(0, 255, 300u);
    sp2->tween<IQuad, Scale>(vec3(0,0,0), 300u, 0, bind(&view::Object::set<Visible>, sp2, false));
}

void SFX::weapon_vfx3(view::pScene& s, vec2 const& pos)
{
    view::pSprite sp1 = view::Sprite::create("square", s, 192, 192, true);
    view::pSprite sp2 = view::Sprite::create("square", s, 192, 192, true);
    sp1->set<Pos2D>(pos); sp2->set<Pos2D>(pos);
    sp1->tween<SineCirc, Alpha>(0, 255, 400u);
    sp1->tween<IOQuad, Scale>(vec3(0,0,0), 400u, 0, bind(&view::Object::set<Visible>, sp1, false));
    sp2->tween<SineCirc, Alpha>(0, 255, 400u);
    sp2->tween<IOQuad, Scale>(vec3(0,0,0), vec3(1,1,1), 400u, 0, bind(&view::Object::set<Visible>, sp2, false));
}


