#ifndef _SHOOTING_CUBES_CIRCLING_ANIMATOR_
#define _SHOOTING_CUBES_CIRCLING_ANIMATOR_

#include "view/detail/CustomAnimator.hpp"

//for debug & warning purpose
#include <iostream>

namespace irr{

namespace scene{

template <template <class> class Eq, class Acc>
class CirclingAnimator : public
    CustomAnimator<Eq, psc::accessor::Accessor<core::vector2df, Acc::TYPE> >
{
    typedef CustomAnimator
        <Eq, psc::accessor::Accessor
            <core::vector2df, Acc::TYPE> >  CirclingAnimatorBase;
    typedef typename Acc::value_type        T;

public:
    //PolarCoord (T): vector2::X = radius, vector2::Y = angle
    CirclingAnimator(ISceneManager* smgr,
                     core::vector2df const& start, core::vector2df const& end,
                     u32 const& duration, core::vector2df const& center, //specialize for 2D circle
                     int const& loop = 0, std::tr1::function<void()>const& cb = 0,
                     s32 const& delayTime = 0)
        :CirclingAnimatorBase(smgr, start, end, duration, loop, cb, delayTime)
    {
        center_   = core::vector2df(center.X, center.Y);
        rotation_ = core::vector3df(-90, 0, 0);
    }

    //specialize for 3D circle
    CirclingAnimator(ISceneManager* smgr,
                     core::vector2df const& start, core::vector2df const& end,
                     u32 const& duration, core::vector3df const& center, //specialize for 3D circle
                     core::vector3df const& rotation = core::vector3df(0),
                     int const& loop = 0, std::tr1::function<void()>const& cb = 0,
                     s32 const& delayTime = 0)
        :CirclingAnimatorBase(smgr, start, end, duration, loop, cb, delayTime),
         center_(center), rotation_(rotation)
    {
    }

    virtual ~CirclingAnimator(){}

    //! animates a scene node
    virtual void animateNode(ISceneNode* node, u32 timeMs) {
        if ( !node ) return;
        if ( static_cast<s32>(timeMs) < this->startTime_ ) return;

        if( timeMs - this->startTime_ >= this->duration_ ) {
            /* we can add periodic callback here.
            if( periodic_cb ) periodic_callback(); */
            this->startTime_ = timeMs; //or should be += duration_ ?
            if( this->loop_ == 0 ) {
                core::vector2df const& pos =
                    Eq<core::vector2df>::calculate(1, this->start_, this->distance_, 1, node);
                calculateCircle(node, pos, center_);
                this->smgr_->addToAnimatorDeletionQueue(this, node);
                if( this->cb_ ) this->cb_();
                return;
            }
            else if( this->loop_ > 0 )
                this->loop_ -= 1;
        }

        core::vector2df const& pos = this->updatePos(node, timeMs);
        calculateCircle(node, pos, center_);
    }

protected:
    inline void calculateCircle(ISceneNode* node, core::vector2df const& pos, core::vector2df const&) {
        float const& radius = pos.X;
        float const& arc    = pos.Y / 180.0f * irr::core::PI;
        core::vector2df circle(radius * cosf(arc), - radius * sinf(arc));
        core::vector2df center2d( center_.X, center_.Y );
        Acc::set(node, center2d + circle);
    }

    inline void calculateCircle(ISceneNode* node, core::vector2df const& pos, core::vector3df const&) {
        float const& radius = pos.X;
        float const& arc    = pos.Y / 180.0f * irr::core::PI;
        core::vector3df circle(radius * cosf(arc), 0, radius * sinf(arc));
        circle.rotateXYBy( rotation_.Z, core::vector3df(0,0,0));
        circle.rotateYZBy( rotation_.X, core::vector3df(0,0,0));
        circle.rotateXZBy( rotation_.Y, core::vector3df(0,0,0));
        Acc::set(node, center_ + circle);
    }

protected:
    T center_;
    core::vector3df rotation_;
};

} // scene
} // irr

#endif
