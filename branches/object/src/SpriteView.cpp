
#include "../include/IrrDevice.hpp"
#include "../include/SpriteView.hpp"
#include "../include/GUIView.hpp"
#include "../include/Accessors.hpp"
#include "../include/EasingEquations.hpp"
#include "../include/CustomAnimator.hpp"
#include <iostream>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace easing;
using namespace accessor;
using std::tr1::function;

SpriteView::SpriteView(GUIView const* parent)
    :ObjectView(parent)
{
    video::IVideoDriver* driver = smgr_->getVideoDriver();
    driver->setTextureCreationFlag(video::ETCF_OPTIMIZED_FOR_QUALITY, true);

    mat_.setFlag(video::EMF_LIGHTING, true);
    mat_.setTexture(0, driver->getTexture("rc/texture/title.png"));
    mat_.MaterialType = video::EMT_ONETEXTURE_BLEND;

    /* you see, the alpha blend func hijacked the default DiffuseColor's alpha value,
       making the DiffuseColor's alpha useless when you use a texture with a valid alpha channel.
       There must be some way in the engine I can change to help with this issue,
       but that probably will take some time, so I decide to leave it as it is for now. */

    //mat_.MaterialTypeParam = 0.001f;
    mat_.MaterialTypeParam = pack_texureBlendFunc(video::EBF_SRC_ALPHA, 
                                                  video::EBF_ONE_MINUS_SRC_ALPHA, 
                                                  video::EMFN_MODULATE_1X);
    mat_.DiffuseColor.set(0,0,0,0);
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
    // std::cout << "before move, this: " << this << std::endl;
    // std::cout << "before move, body_: " << body_ << std::endl;
    vector2df newpos = vector2df(x-320.f+51.f, -y+240.f-21.f);
    tween<Linear, Pos2D>(newpos, delay_ms, false, cb, delay);
    // std::cout << "after move, this: " << this << std::endl;
    // std::cout << "after move, body_: " << body_ << std::endl;
}

SpriteView* SpriteView::clone() const
{
    SpriteView* obj = new SpriteView(*this);
    return obj;
}

SpriteView::~SpriteView()
{
}