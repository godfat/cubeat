
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

class Game : public Object
{
public:
    typedef std::tr1::shared_ptr< Game > pointer_type;
    static pointer_type create(pScene const& world, pScene const& gui) {
        //return utils::ObjectPool< Game >::create()->init(world, gui);
        return pointer_type(new Game)->init(world, gui);
    }

    //virtual pointer_type clone() const;
    virtual void redraw();
    virtual Game& setWorld(pScene const&);
    virtual Game& setGUI(pScene const&);
    virtual pScene& world() { return world_; }
    virtual pScene& gui()   { return gui_; }

    virtual ~Game(){}

protected:
    pointer_type init(pScene const& world, pScene const& gui);

protected:
    pScene world_;
    pScene gui_;

    //these are temporarily here.
    std::vector < pSprite > cursors_;
};

}  //view
}  //psc

#endif
