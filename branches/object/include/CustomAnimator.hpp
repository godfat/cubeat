
#ifndef _SHOOTING_CUBE_CUSTOM_ANIMATOR_
#define _SHOOTING_CUBE_CUSTOM_ANIMATOR_

#include "ISceneNode.h"
#include "IrrDevice.hpp" //probably a bad idea, we'll see.

#include <boost/function.hpp>

namespace irr
{
namespace scene
{

template <template<class> class Eq, class Accessor>
class CustomAnimator : public ISceneNodeAnimator
{
    typedef boost::function<void()> EndCallback;
    typedef typename Accessor::value_type T;
public:

	//! constructor
    CustomAnimator(ISceneNode* node, T const& start, T const& end, u32 duration, 
                   bool loop = true, EndCallback cb = 0, u32 delayTime = 0) 
        : start_(start), end_(end), distance_(0.0f), duration_(duration), 
          loop_(loop), cb_(cb)
    {
	    #ifdef _DEBUG
	    setDebugName("CustomAnimator");
	    #endif
        smgr_ = IrrDevice::i()->getSceneManager();
        node->addAnimator( this );
        this->drop();
        startTime_ = IrrDevice::i()->getTimer()->getTime() + delayTime;
	    recalculateImidiateValues(end);
    };

    CustomAnimator(ISceneNode* node, T const& end, u32 duration, bool loop = true, 
                   EndCallback cb = 0, u32 delayTime = 0) 
        : end_(end), distance_(0.0f), duration_(duration), loop_(loop), cb_(cb)
    {
	    #ifdef _DEBUG
	    setDebugName("CustomAnimator");
	    #endif
        smgr_ = IrrDevice::i()->getSceneManager();
        node->addAnimator( this );
        this->drop();
        Accessor::get(node, start_);
        startTime_ = IrrDevice::i()->getTimer()->getTime() + delayTime;
	    recalculateImidiateValues(end);
    };

	//! destructor
    virtual ~CustomAnimator(){};

	//! animates a scene node
    virtual void animateNode(ISceneNode* node, u32 timeMs) {
	    if ( !node ) return;

	    u32 t = (timeMs-startTime_);
	    T pos = start_;

        if (!loop_ && t >= duration_) {
		    pos = end_; 
            Accessor::set(node, pos);
            if( cb_ ) cb_();
            smgr_->addToAnimatorDeletionQueue(this, node); 
        }
        else {
            u32 time = t % duration_;
            pos = Eq<T>::calculate((f32)time, pos, static_cast<T>(vector_*distance_), (f32)duration_, node);
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
        distance_ = static_cast<f32>(end_ - start_);
        vector_ = 1;
    }

    void recalculateImidiateValues(core::vector3df const&) {
        vector_ = end_ - start_;
        distance_ = vector_.getLength();
        vector_.normalize();
    }

    void recalculateImidiateValues(core::vector2df const&) {
        vector_ = end_ - start_;
        distance_ = vector_.getLength();
        vector_.normalize();
    }

    ISceneManager* smgr_;

    T start_;
	T end_;
	T vector_;
	f32 distance_;
  //f32 timeFactor_;
	u32 startTime_;
	u32 duration_;
	bool loop_;

    EndCallback cb_;
};

} // end namespace scene
} // end namespace irr

#endif