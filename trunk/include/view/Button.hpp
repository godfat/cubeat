#ifndef _SHOOTING_CUBES_BUTTON_VIEW_
#define _SHOOTING_CUBES_BUTTON_VIEW_

#include "view/Sprite.hpp"

#include "utils/ObjectPool.hpp"
#include <boost/tr1/memory.hpp>

namespace psc {

namespace ctrl {
class Button;
}

namespace view {

class Scene;
class Button;
//This thing should be "upgrade" to Object level. 
//It's quite critical to the view hierarchy.
class CallbackDelegate {
    typedef std::tr1::shared_ptr<Button> pButton;
public:
    CallbackDelegate(){}
    CallbackDelegate& operator=(boost::function<void(pSprite&)> const&);
    CallbackDelegate& setButton(ctrl::Button const*);
    void setOwner(pButton const);
private:
    ctrl::Button const* subscribed_btn_;
    wpSprite owner_;
};

class Button : public Sprite
{
public:
    typedef std::tr1::shared_ptr< Button > pointer_type;
    static pointer_type create(pObject const& parent) {
        pointer_type p = psc::ObjectPool< Button >::create();
        p->init(parent);
        return p;
    }

    Button(){}
    virtual Button* clone() const;

    CallbackDelegate& onPress(ctrl::Button const*);

  /*CallbackDelegate& onRelease(Button const*);
    CallbackDelegate& onUp(Button const*);
    CallbackDelegate& onDown(Button const*);*/ //draft, dont use just yet

    virtual ~Button();

protected:
    void init(pObject const&);

private:
    CallbackDelegate press_;

  /*CallbackDelegate release_;
    CallbackDelegate up_;
    CallbackDelegate down_; */ //draft, dont use just yet 
};

typedef Button::pointer_type pButton;

}  //view
}  //psc

#endif
