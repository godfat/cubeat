
#include "view/SceneObjectView.hpp"
#include "IrrDevice.hpp"

#include <sstream>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;

SceneObjectView* SceneObjectView::clone() const
{
    SceneObjectView* obj = new SceneObjectView(*this);
    return obj;
}

SceneObjectView& SceneObjectView::init(ObjectView const* parent)
{
    std::ostringstream oss;
    oss << "rc/model/" << path_ << ".x";
    IMesh* mesh = smgr_->getMesh( oss.str().c_str() )->getMesh(0);

    body_ = smgr_->addMeshSceneNode( mesh, parent?parent->body():0 );
    return *this;
}