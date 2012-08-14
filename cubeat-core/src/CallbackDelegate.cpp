
#include "CallbackDelegate.hpp"
#include "view/Sprite.hpp"
#include "EventDispatcher.hpp"
#include "ctrl/TimerDispatcher.hpp"
#include "Button.hpp"
#include "Input.hpp"

using namespace psc;
using namespace ctrl;
using std::tr1::function;

CallbackDelegate::CallbackDelegate
    (view::pSprite const& owner, Button const* btn, BSTATE const& state, bool const& manual)
    :owner_(owner), subscribed_btn_(btn), subscribed_state_(state), manual_(manual), cb_(0)
{
}

CallbackDelegate& CallbackDelegate::operator =(function<void(view::pSprite&)> const& cb)
{
    if( cb_ == 0 && !manual_ ) {
        EventDispatcher::i().
            subscribe_obj_event(&cb_, owner_.lock(), subscribed_btn_, subscribed_state_);
    }
    cb_ = cb;
    return *this;
}

Button const* CallbackDelegate::subscribed_btn()   const { return subscribed_btn_; }
BSTATE        CallbackDelegate::subscribed_state() const { return subscribed_state_; }

void CallbackDelegate::call_manually() {
    if( cb_ != 0 && manual_ ) {
        if( view::pSprite sv = owner_.lock() )
            cb_(sv);
    }
}

////////////////////////////////////////////////////////////////////////////////////////

FocusDelegate::FocusDelegate
    (view::pSprite const& owner, Input const* input, FSTATE const& state, bool const& manual)
    :owner_(owner), subscribed_input_(input), subscribed_state_(state), manual_(manual), cb_(0)
{
}

FocusDelegate& FocusDelegate::operator =(function<void(view::pSprite&, int, int)> const& cb)
{
    if( cb_ == 0 && !manual_ ) {
        EventDispatcher::i().
            subscribe_focus_event(&cb_, owner_.lock(), subscribed_input_, subscribed_state_);
    }
    cb_ = cb;
    return *this;
}

Input const* FocusDelegate::subscribed_input() const { return subscribed_input_; }
FSTATE       FocusDelegate::subscribed_state() const { return subscribed_state_; }

void FocusDelegate::call_manually() {
    if( cb_ != 0 && manual_ ) {
        int x = subscribed_input_->cursor().x();
        int y = subscribed_input_->cursor().y();
        if( view::pSprite sv = owner_.lock() )
            cb_(sv, x, y);
    }
}
