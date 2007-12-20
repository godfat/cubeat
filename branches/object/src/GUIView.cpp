
#include "../include/IrrDevice.hpp"
#include "../include/GUIView.hpp"

using namespace irr;
using namespace core;
using namespace scene;

GUIView::GUIView(ObjectView const* parent)
    :ObjectView(parent)
{
    camera_ = 
        smgr_->addCameraSceneNode(parent?parent->body():0, vector3df(10000,0,0), vector3df(10000,0,100));
    matrix4 ortho;
    ortho.buildProjectionMatrixOrthoLH( static_cast<f32>(640), static_cast<f32>(480), 0, 20);
    camera_->setProjectionMatrix( ortho );
    camera_->setIsOrthogonal(true);

	light_ = 
		smgr_->addLightSceneNode(0, core::vector3df(0.0,0.0,-1.0), video::SColorf(1.0f, 1.0f, 1.0f), 100.0f);
    light_->setRotation(vector3df(0.f, 0.f, 0.f));
    light_->getLightData().Type = video::ELT_DIRECTIONAL;
    light_->getLightData().CastShadows = false;

    body_ = smgr_->addEmptySceneNode(camera_);
}

GUIView* GUIView::clone() const
{
    GUIView* obj = new GUIView(*this);
    return obj;
}

ICameraSceneNode* GUIView::camera() const
{
    return camera_;
}

GUIView::~GUIView()
{
}