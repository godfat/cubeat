
#ifndef _SHOOTING_CUBES_SPECIAL_EFFECTS_VIEW_
#define _SHOOTING_CUBES_SPECIAL_EFFECTS_VIEW_

/* TODO:
   I am going to do some dirty work to get Special Effects (particles)
   working. so the code here will not be so extensible and flexible,
   and they will mainly depend on irrlicht's particle system without wrapping.

   This class will be a singleton, for now, and for the ease of use. */

#include "all_fwd.hpp"
#include "data/BasicViewTypes.hpp"

namespace psc { namespace view {

class SFX
{
public:
    static SFX& i() {
        static SFX singleton;
        return singleton;
    }

    ~SFX();

    void normal_weapon_vfx(view::pScene&, vec2 const&);
    void weapon_vfx1(view::pScene&, vec2 const&);
    void weapon_vfx2(view::pScene&, vec2 const&);
    void weapon_vfx3(view::pScene&, vec2 const&);

protected:

private:
    SFX();
};

} //view
} //psc

#endif
