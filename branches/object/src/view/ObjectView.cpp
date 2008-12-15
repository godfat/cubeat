
#include "IrrDevice.hpp"
#include "view/ObjectView.hpp"

#include "Accessors.hpp"

using namespace irr;
using namespace core;
using namespace scene;
using namespace accessor;

//ObjectView::ObjectView(ObjectView const* parent)
ObjectView::ObjectView()
    :smgr_(IrrDevice::i()->getSceneManager()),
     body_(0)
{
    //init(parent);
}

ObjectView* ObjectView::clone() const
{
    ObjectView* obj = new ObjectView(*this);
    return obj;
}

ObjectView& ObjectView::init(ObjectView const* parent)
{
    body_ = smgr_->addEmptySceneNode(parent?parent->body():0);
    return *this;
}

void ObjectView::moveTo(int x, int y, int z)
{
    //body_->setPosition(vector3df(x,y,z));
    set<Pos3D>(vector3df(x,y,z));
}

ISceneNode* ObjectView::body() const
{
    return body_;
}

ObjectView::~ObjectView()
{
}
