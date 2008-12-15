#ifndef _SHOOTING_CUBES_BUTTON_VIEW_
#define _SHOOTING_CUBES_BUTTON_VIEW_

#include "view/Sprite.hpp"

namespace ctrl {
class Button;
}

namespace view {

class Scene;

//This thing should be "upgrade" to Object level. 
//It's quite critical to the view hierarchy.
class CallbackDelegate {
public:
    CallbackDelegate(Sprite* owner):owner_(owner){}
    CallbackDelegate& operator=(boost::function<void(Sprite*)> const&);
    CallbackDelegate& setButton(ctrl::Button const*);
private:
    ctrl::Button const* subscribed_btn_;
    Sprite* owner_;
};

class Button : public Sprite
{
public:
    //Button(Scene const* parent);
    Button():press_(this)//, release_(this), up_(this), down_(this)
        {}

    virtual Button* clone() const;

    CallbackDelegate& onPress(ctrl::Button const*);

  /*CallbackDelegate& onRelease(Button const*);
    CallbackDelegate& onUp(Button const*);
    CallbackDelegate& onDown(Button const*);*/ //draft, dont use just yet

    virtual ~Button();

private:
    CallbackDelegate press_;

  /*CallbackDelegate release_;
    CallbackDelegate up_;
    CallbackDelegate down_; */ //draft, dont use just yet 
};

}  //view

#endif
