
#ifndef _SHOOTING_CUBE_CUSTOM_ANIMATOR_
#define _SHOOTING_CUBE_CUSTOM_ANIMATOR_

#include "ISceneNode.h"
#include "IrrDevice.hpp" //probably a bad idea, we'll see.

#include <boost/function.hpp>

namespace irr
{
namespace scene
{

template <template<class> class Eq, class Setter>
class CustomAnimator : public ISceneNodeAnimator
{
    typedef boost::function<void()> EndCallback;
    typedef typename Setter::value_type T;
public:

	//! constructor
    CustomAnimator(T const& startPoint, T const& endPoint, u32 timeForWay, 
                   bool loop = true, EndCallback cb = NULL, u32 delayTime = 0) 
        : Start(startPoint), End(endPoint), WayLength(0.0f), TimeForWay(timeForWay), 
          Loop(loop), cb_(cb)
    {
	    #ifdef _DEBUG
	    setDebugName("CustomAnimator");
	    #endif
        smgr_ = IrrDevice::i()->getSceneManager();
        StartTime = IrrDevice::i()->getTimer()->getTime() + delayTime;
	    recalculateImidiateValues(startPoint);
    };

	//! destructor
    virtual ~CustomAnimator(){};

	//! animates a scene node
    virtual void animateNode(ISceneNode* node, u32 timeMs) {
	    if (!node) return;

	    u32 t = (timeMs-StartTime);

	    T pos = Start;

        if (!Loop && t >= TimeForWay) {
		    pos = End; 
            Setter::set(node, pos);
            if( cb_ ) cb_();
            smgr_->addToAnimatorDeletionQueue(this, node); 
        }
        else {
            u32 time = t % TimeForWay;
            pos = Eq<T>::calculate((f32)time, pos, static_cast<T>(Vector*WayLength), (f32)TimeForWay, node);
            Setter::set(node, pos);
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
        WayLength = static_cast<f32>(End - Start);
        Vector = 1;
    }

    void recalculateImidiateValues(core::vector3df const&) {
        Vector = End - Start;
        WayLength = Vector.getLength();
        Vector.normalize();
    }

    ISceneManager* smgr_;

    T Start;
	T End;
	T Vector;
	f32 WayLength;
  //f32 TimeFactor;
	u32 StartTime;
	u32 TimeForWay;
	bool Loop;

    EndCallback cb_;
};

} // end namespace scene
} // end namespace irr

#endif