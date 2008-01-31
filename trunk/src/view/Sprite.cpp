
#include "IrrDevice.hpp"
#include "view/Sprite.hpp"
#include "view/Scene.hpp"
#include "EventDispatcher.hpp"
#include "Button.hpp"
#include "Accessors.hpp"
#include "EasingEquations.hpp"
#include "CustomAnimator.hpp"

#include <sstream>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;

using namespace easing;
using namespace accessor;

using namespace psc;
using namespace view;
using std::tr1::function;


CallbackDelegate& CallbackDelegate::operator =(std::tr1::function<void(pSprite&)> const& cb)
{
    ctrl::EventDispatcher::i().subscribe_obj_event(cb, subscribed_btn_, owner_.lock());
    return *this;
}

CallbackDelegate& CallbackDelegate::setButton(ctrl::Button const* btn)
{
    subscribed_btn_ = btn;
    return *this;
}

void CallbackDelegate::setOwner(pSprite const& owner)
{
    owner_ = owner;
}

/*
    TODO:
    still some problem with material settings(alpha). I'll fix that later.
*/

pSprite Sprite::init(pObject const& parent)
{
    if( name_.size() < 1 ) {
        Object::init(parent);
        return shared_from_this();
    }
    std::ostringstream oss;
    oss << "rc/texture/" << name_ << ".png";

    video::IVideoDriver* driver = smgr_->getVideoDriver();

    SMaterial mat;
    mat.setFlag(video::EMF_LIGHTING, true);
    mat.setTexture(0, driver->getTexture(oss.str().c_str()));

    mat.MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL;
    mat.MaterialTypeParam = 0.01f;

    mat.DiffuseColor.set(255,255,255,255);

    IMesh* mesh = smgr_->getMesh( "rc/model/plane.x" )->getMesh(0);
    body_ = smgr_->addMeshSceneNode( mesh, parent->body(), -1, vector3df(0,0,5) );
    body_->setName( name_.c_str() );

    body_->getMaterial(0) = mat;

    press_.setOwner( shared_from_this() );
    scene_ = parent->scene();

    return shared_from_this();
}

Sprite& Sprite::moveTo(int x, int y)
{
    //body_->setPosition(vector3df(x-320.f+51.f, -y+240.f-21.f, body_->getPosition().Z));
    set<Pos2D>(vector2df(x-320.f+51.f, -y+240.f-21.f));
    return *this;
}

Sprite& Sprite::setDepth(float d)
{
    vector2df pos2d = get<Pos2D>();
    set<Pos3D>(vector3df(pos2d.X, pos2d.Y, d));
    return *this;
}

Sprite& Sprite::moveTween(int x, int y, int delay_ms, function<void()> cb, int delay)
{
    vector2df newpos = vector2df(x-320.f+51.f, -y+240.f-21.f);
    tween<Linear, Pos2D>(newpos, delay_ms, false, cb, delay);
    return *this;
}

Sprite* Sprite::clone() const
{
    Sprite* obj = new Sprite(*this);
    return obj;
}

CallbackDelegate& Sprite::onPress(ctrl::Button const* btn)
{
    return press_.setButton(btn);
}

Sprite::~Sprite()
{
}
