
#ifndef _SHOOTING_CUBES_MENU_VIEW_
#define _SHOOTING_CUBES_MENU_VIEW_

#include "view/Object.hpp"
#include "utils/ObjectPool.hpp"
#include <boost/tr1/memory.hpp>

namespace psc { namespace view {

class Menu : public Object, public std::tr1::enable_shared_from_this<Menu>
{
public:
    typedef std::tr1::shared_ptr< Menu > pointer_type;
    static pointer_type create(pObject const& parent) {
        pointer_type p = psc::ObjectPool< Menu >::create();
        p->init(parent);
        return p;
    }

    Menu(){}
    
    virtual Menu* clone() const;

    virtual ~Menu(){}

protected:
    void init(pObject const&);

};

typedef Menu::pointer_type pMenu;

} //view
} //psc

#endif
