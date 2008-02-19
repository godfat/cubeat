
#ifndef _SHOOTING_CUBES_GAME_VIEW_
#define _SHOOTING_CUBES_GAME_VIEW_

/* TODO:
   Inputs are temporarily declared in this scope,
   will be removed later. (when player class is out) */

#include "view/Object.hpp"
#include "utils/ObjectPool.hpp"
#include "all_fwd.hpp"

namespace irr {
namespace video {
class IVideoDriver;
}
}

namespace psc { namespace view {

class Game : public Object, public std::tr1::enable_shared_from_this<Game>
{
public:
    typedef std::tr1::shared_ptr< Game > pointer_type;
    static pointer_type create(pScene const& world, pScene const& gui) {
        pointer_type p = utils::ObjectPool< Game >::create();
        p->init(world, gui);
        return p;
    }

    //virtual pointer_type clone() const;
    virtual void redraw();
    virtual Game& setWorld(pScene const&);
    virtual Game& setGUI(pScene const&);
    virtual pScene& world() { return world_; }
    virtual pScene& gui()   { return gui_; }

    virtual ~Game(){}

protected:
    void init(pScene const& world, pScene const& gui);

protected:
    irr::video::IVideoDriver* driver_;

    pScene world_;
    pScene gui_;

    //these are temporarily here.
    std::vector < pSprite > cursors_;
};

}  //view
}  //psc

#endif
