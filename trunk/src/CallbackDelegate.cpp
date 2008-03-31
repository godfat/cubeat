
#include "CallbackDelegate.hpp"
#include "view/Sprite.hpp"
#include "EventDispatcher.hpp"
#include "Button.hpp"

using namespace psc;
using namespace ctrl;
using std::tr1::function;

CallbackDelegate::
CallbackDelegate(view::pSprite const& owner, Button const* btn, BSTATE const& state)
    :owner_(owner), subscribed_btn_(btn), subscribed_state_(state)
{
}

CallbackDelegate& CallbackDelegate::operator =(function<void(view::pSprite&)> const& cb)
{
    EventDispatcher::i().
        subscribe_obj_event(cb, owner_.lock(), subscribed_btn_, subscribed_state_);
    return *this;
}

CallbackDelegate& CallbackDelegate::setButton(ctrl::Button const* btn)
{
    subscribed_btn_ = btn;
    return *this;
}

void CallbackDelegate::setOwner(view::pSprite const& owner)
{
    owner_ = owner;
}
