
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

void AnimatedSceneObject::init(pObject const& parent)
{
    std::ostringstream oss;
    oss << "rc/model/" << name_ << ".x";
    IAnimatedMesh* mesh = smgr_->getMesh( oss.str().c_str() );

    body_ = smgr_->addAnimatedMeshSceneNode( mesh, parent?parent->body():0 );
    body_->setRotation(vector3df(0.f, 60.f, 0.f));
    body_->getMaterial(0).Shininess = 0;
}
