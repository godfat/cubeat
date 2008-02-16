
#include "IrrDevice.hpp"
#include "view/Scene.hpp"
#include "utils/Random.hpp"
#include "Accessors.hpp"

#include <iostream>

using namespace irr;
using namespace core;
using namespace scene;

using namespace psc;
using namespace view;
using namespace accessor;

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
    float w = (float)driver->getScreenSize().Width;
    float h = (float)driver->getScreenSize().Height;
    camera_->setPosition( vector3df(10000,0,0) );
    camera_->setTarget( vector3df(10000,0,100) );
    float const bloat = 200.0f; //size bloat in case items are culled when they are near the edge

    matrix4 ortho;
    ortho.buildProjectionMatrixOrthoLH( w+bloat, h+bloat, 0, 10000);
    camera_->setProjectionMatrix( ortho );
    camera_->setIsOrthogonal(true);

    body_->setScale(vector3df( (w+bloat)/w, (h+bloat)/h, 1 ));
    body_->setPosition(vector3df(-(w+bloat)/2, (h+bloat)/2, 500));  //default depth at 500..
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
