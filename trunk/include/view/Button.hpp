#ifndef _SHOOTING_CUBES_BUTTON_VIEW_
#define _SHOOTING_CUBES_BUTTON_VIEW_

#include "view/Sprite.hpp"

#include "utils/ObjectPool.hpp"
#include "all_fwd.hpp"

namespace psc {

namespace view {

/* This class is nullified. Functionality moved to Sprite class */

class Button : public Sprite
{
public:
    typedef std::tr1::shared_ptr< Button > pointer_type;
    static pointer_type create(std::string const& name,
                               pObject const& parent,
                               int const& w = 100,
                               int const& h = 100,
                               bool const& center = false)
    {
        return utils::ObjectPool< Button >::create(name, center)->init(parent, w, h);
    }

    Button(std::string const& name, bool const& center)
        :Sprite(name, center){}

    virtual Button* clone() const;

    virtual ~Button();

protected:
    pointer_type init(pObject const&, int const&, int const&);

private:

};

}  //view
}  //psc

#endif
