#ifndef _SHOOTING_CUBES_SPRITE_VIEW_
#define _SHOOTING_CUBES_SPRITE_VIEW_

#include "view/Object.hpp"
#include "utils/ObjectPool.hpp"
#include "all_fwd.hpp"

namespace psc {

namespace ctrl {
class Button;
}

namespace view {

class Sprite;

/* TODO:
   This thing should be "upgrade" to Object level.
   It's quite critical to the view hierarchy.     */

class CallbackDelegate
{
public:
    CallbackDelegate(){}
    CallbackDelegate& operator=(std::tr1::function<void(pSprite&)> const&);
    CallbackDelegate& setButton(ctrl::Button const*);
    void setOwner(pSprite const&);

private:
    ctrl::Button const* subscribed_btn_;
    wpSprite owner_;
};

class Sprite : public Object, public std::tr1::enable_shared_from_this<Sprite>
{
public:
    typedef std::tr1::shared_ptr< Sprite > pointer_type;
    static pointer_type create(std::string const& name, pObject const& parent) {
        return ObjectPool< Sprite >::create(name)->init(parent);
    }

    Sprite(std::string const& name):Object(name){}

    virtual Sprite* clone() const;
    virtual Sprite& moveTo(int,int);
    virtual Sprite& setDepth(float);
    virtual Sprite& moveTween(int,int,int,std::tr1::function<void()> cb = 0, int delay = 0);

    CallbackDelegate& onPress(ctrl::Button const*);
  /*CallbackDelegate& onRelease(Button const*);
    CallbackDelegate& onUp(Button const*);
    CallbackDelegate& onDown(Button const*);*/ //draft, dont use just yet

    virtual ~Sprite();

protected:
    pointer_type init(pObject const&);

protected:
    CallbackDelegate press_;
  /*CallbackDelegate release_;
    CallbackDelegate up_;
    CallbackDelegate down_; */ //draft, dont use just yet
};

} //view
} //psc

#endif
