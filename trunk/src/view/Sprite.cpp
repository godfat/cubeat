
#include "IrrDevice.hpp"
#include "view/Sprite.hpp"
#include "view/Scene.hpp"
#include "Accessors.hpp"
#include "EasingEquations.hpp"
#include "CustomAnimator.hpp"
#include <iostream>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;

using namespace easing;
using namespace accessor;

using namespace psc;
using namespace view;
using std::tr1::function;
/*
Sprite::Sprite(Scene const* parent)
    :Object(parent)
{
}
*/
void Sprite::init(pObject const& parent)
{
    video::IVideoDriver* driver = smgr_->getVideoDriver();

    SMaterial mat_;

    mat_.setFlag(video::EMF_LIGHTING, true);
    mat_.setTexture(0, driver->getTexture("rc/texture/title.png"));
/*    mat_.MaterialType = video::EMT_ONETEXTURE_BLEND;

    mat_.MaterialTypeParam =
        video::pack_texureBlendFunc(EBF_SRC_ALPHA, EBF_ONE_MINUS_SRC_ALPHA, EMFN_MODULATE_1X); */

    mat_.MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL;
    mat_.MaterialTypeParam = 0.1f;

    //still some problem with material settings. I'll fix that later.

    mat_.DiffuseColor.set(255,255,255,255);

    body_ = smgr_->addBillboardSceneNode(parent->body(), dimension2df(100,40), vector3df(0,0,5));
    body_->setScale(vector3df(10.0/5.0,1.0,1.0));

    body_->getMaterial(0) = mat_;
}

Sprite& Sprite::moveTo(int x, int y)
{
    //body_->setPosition(vector3df(x-320.f+51.f, -y+240.f-21.f, body_->getPosition().Z));
    set<Pos2D>(vector2df(x-320.f+51.f, -y+240.f-21.f));
    return *this;
}

Sprite& Sprite::setDepth(float d)
{
    vector2df pos2d = get<Pos2D>();
    set<Pos3D>(vector3df(pos2d.X, pos2d.Y, d));
    return *this;
}

Sprite& Sprite::moveTween(int x, int y, int delay_ms, function<void()> cb, int delay)
{
    vector2df newpos = vector2df(x-320.f+51.f, -y+240.f-21.f);
    tween<Linear, Pos2D>(newpos, delay_ms, false, cb, delay);
    return *this;
}

Sprite* Sprite::clone() const
{
    Sprite* obj = new Sprite(*this);
    return obj;
}

Sprite::~Sprite()
{
}
