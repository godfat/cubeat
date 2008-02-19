
#include "view/SceneObject.hpp"
#include "IrrDevice.hpp"

#include <sstream>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;

using namespace psc;
using namespace view;

SceneObject* SceneObject::clone() const
{
    SceneObject* obj = new SceneObject(*this);
    return obj;
}

void SceneObject::init(pObject const& parent)
{
    std::ostringstream oss;
    oss << "rc/model/" << name_ << ".x";
    IMesh* mesh = smgr_->getMesh( oss.str().c_str() )->getMesh(0);

    body_ = smgr_->addMeshSceneNode( mesh, parent?parent->body():0 );
}
