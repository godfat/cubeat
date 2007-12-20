#ifndef _SHOOTING_CUBES_SPRITE_VIEW_
#define _SHOOTING_CUBES_SPRITE_VIEW_

#include "ObjectView.hpp"
#include "CustomAnimator.hpp"
#include "boost/tr1/functional.hpp"
#include <iostream>

class GUIView;

class SpriteView : public ObjectView
{
public:
    SpriteView(GUIView const* parent);

    virtual SpriteView* clone() const;

    virtual void moveTo(int,int);
    void moveTween(int,int,int,std::tr1::function<void()> cb = 0, int delay = 0);

    //add Tween interface here?
    template <template <class> class Eq, class Accessor>
    void tween(typename Accessor::value_type const& start,
               typename Accessor::value_type const& end,
               int                           const& duration,
               bool                          const& loop = true,
               std::tr1::function<void()>              cb = 0,
               int                           const& delay = 0) 
    {
        // std::cout << "before new anim" << std::endl;
        irr::scene::ISceneNodeAnimator* anim = 
            new irr::scene::CustomAnimator<Eq, Accessor>
                (start, end, duration, loop, cb, delay);
        // std::cout << "before add" << std::endl;
        body_->addAnimator( anim );
        // std::cout << "after add" << std::endl;
        anim->drop();
        // std::cout << "after drop" << std::endl;
    }

    template <template <class> class Eq, class Accessor>
    void tween(typename Accessor::value_type const& end,
               int                           const& duration,
               bool                          const& loop = true,
               std::tr1::function<void()>              cb = 0,
               int                           const& delay = 0)
    {
        // std::cout << "the body_ address: " << body_ << std::endl;
        // std::cout << "before new anim" << std::endl;
        irr::scene::ISceneNodeAnimator* anim = 
            new irr::scene::CustomAnimator<Eq, Accessor>
                (body_, end, duration, loop, cb, delay);
        // std::cout << "before add" << std::endl;
        body_->addAnimator( anim );
        // std::cout << "after add" << std::endl;
        anim->drop();
        // std::cout << "after drop" << std::endl;
    }

    virtual ~SpriteView();

protected:
    irr::video::SMaterial mat_;
private:
    // SpriteView(SpriteView const&){std::cout << "!!!!!!ERRRRRROORR!!!!!" << std::endl;}
};

#endif