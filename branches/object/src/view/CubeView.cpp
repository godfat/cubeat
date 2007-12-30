
#include "view/CubeView.hpp"
#include "IrrDevice.hpp"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;

CubeView* CubeView::clone() const
{
    CubeView* obj = new CubeView(*this);
    return obj;
}

CubeView& CubeView::init(ObjectView const* parent)
{
    SceneObjectView::init(parent);
    return *this;
}

void CubeView::ownerHit()
{
}

void CubeView::enemyHit()
{
}