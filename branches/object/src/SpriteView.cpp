
#include "../include/IrrDevice.hpp"
#include "../include/SpriteView.hpp"
#include "../include/GUIView.hpp"

using namespace irr;
using namespace core;
using namespace scene;

SpriteView::SpriteView(GUIView const* parent)
    :ObjectView(parent)
{
    video::IVideoDriver* driver = smgr_->getVideoDriver();

    mat_.setFlag(video::EMF_LIGHTING, false);
    mat_.setTexture(0, driver->getTexture("rc/texture/title.png"));
    mat_.MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL;
    mat_.MaterialTypeParam = 0.001f;

    body_ = smgr_->addBillboardSceneNode(parent->body(), dimension2df(100,40), vector3df(0,0,5));
    body_->setScale(vector3df(10.0/5.0,1.0,1.0));

    body_->getMaterial(0) = mat_;
}

SpriteView* SpriteView::clone() const
{
    SpriteView* obj = new SpriteView(*this);
    return obj;
}

SpriteView::~SpriteView()
{
}