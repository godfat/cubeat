
#include "view/AnimatedSceneObject.hpp"
#include "IrrDevice.hpp"

#include <sstream>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;

using namespace psc;
using namespace view;

AnimatedSceneObject* AnimatedSceneObject::clone() const
{
    AnimatedSceneObject* obj = new AnimatedSceneObject(*this);
    return obj;
}

pAnimatedSceneObject AnimatedSceneObject::init(pObject const& parent)
{
    setupSceneAndManager(parent);

    std::ostringstream oss;
    oss << "rc/model/" << name_ << ".x";
    IAnimatedMesh* mesh = smgr_->getMesh( oss.str().c_str() );
    IAnimatedMeshSceneNode* temp = smgr_->addAnimatedMeshSceneNode( mesh, parent->body() );
    temp->setAnimationSpeed(0);
    body_ = temp;
    body_->getMaterial(0).Shininess = 0;

    return std::tr1::static_pointer_cast<AnimatedSceneObject>( shared_from_this() );
}
