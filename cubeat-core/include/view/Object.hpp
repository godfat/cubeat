#ifndef _SHOOTING_CUBES_OBJECT_VIEW_
#define _SHOOTING_CUBES_OBJECT_VIEW_

#include "view/detail/CustomAnimator.hpp"
#include "view/detail/WaypointAnimator.hpp"
#include "view/detail/CirclingAnimator.hpp"
#include "view/detail/SplineAnimator.hpp"
#include "view/detail/SpeedFuncAnimator.hpp"
#include "utils/ObjectPool.hpp"
#include "data/BasicViewTypes.hpp"
#include "data/TAnimatorParam.hpp"
#include "all_fwd.hpp"
#include <boost/tr1/functional.hpp>
#include <string>
#include <vector>

namespace irr{
namespace scene{
class ISceneManager;
class ISceneNode;
}
}

namespace psc { namespace view {

class Object : public std::tr1::enable_shared_from_this<Object>
{
public:
    typedef std::tr1::shared_ptr< Object > pointer_type;

    static pointer_type create(pointer_type const& parent) {
        pointer_type p = utils::ObjectPool< Object >::create("");
        p->init(parent);
        return p;
    }

    Object(std::string const& name = "");

    std::string     getSceneName() const;
    virtual Object* clone() const;
    virtual pScene  scene() const;
    virtual Object& moveTo(int,int,int);
    virtual Object& setTexture(std::string const& path);
    virtual Object& setPickable(bool const&);
    virtual Object& setLighting(bool const&);

    virtual irr::scene::ISceneNode* body() const;

    template <class Accessor>
    Object& set(typename Accessor::value_type const& value) {
        Accessor::set(body_, value);
        return *this;
    }

    template <class Accessor>
    typename Accessor::value_type get() const {
        typename Accessor::value_type out = typename Accessor::value_type();
        Accessor::get(body_, out);
        return out;
    }

    ////////////////// tweening with parameter construct /////////////////////

    template <template <class> class Eq, class Accessor>
    Object& tween(data::AnimatorParam<Eq, Accessor> const& p)
    {
        if( p.speedFuncValid() ) {
            if( p.startValid() )
                tween<Eq, Accessor>(p.start(), p.end(), p.speedfunc(), p.loop(), p.cb(), p.delay());
            else
                tween<Eq, Accessor>(p.end(), p.speedfunc(), p.loop(), p.cb(), p.delay());
        } else {
            if( p.startValid() )
                tween<Eq, Accessor>(p.start(), p.end(), p.duration(), p.loop(), p.cb(), p.delay());
            else
                tween<Eq, Accessor>(p.end(), p.duration(), p.loop(), p.cb(), p.delay());
        }
        return *this;
    }

    template <template <class> class Eq, class Accessor>
    Object& tween(data::WaypointParam<Eq, Accessor> const& p)
    {
        tween<Eq, Accessor>
        (p.waypoints(), p.duration(), p.closed(), p.loop(), p.cb(), p.delay(), p.tensions());
        return *this;
    }

    template <template <class> class Eq, class Accessor>
    Object& tween(data::CirclingParam<Eq, Accessor> const& p)
    {
        tween<Eq, Accessor>(p, typename data::CirclingParam<Eq, Accessor>::T());
        return *this;
    }

    ////////////// queued animation with parameter construct /////////////////

    template <template <class> class Eq, class Accessor>
    Object& queue(data::AnimatorParam<Eq, Accessor> const& p)
    {
        if( p.speedFuncValid() ) {
            if( p.startValid() )
                queue<Eq, Accessor>(p.start(), p.end(), p.speedfunc(), p.loop(), p.cb(), p.delay());
            else
                queue<Eq, Accessor>(p.end(), p.speedfunc(), p.loop(), p.cb(), p.delay());
        } else {
            if( p.startValid() )
                queue<Eq, Accessor>(p.start(), p.end(), p.duration(), p.loop(), p.cb(), p.delay());
            else
                queue<Eq, Accessor>(p.end(), p.duration(), p.loop(), p.cb(), p.delay());
        }
        return *this;
    }

    template <template <class> class Eq, class Accessor>
    Object& queue(data::WaypointParam<Eq, Accessor> const& p)
    {
        queue<Eq, Accessor>
        (p.waypoints(), p.duration(), p.closed(), p.loop(), p.cb(), p.delay(), p.tensions());
        return *this;
    }

    template <template <class> class Eq, class Accessor>
    Object& queue(data::CirclingParam<Eq, Accessor> const& p)
    {
        queue<Eq, Accessor>(p, typename data::CirclingParam<Eq, Accessor>::T());
        return *this;
    }

    //// Original tweening methods (for compatibility with client code): ////

#define START         typename Accessor::value_type const& start
#define END           typename Accessor::value_type const& end
#define DURATION      unsigned int                  const& duration
#define SPEEDFUNC     std::tr1::function<float()>   const& speedfunc
#define CLOSED        bool                          const& closed = false
#define TENSIONS      std::vector<float>            const& tensions = std::vector<float>()
#define VEC2START     vec2                          const& start
#define VEC2END       vec2                          const& end
#define VEC2CENTER    vec2                          const& center
#define VEC3CENTER    vec3                          const& center
#define VEC3ROTATION  vec3                          const& rotation
#define WAYPOINTS     std::vector<typename Accessor::value_type> const& waypoints
#define LOOP_CB_DELAY int const& loop = 0, std::tr1::function<void()> const& cb = 0, int const& delay = 0

    // an animation which specified start and end.
    template <template <class> class Eq, class Accessor>
    Object& tween(START, END, DURATION, LOOP_CB_DELAY)
    {
        queue<Eq, Accessor>(start, end, duration, loop, cb, delay);
        startTween();
        return *this;
    }

    // an animation which specified only end, will use current status as start.
    template <template <class> class Eq, class Accessor>
    Object& tween(END, DURATION, LOOP_CB_DELAY)
    {
        typename Accessor::value_type start = typename Accessor::value_type();
        Accessor::get(body_, start);
        tween<Eq, Accessor>(start, end, duration, loop, cb, delay);
        return *this;
    }

    // an animation with speed control (start, end)
    template <template <class> class Eq, class Accessor>
    Object& tween(START, END, SPEEDFUNC, LOOP_CB_DELAY)
    {
        queue<Eq, Accessor>(start, end, speedfunc, loop, cb, delay);
        startTween();
        return *this;
    }

    // an animation with speed control (current status, end)
    template <template <class> class Eq, class Accessor>
    Object& tween(END, SPEEDFUNC, LOOP_CB_DELAY)
    {
        typename Accessor::value_type start = typename Accessor::value_type();
        Accessor::get(body_, start);
        tween<Eq, Accessor>(start, end, speedfunc, loop, cb, delay);
        return *this;
    }

    // an animation with more than 2 keypoints (waypoints).
    template <template <class> class Eq, class Accessor>
    Object& tween(WAYPOINTS, DURATION, CLOSED, LOOP_CB_DELAY, TENSIONS)
    {
        queue<Eq, Accessor>(waypoints, duration, closed, loop, cb, delay, tensions);
        startTween();
        return *this;
    }

    // orbit a center, specialized for 2D circle (start and end is polar coordinate)
    template <template <class> class Eq, class Accessor>
    Object& orbit(VEC2CENTER, VEC2START, VEC2END, DURATION, LOOP_CB_DELAY)
    {
        queue<Eq, Accessor>(center, start, end, duration, loop, cb, delay);
        startTween();
        return *this;
    }

    // orbit a center, specialized for 3D circle (start and end is polar coordinate)
    template <template <class> class Eq, class Accessor>
    Object& orbit(VEC3CENTER, VEC2START, VEC2END, VEC3ROTATION, DURATION, LOOP_CB_DELAY)
    {
        queue<Eq, Accessor>(center, start, end, rotation, duration, loop, cb, delay);
        startTween();
        return *this;
    }

    //////////////////// Queued animation methods: /////////////////////

    // an animation which specified start and end.
    template <template <class> class Eq, class Accessor>
    Object& queue(START, END, DURATION, LOOP_CB_DELAY)
    {
        if( duration == 0 ) return *this;
        std::tr1::function<void()> combo = bind(&Object::nextTween, this, cb);
        irr::scene::AnimatorBase* anim =
            new irr::scene::CustomAnimator<Eq, Accessor>
                (smgr_, start, end, duration, loop, combo, delay);
        anim_queue_.push_back( anim );
        return *this;
    }

    // an animation which specified only end, will use current status as start.
    template <template <class> class Eq, class Accessor>
    Object& queue(END, DURATION, LOOP_CB_DELAY)
    {
        if( duration == 0 ) return *this;
        typename Accessor::value_type start = typename Accessor::value_type();
        Accessor::get(body_, start);
        queue<Eq, Accessor>(start, end, duration, loop, cb, delay);
        return *this;
    }

    // an animation with speed control (start, end)
    template <template <class> class Eq, class Accessor>
    Object& queue(START, END, SPEEDFUNC, LOOP_CB_DELAY)
    {
        if( !speedfunc ) return *this;
        std::tr1::function<void()> combo = bind(&Object::nextTween, this, cb);
        irr::scene::AnimatorBase* anim =
            new irr::scene::SpeedFuncAnimator<Eq, Accessor>
                (smgr_, start, end, speedfunc, loop, combo, delay);
        anim_queue_.push_back( anim );
        return *this;
    }

    // an animation with speed control (current status, end)
    template <template <class> class Eq, class Accessor>
    Object& queue(END, SPEEDFUNC, LOOP_CB_DELAY)
    {
        if( !speedfunc ) return *this;
        typename Accessor::value_type start = typename Accessor::value_type();
        Accessor::get(body_, start);
        queue<Eq, Accessor>(start, end, speedfunc, loop, cb, delay);
        return *this;
    }

    // an animation with more than 2 keypoints (waypoints).
    template <template <class> class Eq, class Accessor>
    Object& queue(WAYPOINTS, DURATION, CLOSED, LOOP_CB_DELAY, TENSIONS)
    {
        if( duration == 0 ) return *this;
        std::tr1::function<void()> combo = bind(&Object::nextTween, this, cb);
        irr::scene::AnimatorBase* anim;
        if( tensions.size() != 0 ) {
            anim = new irr::scene::SplineAnimator<Eq, Accessor>
                (smgr_, waypoints, duration, loop, combo, delay, tensions, closed);
        }
        else {
            anim = new irr::scene::WaypointAnimator<Eq, Accessor>
                (smgr_, waypoints, duration, loop, combo, delay, closed);
        }
        anim_queue_.push_back( anim );
        return *this;
    }

    // orbit a center, specialized for 2D circle
    template <template <class> class Eq, class Accessor>
    Object& queue(VEC2CENTER, VEC2START, VEC2END, DURATION, LOOP_CB_DELAY)
    {
        if( duration == 0 ) return *this;
        std::tr1::function<void()> combo = bind(&Object::nextTween, this, cb);
        irr::scene::AnimatorBase* anim =
            new irr::scene::CirclingAnimator<Eq, Accessor>
                (smgr_, start, end, duration, center, loop, combo, delay);
        anim_queue_.push_back( anim );
        return *this;
    }

    // orbit a center, specialized for 3D circle
    template <template <class> class Eq, class Accessor>
    Object& queue(VEC3CENTER, VEC2START, VEC2END, VEC3ROTATION, DURATION, LOOP_CB_DELAY)
    {
        if( duration == 0 ) return *this;
        std::tr1::function<void()> combo = bind(&Object::nextTween, this, cb);
        irr::scene::AnimatorBase* anim =
            new irr::scene::CirclingAnimator<Eq, Accessor>
                (smgr_, start, end, duration, center, rotation, loop, combo, delay);
        anim_queue_.push_back( anim );
        return *this;
    }

    virtual Object& clearTween(accessor::AT::ATEnum const&);
    virtual Object& clearAllTween();
    Object& clearAllQueuedTween();
    Object& clearQueuedTween(accessor::AT::ATEnum const&);

#undef START
#undef END
#undef DURATION
#undef SPEEDFUNC
#undef CLOSED
#undef TENSIONS
#undef VEC2START
#undef VEC2END
#undef VEC2CENTER
#undef VEC3CENTER
#undef VEC3ROTATION
#undef WAYPOINTS
#undef LOOP_CB_DELAY

    virtual ~Object();

protected:
    void setupSceneAndManager(pointer_type const& parent);
    void init(pointer_type const& parent);
    virtual void startTween();
    void nextTween(std::tr1::function<void()>const& orig_cb);

    ///////////////// tween param matching & deducing series ///////////////////

    template <template <class> class Eq, class Accessor>
    void tween(data::CirclingParam<Eq, Accessor> const& p, vec2 const&)
    {
        orbit<Eq, Accessor>
        (p.center(), p.start(), p.end(), p.duration(), p.loop(), p.cb(), p.delay());
    }

    template <template <class> class Eq, class Accessor>
    void tween(data::CirclingParam<Eq, Accessor> const& p, vec3 const&)
    {
        orbit<Eq, Accessor>
        (p.center(), p.start(), p.end(), p.rotation(), p.duration(), p.loop(), p.cb(), p.delay());
    }

    ///////////// queued animation param matching & deducing series ///////////

    template <template <class> class Eq, class Accessor>
    void queue(data::CirclingParam<Eq, Accessor> const& p, vec2 const&)
    {
        queue<Eq, Accessor>
        (p.center(), p.start(), p.end(), p.duration(), p.loop(), p.cb(), p.delay());
    }

    template <template <class> class Eq, class Accessor>
    void queue(data::CirclingParam<Eq, Accessor> const& p, vec3 const&)
    {
        queue<Eq, Accessor>
        (p.center(), p.start(), p.end(), p.rotation(), p.duration(), p.loop(), p.cb(), p.delay());
    }

protected:
    irr::scene::ISceneManager* smgr_;
    irr::scene::ISceneNode*    body_;
    std::list< irr::scene::AnimatorBase* > anim_queue_;

    wpScene scene_;

    std::string name_;
};

} //view
} //psc

#endif
