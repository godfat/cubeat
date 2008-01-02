
#include "IrrDevice.hpp"
#include "view/Button.hpp"
#include "view/Scene.hpp"
#include "EventDispatcher.hpp"
#include "Button.hpp"

#include <iostream>

using namespace irr;
using namespace core;
using namespace scene;
using namespace view;

CallbackDelegate& CallbackDelegate::operator =(boost::function<void(Sprite*)> const& cb)
{
    ctrl::EventDispatcher::i().subscribe_obj_event(cb, subscribed_btn_, owner_);
    return *this;
}

CallbackDelegate& CallbackDelegate::setButton(ctrl::Button const* btn)
{
    subscribed_btn_ = btn;
    return *this;
}

/*
Button::Button(Scene const* parent)
    :Sprite(parent)
{
}
*/

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
