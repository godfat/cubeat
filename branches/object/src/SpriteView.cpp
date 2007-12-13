
#include "../include/IrrDevice.hpp"
#include "../include/SpriteView.hpp"
#include "../include/GUIView.hpp"
#include "boost/bind.hpp"

using namespace irr;
using namespace core;
using namespace scene;
using boost::function;
using boost::bind;

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

void SpriteView::step1() {
    moveTo( 540, 0, 10, 4000, bind(&SpriteView::step2, this) );
}
void SpriteView::step2() {
    moveTo( 540, 420, 10, 3000, bind(&SpriteView::step3, this) );
}
void SpriteView::step3() {
    moveTo( 0, 420, 10, 4000, bind(&SpriteView::step4, this) );
}
void SpriteView::step4() {
    moveTo( 0, 0, 10, 3000, bind(&SpriteView::step1, this) );
}

SpriteView::~SpriteView()
{
}