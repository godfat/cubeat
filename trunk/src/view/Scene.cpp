
#include "IrrDevice.hpp"
#include "view/Scene.hpp"
#include "view/Sprite.hpp"     //temporary
#include "Accessors.hpp"
#include "EventDispatcher.hpp"
#include "Input.hpp"
#include "Player.hpp"
#include "Weapon.hpp"
#include "Button.hpp"
#include "Conf.hpp"

#include <boost/foreach.hpp>
#include <iostream>

using namespace irr;
using namespace core;
using namespace scene;

using namespace psc;
using namespace view;
using namespace accessor;
using std::tr1::bind;
using namespace std::tr1::placeholders;
using std::tr1::static_pointer_cast;
using std::tr1::dynamic_pointer_cast;

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

void Scene::enableGlobalHittingEvent() {
    BOOST_FOREACH(ctrl::Input const* input, ctrl::Input::getInputs()) {
        ctrl::Button const* btn = &input->trig2();
        ctrl::EventDispatcher::i().subscribe_btn_event(
            bind(&Scene::onPressEvent, this, _1, _2, btn), scene_, btn, ctrl::BTN_PRESS);

//        ctrl::Button const* btn = &input->trig2();
//        ctrl::EventDispatcher::i().subscribe_btn_event(
//            bind(&Scene::onDownEvent, this, _1, _2, btn), scene_, btn, ctrl::BTN_DOWN);
    }
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

void Scene::onPressEvent(int x, int y, ctrl::Button const* bound_button)
{
    if ( ctrl::pPlayer p = bound_button->owner()->player() )
        if( p->can_fire() && !p->is_changing_wep() )
            processHit(x, y, bound_button);
}

void Scene::onDownEvent(int x, int y, ctrl::Button const* bound_button)
{
    if ( ctrl::pPlayer p = bound_button->owner()->player() )
        if( p->can_fire_repeatedly() && p->can_fire() && !p->is_changing_wep() )
            processHit(x, y, bound_button);
}

void Scene::processHit(int x, int y, ctrl::Button const* btn)
{
    vec2 point(x, y);
    ctrl::Weapon* wep = btn->owner()->player()->weapon();
    ObjList picklist = pick(point, wep->areaPredicate());
    BOOST_FOREACH(wpObject& picked, picklist) {
        //if( pObject obj = picked.lock() )
        if( pSprite obj = dynamic_pointer_cast<Sprite>(picked.lock()) ) { //temporary
            //std::cout << obj << " is hit in (manually called in Scene.)\n";
            obj->triggerHit( btn );
        }
    }
    wep->fire();
}

void Scene::update_focus_objs_by_input(ctrl::Input const* input)
{
    pickmap_[input] = ObjList();
    if( ctrl::pPlayer player = input->player() ) {
        vec2 point(input->cursor().x(), input->cursor().y());
        ObjList const& picked = pick(point, player->weapon()->areaPredicate());
        ObjList& pick_input   = pickmap_[input];
        pick_input.insert(pick_input.end(), picked.begin(), picked.end());
    }

    ObjList& old_list = last_pickmap_[input];  //reference
    ObjList& new_list = pickmap_[input];       //reference
    new_list.sort();                           //sort the newly picked list

    ObjList::const_iterator o = old_list.begin(), oe = old_list.end();
    ObjList::const_iterator n = new_list.begin(), ne = new_list.end();
    ObjList focus_objs_list, leave_objs_list;

    while( o != oe && n != ne ) {
        if( !(*o < *n) && !(*n < *o) ) ++o, ++n;
        else if( *o < *n )
            leave_objs_list.push_back(*o), ++o;
        else if( *n < *o )
            focus_objs_list.push_back(*n), ++n;
    }
    while( o != oe ) leave_objs_list.push_back(*o), ++o;
    while( n != ne ) focus_objs_list.push_back(*n), ++n;

    BOOST_FOREACH(wpObject& weak_obj, focus_objs_list)
        //if( pObject obj = weak_obj.lock() )
        if( pSprite obj = dynamic_pointer_cast<Sprite>(weak_obj.lock()) ) {    //temporary
            //std::cout << obj << " is hovered in (manually called in Scene.)\n";
            obj->triggerHoverIn( input );
        }
    BOOST_FOREACH(wpObject& weak_obj, leave_objs_list)
        //if( pObject obj = weak_obj.lock() )
        if( pSprite obj = dynamic_pointer_cast<Sprite>(weak_obj.lock()) ) {    //temporary
            //std::cout << obj << " is hovered out (manually called in Scene.)\n";
            obj->triggerHoverOut( input );
        }

    last_pickmap_[input] = pickmap_[input]; //copy?
}


std::list<wpObject> const& Scene::pick(vec2 const& p)
{
    picked_temporary_.clear();
    position2di pos(p.X, p.Y);
    ISceneNode* picked = getCollisionMgr()->getSceneNodeFromScreenCoordinatesBB(pos, 1, true);
    if( picked ) {
        picked_temporary_.push_back( node2view_[picked] );
    }
    return picked_temporary_;
}

std::list<wpObject> const& Scene::pick
    (vec2 const& p, std::tr1::function<bool(vec2 const&, vec2 const&)> const& pred)
{
    if( pred == 0 ) return pick(p);

    picked_temporary_.clear();
    ISceneCollisionManager* colm = getCollisionMgr();
    BOOST_FOREACH(Node2ViewPair& nv, node2view_) {
        position2di pos = colm->getScreenCoordinatesFrom3DPosition(nv.first->getAbsolutePosition(), camera_);
        if( pred(p, vec2(pos.X, pos.Y)) )
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
