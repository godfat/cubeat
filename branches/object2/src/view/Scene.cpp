
#include "IrrDevice.hpp"
#include "view/Scene.hpp"
#include "Accessors.hpp"

using namespace irr;
using namespace core;
using namespace scene;
using namespace accessor;
using namespace view;

/*
Scene::Scene(Object const* parent)
    :Object(parent)
{
}
*/
void Scene::init(Object const* parent)
{
    camera_ =
        smgr_->addCameraSceneNode(parent?parent->body():0); 

	light_ =
		smgr_->addLightSceneNode(0, core::vector3df(0.0,0.0,-1.0), video::SColorf(1.0f, 1.0f, 1.0f), 100.0f);
    light_->setRotation(vector3df(0.f, 0.f, 0.f));
    light_->getLightData().Type = video::ELT_DIRECTIONAL;
    light_->getLightData().CastShadows = false;

    body_ = smgr_->addEmptySceneNode(camera_);
}

Scene& Scene::setTo2DView(int w, int h)
{
    camera_->setPosition( vector3df(10000,0,0) );
    camera_->setTarget( vector3df(10000,0,100) );

    matrix4 ortho;
    ortho.buildProjectionMatrixOrthoLH( static_cast<f32>(w), static_cast<f32>(h), 0, 100);
    camera_->setProjectionMatrix( ortho );
    camera_->setIsOrthogonal(true);
    return *this;
}

Scene& Scene::setTo3DView(float FoV)
{
    video::IVideoDriver* driver = smgr_->getVideoDriver();

    camera_->setPosition( vector3df(0,0,0) );
    camera_->setTarget( vector3df(0,0,100) );
    camera_->setFOV( FoV );
    camera_->setAspectRatio( (float)driver->getScreenSize().Width / driver->getScreenSize().Height );
    camera_->setIsOrthogonal(false);
    
    return *this;
}

Scene& Scene::activate()
{
    smgr_->setActiveCamera( camera_ );
    light_->setVisible( true );
    set<Visible>(true);
    return *this;
}

Scene& Scene::deactivate()
{
    light_->setVisible( false );
    set<Visible>(false);
    return *this;
}

Scene* Scene::clone() const
{
    Scene* obj = new Scene(*this);
    return obj;
}

ICameraSceneNode* Scene::camera() const
{
    return camera_;
}

Scene::~Scene()
{
}
