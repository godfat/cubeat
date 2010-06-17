
#ifndef _SHOOTING_CUBES_CUSTOM_ANIMATOR_
#define _SHOOTING_CUBES_CUSTOM_ANIMATOR_

#include "ISceneNode.h"
#include "IrrDevice.hpp"
#include <tr1/functional>

#include <cmath>

namespace irr
{
namespace scene
{

class AnimatorBase : public ISceneNodeAnimator {
public:
    AnimatorBase(s32 const& delayTime = 0):delayTime_(delayTime){}
    void updateStartTime() {
        startTime_ = IrrDevice::i().d()->getTimer()->getTime() + delayTime_;
    }

    virtual AnimatorBase* clone() const = 0;
    virtual AnimatorBase* cloneWithoutCallback() const = 0;

protected:
    s32 startTime_;
    s32 delayTime_;
};

template <template<class> class Eq, class Accessor>
class CustomAnimator : public AnimatorBase
{
protected:
    typedef std::tr1::function<void()> EndCallback;
    typedef typename Accessor::value_type T;

public:

    //! constructor
    CustomAnimator(ISceneManager* smgr, T const& start, T const& end, u32 const& duration,
                   int const& loop = 0, EndCallback const& cb = 0,
                   s32 const& delayTime = 0)
        : AnimatorBase( delayTime ),
          smgr_(smgr), start_(start), end_(end), distance_(end_ - start_),
          duration_(duration), loop_(loop), cb_(cb)
    {
        #ifdef _DEBUG
        setDebugName("CustomAnimator");
        #endif
        updateStartTime();
    };

    //! destructor
    virtual ~CustomAnimator(){};

    //! animates a scene node
    virtual void animateNode(ISceneNode* node, u32 timeMs) {
        if ( !node ) return;
        if ( static_cast<s32>(timeMs) < startTime_ ) return;

        if( timeMs - startTime_ >= duration_ ) {
            /* we can add periodic callback here.
            if( periodic_cb ) periodic_callback(); */
            startTime_ = timeMs; //or should be += duration_ ?
            if( loop_ == 0 ) {
                Accessor::set(node, Eq<T>::calculate(1, start_, distance_, 1, node) );
                smgr_->addToAnimatorDeletionQueue(this, node);
                if( cb_ ) cb_();
                return;
            }
            else if( loop_ > 0 ) loop_ -= 1;
        }

        T const& pos = updatePos(node, timeMs);
        Accessor::set(node, pos);
    }

    //! Creates a clone of this animator.
    /** Please note that you will have to drop
    (IReferenceCounted::drop()) the returned pointer after calling this. */
    virtual ISceneNodeAnimator* createClone(ISceneNode* node, ISceneManager* newManager=0) { return 0; }

    //! Writes attributes of the scene node animator.
    virtual void serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options=0) const {}

    //! Reads attributes of the scene node animator.
    virtual void deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options=0) {}

    //! Returns type of the scene node animator
    virtual ESCENE_NODE_ANIMATOR_TYPE getType() const { return (ESCENE_NODE_ANIMATOR_TYPE)Accessor::TYPE; }

    //! Returns a deep clone of this animator
    virtual AnimatorBase* clone() const {
        return new CustomAnimator<Eq, Accessor>(smgr_, start_, end_, duration_, loop_, cb_, delayTime_);
    }

    //! Returns a deep clone of this animator (but it dosen't clone the callback function)
    virtual AnimatorBase* cloneWithoutCallback() const {
        return new CustomAnimator<Eq, Accessor>(smgr_, start_, end_, duration_, loop_, 0, delayTime_);
    }

protected:
    inline T updatePos(ISceneNode* node, u32 const& timeMs) {
        u32 t = timeMs - startTime_;
        f32 dur  = static_cast<f32>( duration_ );
        f32 time = static_cast<f32>( t % duration_ );
        return Eq<T>::calculate(time, start_, distance_, dur, node);
    }

    template<class TT>
    inline float distance(TT const& next, TT const& last) {
        return std::abs(static_cast<float>(next - last));
    }
    inline float distance(core::vector3df const& next, core::vector3df const& last ) {
        return (next - last).getLength();
    }
    inline float distance(core::vector2df const& next, core::vector2df const& last ) {
        return (next - last).getLength();
    }

protected:
    ISceneManager* smgr_;

    T start_;
    T end_;
  //T vector_;
  //f32 length_;
    T distance_;
  //f32 timeFactor_;
  //s32 startTime_;
    u32 duration_;
    int loop_;

    EndCallback cb_;
};

} // end namespace scene
} // end namespace irr

#endif
