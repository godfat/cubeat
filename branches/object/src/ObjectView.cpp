
#include "../include/IrrDevice.hpp"
#include "../include/ObjectView.hpp"
#include "../include/CustomAnimator.hpp"

using namespace irr;
using namespace core;
using namespace scene;
using namespace easing;
using namespace setter;
using boost::function;

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
    body_->setPosition(vector3df(x-320+51,-y+240-21,z));
}

void ObjectView::moveTo(int x, int y, int z, int delay_ms, function<void()> cb)
{
    vector3df oldpos = body_->getPosition();
    vector3df newpos = vector3df(x-320+51,-y+240-21,z);
    ISceneNodeAnimator* anim = 
        new CustomAnimator<vector3df, Linear, Position>(oldpos, newpos, delay_ms, false, cb);
    body_->addAnimator( anim );
    anim->drop();
}

ISceneNode* ObjectView::body() const
{
    return body_;
}

ObjectView::~ObjectView()
{
}