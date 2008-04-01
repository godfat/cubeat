
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

Object& Object::setTexture(std::string const& path)
{
    std::string full_path("rc/texture/");
    full_path += path + ".png";
    video::IVideoDriver* driver = smgr_->getVideoDriver();
    body_->setMaterialTexture(0, driver->getTexture(full_path.c_str()) );

    //temp code
    if( name_ == "unnamed" ) {
        name_ = path;
        body_->setName(path.c_str());
    }
    return *this;
}

Object& Object::clearAllTween()
{
    clearAllQueuedTween();
    body_->removeAnimators();
    return *this;
}

Object& Object::clearAllQueuedTween()
{
    BOOST_FOREACH(AnimatorBase*& anim, anim_queue_)
        anim->drop();
    anim_queue_.clear();
    return *this;
}

Object& Object::clearTween(AT::ATEnum const& eType)
{
    typedef core::list< ISceneNodeAnimator* > IrrAnimList;
    IrrAnimList const& alist = body_->getAnimators();
    for( IrrAnimList::ConstIterator it = alist.begin(), end = alist.end();
         it != end; ++it ) {
        if( static_cast<int>((*it)->getType()) == static_cast<int>(eType) ) //work?
            smgr_->addToAnimatorDeletionQueue(*it, body_);
    }
    return *this;
}

Object& Object::clearQueuedTween(AT::ATEnum const& eType)
{
    typedef std::list< AnimatorBase* > AnimList;
    typedef std::list< AnimList::iterator > AnimRemoval;
    AnimRemoval removal;
    for( AnimList::iterator it = anim_queue_.begin(), end = anim_queue_.end();
         it != end; ++it ) {
        if( static_cast<int>((*it)->getType()) == static_cast<int>(eType) ) //work?
            removal.push_back(it);
    }
    BOOST_FOREACH(AnimList::iterator& it, removal)
        anim_queue_.erase(it);
    removal.clear();
    return *this;
}

void Object::startTween() {
    if( anim_queue_.size() > 0 ) {
        AnimatorBase* anim = anim_queue_.front();
        anim_queue_.pop_front();
        anim->updateStartTime();
        clearTween( static_cast<AT::ATEnum>(anim->getType()) ); //del all animators of same type
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
    clearAllQueuedTween();
    if( body_ ) {
        if( scene() )
            scene()->removePickMapping( body_ );
        body_->remove();
    }
}
