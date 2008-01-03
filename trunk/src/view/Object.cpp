
#include "IrrDevice.hpp"
#include "view/Object.hpp"

#include "Accessors.hpp"

using namespace irr;
using namespace core;
using namespace scene;
using namespace accessor;

using namespace psc;
using namespace view;

//Object::Object(Object const* parent)
Object::Object()
    :smgr_(IrrDevice::i()->getSceneManager()),
     body_(0)
{
    //init(parent);
}

Object* Object::clone() const
{
    Object* obj = new Object(*this);
    return obj;
}

void Object::init(pObject const parent)
{
    body_ = smgr_->addEmptySceneNode(parent?parent->body():0);
}

Object& Object::moveTo(int x, int y, int z)
{
    //body_->setPosition(vector3df(x,y,z));
    set<Pos3D>(vector3df(x,y,z));
    return *this;
}

ISceneNode* Object::body() const
{
    return body_;
}

Object::~Object()
{
}
