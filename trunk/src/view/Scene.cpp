
#include "IrrDevice.hpp"
#include "view/Scene.hpp"
#include "Accessors.hpp"
#include "EventDispatcher.hpp"
#include "Conf.hpp"

#include <boost/foreach.hpp>
#include <iostream>

using namespace irr;
using namespace core;
using namespace scene;

using namespace psc;
using namespace view;
using namespace accessor;
using std::tr1::static_pointer_cast;

Scene::Scene()
{
    smgr_ = IrrDevice::i().d()->getSceneManager()->createNewSceneManager( false );
}

void Scene::init(std::string const& name)
{
    camera_ = smgr_->addCameraSceneNode(0);
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

    std::cout << name << "'s collision manager: " << smgr_->getSceneCollisionManager() << "\n";

    scene_ = std::tr1::static_pointer_cast<Scene>(shared_from_this());
}

Scene& Scene::setTo2DView()
{
    float w = (float)Conf::i().SCREEN_W;
    float h = (float)Conf::i().SCREEN_H;
//    camera_->setPosition( vector3df(10000,0,0) );
//    camera_->setTarget( vector3df(10000,0,100) );
    camera_->setPosition( vector3df(0,0,0) );
    camera_->setTarget( vector3df(0,0,100) );
    float const bloat = 0.0f;    //not useful??? set to 0 for now...

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
    camera_->setPosition( vector3df(0,0,0) );
    camera_->setTarget( vector3df(0,0,100) );
    camera_->setFOV( FoV );
    camera_->setAspectRatio( (float)Conf::i().SCREEN_W / Conf::i().SCREEN_H );
    camera_->setIsOrthogonal(false);

    body_->setPosition(vector3df(0,0,0));
    return *this;
}

Scene& Scene::activate()
{
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

ISceneCollisionManager* Scene::getCollisionMgr() const
{
    return smgr_->getSceneCollisionManager();
}

std::list<wpObject> const& Scene::pick(vec2 const& p)
{
    picked_temporary_.clear();
    position2di pos(p.X, p.Y);
    ISceneNode* picked = getCollisionMgr()->getSceneNodeFromScreenCoordinatesBB(pos, 1, true);
    if( picked )
        picked_temporary_.push_back( node2view_[picked] );
    return picked_temporary_;
}

std::list<wpObject> const& Scene::pick(vec2 const& p, int const& r)
{
    picked_temporary_.clear();
    ISceneCollisionManager* colm = getCollisionMgr();
    vec2 scene_offset_ = get<Pos2D>();
    BOOST_FOREACH(Node2ViewPair& nv, node2view_) {
        position2di pos = colm->getScreenCoordinatesFrom3DPosition(nv.first->getPosition(), camera_);
        vec2 object_pos(pos.X + scene_offset_.X, pos.Y + scene_offset_.Y);
        if( (p - object_pos).getLengthSQ() < r*r )
            picked_temporary_.push_back( nv.second );
    }
    return picked_temporary_;
}

std::list<wpObject> const& Scene::pick(vec2 const& p, int const& w, int const& h)
{
    picked_temporary_.clear();
    ISceneCollisionManager* colm = getCollisionMgr();
    vec2 scene_offset_ = get<Pos2D>();
    BOOST_FOREACH(Node2ViewPair& nv, node2view_) {
        position2di pos = colm->getScreenCoordinatesFrom3DPosition(nv.first->getPosition(), camera_);
        vec2 object_pos(pos.X + scene_offset_.X, pos.Y + scene_offset_.Y);
        if( object_pos.X > p.X - w/2 && object_pos.X < p.X + w/2 &&
            object_pos.Y > p.Y - h/2 && object_pos.Y < p.Y + h/2 )
            picked_temporary_.push_back( nv.second );
    }
    return picked_temporary_;
}

void Scene::addPickMapping(ISceneNode* node, wpObject const& obj)
{
    node2view_.insert( std::make_pair(node, obj) );
}

void Scene::removePickMapping(ISceneNode* node)
{
    node2view_.erase( node );
}

Scene::~Scene()
{
    std::cout << body_->getName() << " scene died.\n";
    node2view_.clear();
    smgr_->drop();
    body_ = 0;
}
