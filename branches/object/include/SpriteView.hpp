#ifndef _SHOOTING_CUBES_SPRITE_VIEW_
#define _SHOOTING_CUBES_SPRITE_VIEW_

#include "ObjectView.hpp"
#include "CustomAnimator.hpp"
#include "boost/function.hpp"

class GUIView;

class SpriteView : public ObjectView
{
public:
    SpriteView(GUIView const* parent);

    virtual SpriteView* clone() const;

    virtual void moveTo(int,int);
    void moveTween(int,int,int,boost::function<void()> cb = 0, int delay = 0);

    //add Tween interface here?
    template <template <class> class Eq, class Accessor>
    void tween(typename Accessor::value_type const& start,
               typename Accessor::value_type const& end,
               int                           const& duration,
               bool                          const& loop = true,
               boost::function<void()>              cb = 0,
               int                           const& delay = 0) 
    {
        irr::scene::ISceneNodeAnimator* anim = 
            new irr::scene::CustomAnimator<Eq, Accessor>
                (body_, start, end, duration, loop, cb, delay);
    }

    template <template <class> class Eq, class Accessor>
    void tween(typename Accessor::value_type const& end,
               int                           const& duration,
               bool                          const& loop = true,
               boost::function<void()>              cb = 0,
               int                           const& delay = 0)
    {
        irr::scene::ISceneNodeAnimator* anim = 
            new irr::scene::CustomAnimator<Eq, Accessor>
                (body_, end, duration, loop, cb, delay);
    }

    virtual ~SpriteView();

protected:
    irr::video::SMaterial mat_;
};

#endif