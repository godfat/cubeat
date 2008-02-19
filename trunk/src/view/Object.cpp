
#include "IrrDevice.hpp"
#include "view/Object.hpp"
#include "view/Scene.hpp"

#include "Accessors.hpp"

using namespace irr;
using namespace core;
using namespace scene;

using namespace psc;
using namespace view;
using namespace accessor;

Object::Object(std::string const& name)
    :smgr_(IrrDevice::i().d()->getSceneManager()),
     body_(0), name_(name)
{

}

Object* Object::clone() const
{
    Object* obj = new Object(*this);
    return obj;
}

void Object::init(pObject const& parent)
{
    body_ = smgr_->addEmptySceneNode(parent?parent->body():0);

    /* DEBUG CODE */
    body_->setDebugDataVisible(scene::EDS_BBOX_BUFFERS);
    body_->setIsDebugObject(true);            //???????
    body_->setPosition(vector3df(0,0,100));   //??????? ghostly blocking everything in the scene.
    /* END OF DEBUG CODE */
    if( parent )
        scene_ = parent->scene();
}

Object& Object::moveTo(int x, int y, int z)
{
    //body_->setPosition(vector3df(x,y,z));
    set<Pos3D>(vector3df(x,y,z));
    return *this;
}

pScene Object::scene()
{
    return scene_.lock();
}

ISceneNode* Object::body() const
{
    return body_;
}

Object::~Object()
{
}
