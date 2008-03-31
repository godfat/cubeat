#ifndef _SHOOTING_CUBES_CTRL_CALLBACK_DELEGATE_
#define _SHOOTING_CUBES_CTRL_CALLBACK_DELEGATE_

#include "ButtonEnum.hpp"
#include "all_fwd.hpp"
#include <tr1/functional>

namespace psc {
namespace ctrl{

class Button;

class CallbackDelegate
{
public:
    CallbackDelegate(view::pSprite const&, Button const*, BSTATE const&);
    CallbackDelegate& operator=(std::tr1::function<void(view::pSprite&)> const&);
    CallbackDelegate& setButton(Button const*);
    void setOwner(view::pSprite const&);

private:
    view::wpSprite owner_;
    Button const* subscribed_btn_;
    BSTATE subscribed_state_;
};
}
}

#endif // _SHOOTING_CUBES_CTRL_CALLBACK_DELEGATE_
