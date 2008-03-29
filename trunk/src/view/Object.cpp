
#include "IrrDevice.hpp"
#include "view/Object.hpp"
#include "view/Scene.hpp"

#include "Accessors.hpp"
#include <boost/foreach.hpp>

#include <iostream>

using namespace irr;
using namespace core;
using namespace scene;

using namespace psc;
using namespace view;
using namespace accessor;
using std::tr1::dynamic_pointer_cast;

Object::Object(std::string const& name)
    :smgr_(0), body_(0), name_(name)
{

}

Object* Object::clone() const
{
    Object* obj = new Object(*this);
    return obj;
}

void Object::setupSceneAndManager(pObject const& parent)
{
    if( pScene p = dynamic_pointer_cast<Scene>( parent ) )
        scene_ = p;
    else scene_ = parent->scene();
    smgr_ = scene()->smgr_;
}

void Object::init(pObject const& parent)
{
    setupSceneAndManager(parent);
    body_ = smgr_->addEmptySceneNode( parent->body() );
    body_->setIsDebugObject(true);
}

Object& Object::moveTo(int x, int y, int z)
{
    //body_->setPosition(vector3df(x,y,z));
    set<Pos3D>(vector3df(x,y,z));
    return *this;
}

void Object::startTween() {
    if( anim_queue_.size() > 0 ) {
        AnimatorBase* anim = anim_queue_.front();
        anim_queue_.pop_front();
        anim->updateStartTime();
        body_->addAnimator( anim );
        anim->drop();
    }
}

void Object::nextTween(std::tr1::function<void()>const& orig_cb)
{
    if( orig_cb )
        orig_cb();
    startTween();
}

pScene Object::scene() const
{
    return scene_.lock();
}

ISceneNode* Object::body() const
{
    return body_;
}

Object::~Object()
{
    BOOST_FOREACH(AnimatorBase*& anim, anim_queue_)
        anim->drop();
    anim_queue_.clear();
    if( body_ ) body_->remove();
}
