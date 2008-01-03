
#include "IrrDevice.hpp"
#include "view/Button.hpp"
#include "view/Scene.hpp"
#include "EventDispatcher.hpp"
#include "Button.hpp"

#include <iostream>

using namespace irr;
using namespace core;
using namespace scene;

using namespace psc;
using namespace view;

CallbackDelegate& CallbackDelegate::operator =(boost::function<void(pSprite&)> const& cb)
{
    ctrl::EventDispatcher::i().subscribe_obj_event(cb, subscribed_btn_, owner_.lock());
    return *this;
}

CallbackDelegate& CallbackDelegate::setButton(ctrl::Button const* btn)
{
    subscribed_btn_ = btn;
    return *this;
}

void CallbackDelegate::setOwner(pButton const owner)
{
    owner_ = owner;
}

/*
Button::Button(Scene const* parent)
    :Sprite(parent)
{
}
*/

void Button::init(pObject const& parent)
{
    Sprite::init(parent);
    press_.setOwner( std::tr1::static_pointer_cast<Button>(shared_from_this()) );
}

Button* Button::clone() const
{
    Button* obj = new Button(*this);
    return obj;
}

CallbackDelegate& Button::onPress(ctrl::Button const* btn)
{
    //std::cout << "Something Clicked.\n";
    return press_.setButton(btn);
}

Button::~Button()
{
}
