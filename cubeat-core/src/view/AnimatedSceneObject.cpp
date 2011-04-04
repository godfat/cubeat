
#include "view/AnimatedSceneObject.hpp"
#include "view/Scene.hpp"
#include "IrrDevice.hpp"
#include "Conf.hpp"

#include <sstream>
#include <utility>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;

using namespace psc;
using namespace view;
using std::tr1::static_pointer_cast;

AnimatedSceneObject* AnimatedSceneObject::clone() const
{
    AnimatedSceneObject* obj = new AnimatedSceneObject(*this);
    return obj;
}

pAnimatedSceneObject AnimatedSceneObject::init(pObject const& parent)
{
    setupSceneAndManager(parent);

    std::ostringstream oss;
    oss << Conf::i().expand("rc/model/") << name_ << ".x";
    IAnimatedMesh* mesh = smgr_->getMesh( oss.str().c_str() );
    IAnimatedMeshSceneNode* temp = smgr_->addAnimatedMeshSceneNode( mesh, parent->body() );
    temp->setAnimationSpeed(0);
    body_ = temp;
    body_->getMaterial(0).Shininess = 0;
    for( size_t i = 0; i < body_->getMaterialCount(); ++i ) {
        body_->getMaterial(i).MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL;
        body_->getMaterial(i).MaterialTypeParam = 0.01f;
        body_->getMaterial(i).DiffuseColor.set(255, 255, 255, 255);
    }

    pAnimatedSceneObject self = static_pointer_cast<AnimatedSceneObject>( shared_from_this() );
    scene()->addPickMapping( body_, self );
    return self;
}

bool AnimatedSceneObject::isPlaying() const
{
    typedef core::list< ISceneNodeAnimator* > IrrAnimList;
    IrrAnimList const& alist = body_->getAnimators();
    for( IrrAnimList::ConstIterator it = alist.begin(), end = alist.end();
         it != end; ++it ) {
        if( static_cast<int>((*it)->getType()) == static_cast<int>(accessor::AT::FRAME) )
            return true;
    }
    return false;
}

