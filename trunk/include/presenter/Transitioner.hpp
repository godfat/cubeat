#ifndef _SHOOTING_CUBES_PRESENTER_TRANSITIONER_
#define _SHOOTING_CUBES_PRESENTER_TRANSITIONER_

/* Transitioner:
   This presenter will do all the transitioning handling
   in this game. So, before a major presenter go away and
   another comes off, Transitioner take over temporarily.
   Includes loading progress, menu to game or game to menu
   transitions.

   It always cycles after the master presenter (like game),
   and this presenter don't interact with user in most cases. */

#include "presenter/Object.hpp"

#include "utils/ObjectPool.hpp"
#include <boost/tr1/memory.hpp>

namespace psc {

namespace view {
class Scene;
class Menu;
typedef std::tr1::shared_ptr<Scene> pScene;
typedef std::tr1::shared_ptr<Menu> pMenu;
}

namespace presenter {

class Transitioner : public Object, public std::tr1::enable_shared_from_this<Object>
{
public:
    typedef std::tr1::shared_ptr< Transitioner > pointer_type;
    typedef std::tr1::weak_ptr< Transitioner > wpointer_type;
    static pointer_type create() {
        pointer_type p = psc::ObjectPool< Transitioner >::create();
        p->init();
        return p;
    }

    virtual Transitioner& setLoadingBar(int const& percent);

    virtual void cycle();

protected:
    void init();

protected:
    view::pScene transition_scene_;
    view::pMenu loading_menu_;

};

typedef Transitioner::pointer_type pTransitioner;
typedef Transitioner::wpointer_type wpTransitioner;

} //presenter
} //psc

#endif // _SHOOTING_CUBES_PRESENTER_TRANSITIONER_