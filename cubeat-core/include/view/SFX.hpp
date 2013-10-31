
#ifndef _SHOOTING_CUBES_SPECIAL_EFFECTS_VIEW_
#define _SHOOTING_CUBES_SPECIAL_EFFECTS_VIEW_

/* TODO:
   I am going to do some dirty work to get Special Effects (particles)
   working. so the code here will not be so extensible and flexible,
   and they will mainly depend on irrlicht's particle system without wrapping.

   This class will be a singleton, for now, and for the ease of use. */

#include "all_fwd.hpp"
#include "data/BasicViewTypes.hpp"
#include "data/TAnimatorParam.hpp"
#include "view/Object.hpp"
#include <list>

namespace psc { namespace view {

namespace fuckyou {
void compose(std::tr1::function<void()> f, std::tr1::function<void()> g);
}

class SFX
{
public:
    typedef std::list< view::pObject >       FXObjList;
    typedef std::list< FXObjList::iterator > FXObjRemoval;

    static SFX& i() {
        static SFX singleton;
        return singleton;
    }

    ~SFX();

    void normal_weapon_vfx(view::pScene&, vec2 const&);
    void weapon_vfx1(view::pScene&, vec2 const&);
    void weapon_vfx2(view::pScene&, vec2 const&);
    void weapon_vfx3(view::pScene&, vec2 const&);
    void cube_explode(view::pSprite);
    SFX& hold(view::pSprite, time_t duration);

    template <template <class> class Eq, class Accessor>
    void custom_effect_holder(view::pObject o, data::AnimatorParam<Eq, Accessor> p)
    {
        std::tr1::function<void()> end_cb  = p.cb();

        effects_holder_.push_front(o);
        std::tr1::function<void()> cleanup = bind(&SFX::clear_obj, this, effects_holder_.begin());

        if( p.cb() ) {
            p.cb( std::tr1::bind(&fuckyou::compose, end_cb, cleanup) );
        } else {
            p.cb(cleanup);
        }
        o->tween(p);
    }

    void init_textures(view::pScene&);
    void cleanup();

protected:
    void clear_obj(FXObjList::iterator);

    FXObjList    effects_holder_;
    FXObjRemoval effects_to_be_deleted_;

private:
    SFX();
};

} //view
} //psc

#endif
