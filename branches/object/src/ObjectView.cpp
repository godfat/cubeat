
#include "../include/IrrDevice.hpp"
#include "../include/ObjectView.hpp"

using namespace irr;
using namespace core;
using namespace scene;

ObjectView::ObjectView(ObjectView const* parent)
    :smgr_(IrrDevice::i()->getSceneManager()), 
     body_(0)
{
}

ObjectView* ObjectView::clone() const
{
    ObjectView* obj = new ObjectView(*this);
    return obj;
}

void ObjectView::moveTo(int x, int y, int z)
{
    body_->setPosition(vector3df(x,y,z));
}

ISceneNode* ObjectView::body() const
{
    return body_;
}

ObjectView::~ObjectView()
{
}