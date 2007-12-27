
#ifndef _SHOOTING_CUBE_CUSTOM_ANIMATOR_
#define _SHOOTING_CUBE_CUSTOM_ANIMATOR_

#include "ISceneNode.h"
#include "include/IrrDevice.hpp"
#include <boost/tr1/functional.hpp>

namespace irr
{
namespace scene
{

template <template<class> class Eq, class Accessor>
class CustomAnimator : public ISceneNodeAnimator
{
protected:
    typedef std::tr1::function<void()> EndCallback;
    typedef typename Accessor::value_type T;

public:

	//! constructor
    CustomAnimator(T const& start, T const& end, u32 duration, bool loop = true,
                   EndCallback cb = 0, u32 delayTime = 0)
        : start_(start), end_(end), length_(0.0f), duration_(duration),
          loop_(loop), cb_(cb)
    {
	    #ifdef _DEBUG
	    setDebugName("CustomAnimator");
	    #endif
        smgr_ = IrrDevice::i()->getSceneManager();
        startTime_ = IrrDevice::i()->getTimer()->getTime() + delayTime;
	    recalculateImidiateValues(end);
    };

	//! destructor
    virtual ~CustomAnimator(){};

	//! animates a scene node
    virtual void animateNode(ISceneNode* node, u32 timeMs) {
	    if ( !node ) return;
        if ( timeMs < startTime_ ) return;

	    u32 t = (timeMs-startTime_);
	    T pos = start_;

        f32 dur = static_cast<f32>( duration_ );
        if ( !loop_ && t >= duration_ ) {
            pos = Eq<T>::calculate(dur, pos, distance_, dur, node);
            Accessor::set(node, pos);
            if( cb_ ) cb_();
            smgr_->addToAnimatorDeletionQueue(this, node);
        }
        else {
            f32 time = static_cast<f32>( t % duration_ );
            pos = Eq<T>::calculate(time, pos, distance_, dur, node);
            Accessor::set(node, pos);
        }
    }

	//! Writes attributes of the scene node animator.
    virtual void serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options=0) const {}

	//! Reads attributes of the scene node animator.
    virtual void deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options=0) {}

	//! Returns type of the scene node animator
	virtual ESCENE_NODE_ANIMATOR_TYPE getType() const { return ESNAT_UNKNOWN; }

protected:
    template<class TT>
    void recalculateImidiateValues(TT const&) {
        length_ = static_cast<f32>(end_ - start_);
        vector_ = 1;
        distance_ = static_cast<T>(length_);
    }

    void recalculateImidiateValues(core::vector3df const&) {
        vector_ = end_ - start_;
        length_ = vector_.getLength();
        vector_.normalize();
        distance_ = static_cast<T>(length_ * vector_);
    }

    void recalculateImidiateValues(core::vector2df const&) {
        vector_ = end_ - start_;
        length_ = vector_.getLength();
        vector_.normalize();
        distance_ = static_cast<T>(length_ * vector_);
    }

    ISceneManager* smgr_;

    T start_;
	T end_;
	T vector_;
    f32 length_;
	T distance_;
  //f32 timeFactor_;
	u32 startTime_;
	u32 duration_;
	bool loop_;

    EndCallback cb_;
};

} // end namespace scene
} // end namespace irr

#endif
