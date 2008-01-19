#ifndef _SHOOTING_CUBES_BUTTON_VIEW_
#define _SHOOTING_CUBES_BUTTON_VIEW_

#include "view/Sprite.hpp"

#include "utils/ObjectPool.hpp"
#include <boost/tr1/memory.hpp>

namespace psc {

namespace view {

/* This class is nullified. Functionality moved to Sprite class */

class Button : public Sprite
{
public:
    typedef std::tr1::shared_ptr< Button > pointer_type;
    static pointer_type create(std::string const& name, pObject const& parent) {
        pointer_type p = psc::ObjectPool< Button >::create(name);
        p->init(parent);
        return p;
    }

    Button(std::string const& name):Sprite(name){}
    virtual Button* clone() const;

    virtual ~Button();

protected:
    void init(pObject const&);

private:

};

typedef Button::pointer_type pButton;

}  //view
}  //psc

#endif
