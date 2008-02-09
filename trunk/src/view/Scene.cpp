
#include "IrrDevice.hpp"
#include "view/Scene.hpp"
#include "utils/Random.hpp"
#include "Accessors.hpp"

#include <iostream>

using namespace irr;
using namespace core;
using namespace scene;
using namespace accessor;

using namespace psc;
using namespace view;

void Scene::init(pObject const& parent, std::string const& name)
{
    camera_ = smgr_->addCameraSceneNode(parent?parent->body():0);
    camera_->setIsDebugObject(true);

	light_ =
		smgr_->addLightSceneNode(0, core::vector3df(0.0,0.0,-1.0), video::SColorf(1.0f, 1.0f, 1.0f), 100.0f);
    light_->setIsDebugObject(true);
    light_->setRotation(vector3df(0.f, 0.f, 0.f));
    light_->getLightData().Type = video::ELT_DIRECTIONAL;
    light_->getLightData().CastShadows = false;

    body_ = smgr_->addEmptySceneNode(camera_);
    body_->setIsDebugObject(true);
    body_->setName( name.c_str() );

    scene_ = shared_from_this();
    deactivate();
}

Scene& Scene::setTo2DView()
{
    video::IVideoDriver* driver = smgr_->getVideoDriver();
    f32 w = (float)driver->getScreenSize().Width;
    f32 h = (float)driver->getScreenSize().Height;
    camera_->setPosition( vector3df(10000,0,0) );
    camera_->setTarget( vector3df(10000,0,100) );

    matrix4 ortho;
    ortho.buildProjectionMatrixOrthoLH( static_cast<f32>(w), static_cast<f32>(h), 0, 10000);
    camera_->setProjectionMatrix( ortho );
    camera_->setIsOrthogonal(true);

    body_->setPosition(vector3df(-w/2, h/2, 500));
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

    body_->setPosition(vector3df(0,0,0));
    return *this;
}

Scene& Scene::activate()
{
    smgr_->setActiveCamera( camera_ );
    camera_->setVisible( true );
    light_->setVisible( true );
    set<Visible>(true);
    return *this;
}

Scene& Scene::redraw()
{
    smgr_->drawAll();
    return *this;
}

Scene& Scene::deactivate()
{
    smgr_->setActiveCamera( 0 );
    camera_->setVisible( false );
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
    std::cout << body_->getName() << " scene died.\n";
}
