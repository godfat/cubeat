
#include "view/AnimatedSceneObject.hpp"
#include "IrrDevice.hpp"

#include <sstream>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace view;

AnimatedSceneObject* AnimatedSceneObject::clone() const
{
    AnimatedSceneObject* obj = new AnimatedSceneObject(*this);
    return obj;
}

void AnimatedSceneObject::init(Object const* parent)
{
    std::ostringstream oss;
    oss << "rc/model/" << path_ << ".x";
    IAnimatedMesh* mesh = smgr_->getMesh( oss.str().c_str() );

    body_ = smgr_->addAnimatedMeshSceneNode( mesh, parent?parent->body():0 );
}
