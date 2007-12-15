
#include "../include/IrrDevice.hpp"
#include "../include/ObjectView.hpp"
#include "../include/Accessors.hpp"
#include "../include/EasingEquations.hpp"
#include "../include/CustomAnimator.hpp"

using namespace irr;
using namespace core;
using namespace scene;
using namespace easing;
using namespace accessor;
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
    body_->setPosition(vector3df(x-320.f+51.f,-y+240.f-21.f,z-0.f));
}

void ObjectView::moveTo(int x, int y, int z, int delay_ms, function<void()> cb)
{
    vector2df newpos = vector2df(x-320.f+51.f, -y+240.f-21.f);
    ISceneNodeAnimator* anim = 
        new CustomAnimator<Linear, Pos2D>(body_, newpos, delay_ms, false, cb);
}

ISceneNode* ObjectView::body() const
{
    return body_;
}

ObjectView::~ObjectView()
{
}