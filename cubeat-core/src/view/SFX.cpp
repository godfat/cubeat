
#include "view/SFX.hpp"
#include "view/Sprite.hpp"
#include "view/Scene.hpp"
#include "Accessors.hpp"
#include "EasingEquations.hpp"
#include "IrrDevice.hpp"

#include <boost/foreach.hpp>

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

void SFX::init_textures(pScene& s)
{
    //pre-load textures, so this won't clash with the threading thingy.
    Sprite::create("spark", s, 96, 96, true);
    Sprite::create("circle", s, 96, 96, true);
    Sprite::create("plight", s, 96, 96, true);
    Sprite::create("square", s, 192, 192, true);
    Sprite::create("cubes/cubes1", s, 64, 64, true);
    Sprite::create("cubes/cubes2", s, 64, 64, true);
    Sprite::create("cubes/cubes3", s, 64, 64, true);
    Sprite::create("cubes/cubes4", s, 64, 64, true);
}

void SFX::clear_obj(FXObjList::iterator obj)
{
    effects_to_be_deleted_.push_back(obj);
}

void SFX::cleanup()
{
    BOOST_FOREACH(FXObjList::iterator o, effects_to_be_deleted_) {
        effects_holder_.erase(o);
    }
    effects_to_be_deleted_.clear();
}

void SFX::normal_weapon_vfx(pScene& s, vec2 const& pos)
{
    pSprite sp = Sprite::create("spark", s, 96, 96, true);
    sp->set<Pos2D>(pos);
    sp->tween<Linear, Alpha>(0, 500u);

    effects_holder_.push_front(sp);
    sp->tween<OCirc, Scale>(vec3(0,0,0), vec3(1,1,1), 500u, 0, bind(&SFX::clear_obj, this, effects_holder_.begin()));
}

void SFX::weapon_vfx1(pScene& s, vec2 const& pos)
{
    pSprite sp = Sprite::create("circle", s, 96, 96, true);
    sp->set<Pos2D>(pos).set<ColorDiffuseVec3>(vec3(0,255,255));
    sp->tween<SineCirc, Alpha>(128, 255, 400u);

    effects_holder_.push_front(sp);
    sp->tween<SineCirc, Scale>(vec3(0,0,0), 400u, 0, bind(&SFX::clear_obj, this, effects_holder_.begin()));
}

void SFX::weapon_vfx2(pScene& s, vec2 const& pos)
{
    pSprite sp1 = Sprite::create("plight", s, 96, 96, true);
    pSprite sp2 = Sprite::create("circle", s, 96, 96, true);
    sp1->set<Pos2D>(pos);
    sp2->set<Pos2D>(pos).set<Green>(100).set<Blue>(0);
    sp1->tween<IQuad, Green>(155, 300u);
    sp1->tween<IQuad, Blue>(0, 300u);
    sp1->tween<Linear, Alpha>(0, 300u);

    effects_holder_.push_front(sp1);
    sp1->tween<OQuad, Scale>(vec3(0,0,0), vec3(1,1,1), 300u, 0, bind(&SFX::clear_obj, this, effects_holder_.begin()));

    sp2->tween<OQuad, Green>(255, 300u);
    sp2->tween<OQuad, Blue>(150, 300u);
    sp2->tween<Linear, Alpha>(0, 255, 300u);

    effects_holder_.push_front(sp2);
    sp2->tween<IQuad, Scale>(vec3(0,0,0), 300u, 0, bind(&SFX::clear_obj, this, effects_holder_.begin()));
}

void SFX::weapon_vfx3(pScene& s, vec2 const& pos)
{
    pSprite sp1 = Sprite::create("square", s, 192, 192, true);
    pSprite sp2 = Sprite::create("square", s, 192, 192, true);
    sp1->set<Pos2D>(pos); sp2->set<Pos2D>(pos);
    sp1->tween<SineCirc, Alpha>(0, 255, 400u);

    effects_holder_.push_front(sp1);
    sp1->tween<IOQuad, Scale>(vec3(0,0,0), 400u, 0, bind(&SFX::clear_obj, this, effects_holder_.begin()));

    sp2->tween<SineCirc, Alpha>(0, 255, 400u);

    effects_holder_.push_front(sp2);
    sp2->tween<IOQuad, Scale>(vec3(0,0,0), vec3(1,1,1), 400u, 0, bind(&SFX::clear_obj, this, effects_holder_.begin()));
}

void SFX::cube_explode(pSprite sp)
{
    sp->setDepth(-10); //so it is not blocked in view;
    sp->tween<OQuad, Scale>(vec3(1.3,1.3,1.3), 300u);
    sp->tween<Linear, Alpha>(0, 300u);

    effects_holder_.push_front(sp);
    sp->tween<Linear, GradientEmissive>(255, 300u, 0, bind(&SFX::clear_obj, this, effects_holder_.begin()));
}
