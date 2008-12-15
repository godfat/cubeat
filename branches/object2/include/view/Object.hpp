#ifndef _SHOOTING_CUBES_OBJECT_VIEW_
#define _SHOOTING_CUBES_OBJECT_VIEW_

/* TODO:
   We might need a queue and loop-finite-times features to tween interface
   will be implemented when needed. */

#include "CustomAnimator.hpp"

#include <boost/tr1/functional.hpp>

namespace irr{
namespace scene{
class ISceneManager;
class ISceneNode;
}
}

namespace view {

class Object
{
public:
    //Object(Object const* parent = 0);
    Object();
    virtual void init(Object const* parent = 0);

    virtual Object* clone() const;
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
                      int                           const& duration,
                      bool                          const& loop = true,
                      std::tr1::function<void()>           cb = 0,
                      int                           const& delay = 0)
    {
        irr::scene::ISceneNodeAnimator* anim =
            new irr::scene::CustomAnimator<Eq, Accessor>
                (start, end, duration, loop, cb, delay);
        body_->addAnimator( anim );
        anim->drop();
        return *this;
    }

    template <template <class> class Eq, class Accessor>
    Object& tween(typename Accessor::value_type const& end,
                      int                           const& duration,
                      bool                          const& loop = true,
                      std::tr1::function<void()>           cb = 0,
                      int                           const& delay = 0)
    {
        typename Accessor::value_type start;
        Accessor::get(body_, start);
        tween<Eq, Accessor>(start, end, duration, loop, cb, delay);
        return *this;
    }

    virtual ~Object();

protected:
    irr::scene::ISceneManager* smgr_;
    irr::scene::ISceneNode* body_;
};

} //view

#endif
