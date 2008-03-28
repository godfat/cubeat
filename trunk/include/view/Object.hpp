#ifndef _SHOOTING_CUBES_OBJECT_VIEW_
#define _SHOOTING_CUBES_OBJECT_VIEW_

/* TODO:
   We might need a queue and loop-finite-times features to tween interface
   will be implemented when needed. */

#include "view/detail/CustomAnimator.hpp"
#include "view/detail/WaypointAnimator.hpp"
#include "view/detail/CirclingAnimator.hpp"
#include "view/detail/SplineAnimator.hpp"
#include "view/detail/SpeedFuncAnimator.hpp"
#include "utils/ObjectPool.hpp"
#include "data/BasicViewTypes.hpp"
#include "all_fwd.hpp"
#include <tr1/functional>
#include <string>
#include <vector>

namespace irr{
namespace scene{
class ISceneManager;
class ISceneNode;
}
}

namespace psc { namespace view {

class Object
{
public:
    typedef std::tr1::shared_ptr< Object > pointer_type;

    static pointer_type create(pointer_type const& parent) {
        pointer_type p = utils::ObjectPool< Object >::create("");
        p->init(parent);
        return p;
    }

    Object(std::string const& name = "");

    virtual Object* clone() const;
    virtual pScene  scene() const;
    virtual Object& moveTo(int,int,int);

    virtual irr::scene::ISceneNode* body() const;

    template <class Accessor>
    Object& set(typename Accessor::value_type const& value) {
        Accessor::set(body_, value);
        return *this;
    }

    template <class Accessor>
    typename Accessor::value_type get() const {
        typename Accessor::value_type out;
        Accessor::get(body_, out);
        return out;
    }

    template <template <class> class Eq, class Accessor>
    Object& tween(typename Accessor::value_type const& start,
                  typename Accessor::value_type const& end,
                  unsigned int                  const& duration,
                  int                           const& loop = 0,
                  std::tr1::function<void()>    const& cb = 0,
                  int                           const& delay = 0)
    {
        if( duration == 0 ) return *this;
        irr::scene::ISceneNodeAnimator* anim =
            new irr::scene::CustomAnimator<Eq, Accessor>
                (smgr_, start, end, duration, loop, cb, delay);
        body_->addAnimator( anim );
        anim->drop();
        return *this;
    }

    template <template <class> class Eq, class Accessor>
    Object& tween(typename Accessor::value_type const& end,
                  unsigned int                  const& duration,
                  int                           const& loop = 0,
                  std::tr1::function<void()>    const& cb = 0,
                  int                           const& delay = 0)
    {
        if( duration == 0 ) return *this;
        typename Accessor::value_type start = typename Accessor::value_type();
        Accessor::get(body_, start);
        tween<Eq, Accessor>(start, end, duration, loop, cb, delay);
        return *this;
    }

    template <template <class> class Eq, class Accessor>
    Object& tween(typename Accessor::value_type const& start,
                  typename Accessor::value_type const& end,
                  std::tr1::function<float()>   const& speedfunc,
                  int                           const& loop = 0,
                  std::tr1::function<void()>    const& cb = 0,
                  int                           const& delay = 0)
    {
        if( !speedfunc ) return *this;
        irr::scene::ISceneNodeAnimator* anim =
            new irr::scene::SpeedFuncAnimator<Eq, Accessor>
                (smgr_, start, end, speedfunc, loop, cb, delay);
        body_->addAnimator( anim );
        anim->drop();
        return *this;
    }

    template <template <class> class Eq, class Accessor>
    Object& tween(typename Accessor::value_type const& end,
                  std::tr1::function<float()>   const& speedfunc,
                  int                           const& loop = 0,
                  std::tr1::function<void()>    const& cb = 0,
                  int                           const& delay = 0)
    {
        if( !speedfunc ) return *this;
        typename Accessor::value_type start = typename Accessor::value_type();
        Accessor::get(body_, start);
        tween<Eq, Accessor>(start, end, speedfunc, loop, cb, delay);
        return *this;
    }

    template <template <class> class Eq, class Accessor>
    Object& tween(std::vector
                  <typename Accessor::value_type> const& waypoints,
                  unsigned int                    const& duration,
                  bool                            const& closed = false,
                  int                             const& loop = 0,
                  std::tr1::function<void()>      const& cb = 0,
                  int                             const& delay = 0,
                  std::vector<float>              const& tensions = std::vector<float>())
    {
        if( duration == 0 ) return *this;
        irr::scene::ISceneNodeAnimator* anim;
        if( tensions.size() != 0 ) {
            anim = new irr::scene::SplineAnimator<Eq, Accessor>
                (smgr_, waypoints, duration, loop, cb, delay, tensions, closed);
        }
        else {
            anim = new irr::scene::WaypointAnimator<Eq, Accessor>
                (smgr_, waypoints, duration, loop, cb, delay, closed);
        }
        body_->addAnimator( anim );
        anim->drop();
        return *this;
    }

    template <template <class> class Eq, class Accessor>
    Object& orbit(vec2                       const& center, //specialize for 2D circle
                  vec2                       const& start,  //2D polar coordinate
                  vec2                       const& end,    //2D polar coordinate
                  unsigned int               const& duration,
                  int                        const& loop = 0,
                  std::tr1::function<void()> const& cb = 0,
                  int                        const& delayTime = 0)
    {
        if( duration == 0 ) return *this;
        irr::scene::ISceneNodeAnimator* anim =
            new irr::scene::CirclingAnimator<Eq, Accessor>
                (smgr_, start, end, duration, center, loop, cb, delayTime);
        body_->addAnimator( anim );
        anim->drop();
        return *this;
    }

    template <template <class> class Eq, class Accessor>
    Object& orbit(vec3                       const& center, //specialize for 3D circle
                  vec2                       const& start,  //2D polar coordinate
                  vec2                       const& end,    //2D polar coordinate
                  vec3                       const& rotation,//rotation along 3 axis
                  unsigned int               const& duration,
                  int                        const& loop = 0,
                  std::tr1::function<void()> const& cb = 0,
                  int                        const& delayTime = 0)
    {
        if( duration == 0 ) return *this;
        irr::scene::ISceneNodeAnimator* anim =
            new irr::scene::CirclingAnimator<Eq, Accessor>
                (smgr_, start, end, duration, center, rotation, loop, cb, delayTime);
        body_->addAnimator( anim );
        anim->drop();
        return *this;
    }

    virtual ~Object();

protected:
    void setupSceneAndManager(pointer_type const& parent);
    void init(pointer_type const& parent);

protected:
    irr::scene::ISceneManager* smgr_;
    irr::scene::ISceneNode*    body_;

    wpScene scene_;

    std::string name_;
};

} //view
} //psc

#endif
