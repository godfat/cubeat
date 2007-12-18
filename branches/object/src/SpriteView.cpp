
#include "../include/IrrDevice.hpp"
#include "../include/SpriteView.hpp"
#include "../include/GUIView.hpp"
#include "../include/Accessors.hpp"
#include "../include/EasingEquations.hpp"
#include "../include/CustomAnimator.hpp"

using namespace irr;
using namespace core;
using namespace scene;
using namespace easing;
using namespace accessor;
using boost::function;

SpriteView::SpriteView(GUIView const* parent)
    :ObjectView(parent)
{
    video::IVideoDriver* driver = smgr_->getVideoDriver();

    mat_.setFlag(video::EMF_LIGHTING, true);
    mat_.setTexture(0, driver->getTexture("rc/texture/title.png"));
    mat_.MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL;
    mat_.MaterialTypeParam = 0.001f;
    mat_.EmissiveColor.set(0,255,255,255);

    body_ = smgr_->addBillboardSceneNode(parent->body(), dimension2df(100,40), vector3df(0,0,5));
    body_->setScale(vector3df(10.0/5.0,1.0,1.0));

    body_->getMaterial(0) = mat_;
}

void SpriteView::moveTo(int x, int y)
{
    body_->setPosition(vector3df(x-320.f+51.f, -y+240.f-21.f, body_->getPosition().Z));
}

void SpriteView::moveTween(int x, int y, int delay_ms, function<void()> cb, int delay)
{
    vector2df newpos = vector2df(x-320.f+51.f, -y+240.f-21.f);
    tween<Linear, Pos2D>(newpos, delay_ms, false, cb, delay);
}

SpriteView* SpriteView::clone() const
{
    SpriteView* obj = new SpriteView(*this);
    return obj;
}

SpriteView::~SpriteView()
{
}