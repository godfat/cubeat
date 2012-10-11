
#include "IrrDevice.hpp"
#include "view/Scene.hpp"
#include "view/Sprite.hpp"     //temporary
#include "Accessors.hpp"
#include "EventDispatcher.hpp"
#include "ctrl/TimerDispatcher.hpp"
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

Scene::Scene() : rt_camera_(0), rt_plane_(0), rt_(0)
{
    smgr_ = IrrDevice::i().d()->getSceneManager()->createNewSceneManager( false );
}

void Scene::init(std::string const& name)
{
    name_ = name;

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

    allow_picking_ = true;
    std::cout << "scene: " << name << " created." << std::endl;

    //get timer dispatcher for SetTime and it's own time.
    timer_ = ctrl::TimerDispatcher::create(name);
    ctrl::EventDispatcher::i().add_timer_dispatcher(timer_);

    scene_ = std::tr1::static_pointer_cast<Scene>(shared_from_this());
}

void Scene::enableGlobalHittingEvent() {
    BOOST_FOREACH(ctrl::Input const* input, ctrl::InputMgr::i().getInputs()) {
        ctrl::Button const* btn = &input->trig2();
        ctrl::EventDispatcher::i().subscribe_btn_event(
            bind(&Scene::onPressEvent, this, _1, _2, btn), scene_, btn, ctrl::BTN_PRESS);

//note: long_press repeating fire is not used for now.
//        ctrl::Button const* btn = &input->trig2();
//        ctrl::EventDispatcher::i().subscribe_btn_event(
//            bind(&Scene::onDownEvent, this, _1, _2, btn), scene_, btn, ctrl::BTN_DOWN);
    }
}

Scene& Scene::setTo2DView()
{
    float w = static_cast<float>( Conf::i().SCREEN_W() );
    float h = static_cast<float>( Conf::i().SCREEN_H() );
    camera_->setPosition( vector3df(0,0,0) );
    camera_->setTarget( vector3df(0,0,100) );
    float const bloat = 0.0f;    //not useful??? set to 0 for now...

    matrix4 ortho;
    ortho.buildProjectionMatrixOrthoLH( w+bloat, h+bloat, 0, 10000);
    camera_->setProjectionMatrix( ortho, true );

    body_->setScale(vector3df( (w+bloat)/w, (h+bloat)/h, 1 ));
    body_->setPosition(vector3df(-(w+bloat)/2, (h+bloat)/2, 5000));  //default depth at 500..
    return *this;
}

Scene& Scene::setTo3DView(float FoV)
{
    matrix4 proj;
    proj.buildProjectionMatrixPerspectiveFovLH( FoV, (float)Conf::i().SCREEN_W() / Conf::i().SCREEN_H(), 0.0, 1000.0 );
    camera_->setProjectionMatrix( proj );
    camera_->setPosition( vector3df(0,0,0) );
    camera_->setTarget( vector3df(0,0,100) );
    camera_->setFOV( FoV );
    camera_->setAspectRatio( (float)Conf::i().SCREEN_W() / Conf::i().SCREEN_H() );

    body_->setPosition(vector3df(0,0,0));
    return *this;
}

Scene& Scene::enablePostProc()
{
    video::IVideoDriver* driver = IrrDevice::i().d()->getVideoDriver();

    rt_ = driver->addRenderTargetTexture(core::dimension2du(Conf::i().SCREEN_W(), Conf::i().SCREEN_H()), "RTT1");

    rt_camera_ = smgr_->addCameraSceneNode(0, core::vector3df(0,0,0), core::vector3df(0,0,10));
    rt_camera_->setIsDebugObject(true);
    core::matrix4 ortho;
    ortho.buildProjectionMatrixOrthoLH( Conf::i().SCREEN_W(), Conf::i().SCREEN_H(), 0, 10000);
    rt_camera_->setProjectionMatrix( ortho, true );

    rt_plane_ = smgr_->addBillboardSceneNode(rt_camera_, core::dimension2df(Conf::i().SCREEN_W(), Conf::i().SCREEN_H()), core::vector3df(0,0,10));
    rt_plane_->setMaterialFlag(video::EMF_LIGHTING, false);
    rt_plane_->setMaterialTexture(0, rt_);
    rt_plane_->setVisible(false);

    return *this;
}

video::ITexture* Scene::getPostProcTexture() const
{
    return rt_;
}

Scene& Scene::allowPicking(bool f)
{
    allow_picking_ = f;
    return *this;
}

Scene& Scene::activate()
{
    set<Visible>(true);
    return *this;
}

Scene& Scene::redraw()
{
    video::IVideoDriver* driver = IrrDevice::i().d()->getVideoDriver();
    IrrDevice::i().d()->getTimer()->setTime(timer_->get_time());

    if( rt_ ) { // Post Processing Enabled
        driver->setRenderTarget(rt_, true, true, video::SColor(0,0,0,255));
        body_->setVisible(true);
        rt_plane_->setVisible(false);
        smgr_->drawAll();
        driver->setRenderTarget(0, true, true, 0);
        body_->setVisible(false);
        rt_plane_->setVisible(true);
        smgr_->setActiveCamera(rt_camera_);
    }

    smgr_->drawAll();
    smgr_->setActiveCamera(camera_);
    // 2012 WTF note: should I make rt_plane_ or rt_camera_ invisible here? so no picking hindering?
    return *this;
}

Scene& Scene::deactivate()
{
    set<Visible>(false);
    return *this;
}

ICameraSceneNode* Scene::camera() const
{
    return camera_;
}

ISceneCollisionManager* Scene::getCollisionMgr() const
{
    return smgr_->getSceneCollisionManager();
}

//warning: the content of this method should not be in view!
//         because it is not view related.
void Scene::onPressEvent(int x, int y, ctrl::Button const* bound_button)
{
    if ( ctrl::pPlayer p = bound_button->owner()->player() )
        if( p->can_fire() && !p->is_changing_wep() )
            processHit(x, y, bound_button);
}

//warning: the content of this method should not be in view!
//         because it is not view related.
void Scene::onDownEvent(int x, int y, ctrl::Button const* bound_button)
{
    if ( ctrl::pPlayer p = bound_button->owner()->player() )
        if( p->can_fire_repeatedly() && p->can_fire() && !p->is_changing_wep() )
            processHit(x, y, bound_button);
}

//warning: the content of this method should not be in view!
//         because it is not view related.
void Scene::processHit(int x, int y, ctrl::Button const* btn)
{
    if( !allow_picking_ ) return;

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
    wep->fire(point);
}

void Scene::update_focus_objs_by_input(ctrl::Input const* input)
{
    pickmap_[input] = ObjList();
    //warning: some code of this part should not be in view. need refactoring.
    if( ctrl::pPlayer player = input->player() ) {
        vec2 point(input->cursor().x(), input->cursor().y());
        ObjList const& picked = pick(point, player->weapon()->areaPredicate());
        ObjList& pick_input   = pickmap_[input];
        pick_input.insert(pick_input.end(), picked.begin(), picked.end());
    }
    //end of warning.

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

    last_pickmap_[input] = pickmap_[input]; //is this copy? should be copy.
}


std::list<wpObject> const& Scene::pick(vec2 const& p)
{
    picked_temporary_.clear();
    if( !allow_picking_ ) return picked_temporary_;

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
    if( !allow_picking_ ) return picked_temporary_;

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

std::time_t Scene::getTime() const { return timer_->get_time(); }

Scene::~Scene()
{
    std::cout << body_->getName() << " scene died." << std::endl;
    node2view_.clear();
    std::cout << "scene cleared: " << smgr_->drop() << "\n";
    body_ = 0;
}
