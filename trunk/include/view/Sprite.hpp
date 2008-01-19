#ifndef _SHOOTING_CUBES_SPRITE_VIEW_
#define _SHOOTING_CUBES_SPRITE_VIEW_

#include "view/Object.hpp"
#include "utils/ObjectPool.hpp"
#include <boost/tr1/memory.hpp>

namespace psc {

namespace ctrl {
class Button;
}

namespace view {

class Sprite;

/* TODO:
   This thing should be "upgrade" to Object level.
   It's quite critical to the view hierarchy.     */

class CallbackDelegate {
    typedef std::tr1::shared_ptr<Sprite> pSprite;
    typedef std::tr1::weak_ptr<Sprite> wpSprite;
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
        pointer_type p = psc::ObjectPool< Sprite >::create(name);
        p->init(parent);
        return p;
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
    void init(pObject const&);

protected:
    CallbackDelegate press_;
  /*CallbackDelegate release_;
    CallbackDelegate up_;
    CallbackDelegate down_; */ //draft, dont use just yet

};

typedef Sprite::pointer_type pSprite;
typedef std::tr1::weak_ptr<Sprite> wpSprite;

} //view
} //psc

#endif
