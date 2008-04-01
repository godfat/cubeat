#ifndef _SHOOTING_CUBES_CTRL_CALLBACK_DELEGATE_
#define _SHOOTING_CUBES_CTRL_CALLBACK_DELEGATE_

#include "ButtonEnum.hpp"
#include "all_fwd.hpp"
#include <tr1/functional>

namespace psc {
namespace ctrl{

class Button;
class Input;

class CallbackDelegate
{
public:
    CallbackDelegate(view::pSprite const&, Button const*, BSTATE const&);
    CallbackDelegate& operator=(std::tr1::function<void(view::pSprite&)> const&);
    Button const* subscribed_btn()   const;
    BSTATE        subscribed_state() const;

private:
    view::wpSprite owner_;
    Button const*  subscribed_btn_;
    BSTATE         subscribed_state_;

    std::tr1::function<void(view::pSprite&)> cb_;
};

class FocusDelegate
{
public:
    FocusDelegate(view::pSprite const&, Input const*, FSTATE const&);
    FocusDelegate& operator=(std::tr1::function<void(view::pSprite&, int x, int y)> const&);
    Input const* subscribed_input() const;
    FSTATE       subscribed_state() const;
private:
    view::wpSprite owner_;
    Input const*   subscribed_input_;
    FSTATE         subscribed_state_;

    std::tr1::function<void(view::pSprite&, int, int)> cb_;
};

} //ctrl
} //psc

#endif // _SHOOTING_CUBES_CTRL_CALLBACK_DELEGATE_
