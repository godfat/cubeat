
#ifndef _SHOOTING_CUBES_PRESENTER_MENU_
#define _SHOOTING_CUBES_PRESENTER_MENU_

/* Currently not used. */

#include "presenter/Object.hpp"

#include "utils/ObjectPool.hpp"
#include "all_fwd.hpp"

namespace psc {

namespace presenter {

class Menu : public Object, public std::tr1::enable_shared_from_this<Menu>
{
public:
    typedef std::tr1::shared_ptr< Menu > pointer_type;
    static pointer_type create() {
        pointer_type p = utils::ObjectPool< Menu >::create();
        p->init();
        return p;
    }

    virtual void cycle();

protected:
    void init();

protected:

};

} //presenter
} //psc


#endif // _SHOOTING_CUBES_PRESENTER_MENU_
