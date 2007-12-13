
#ifndef _SHOOTING_CUBE_CUSTOM_ANIMATOR_
#define _SHOOTING_CUBE_CUSTOM_ANIMATOR_

#include "ISceneNode.h"
#include "boost/function.hpp"

namespace irr
{
namespace scene
{

namespace SetterCallback {
    struct PosSetter{
        static void set(ISceneNode* node, core::vector3df val ) {
            node->setPosition(val);
        }
    };
    struct RotSetter{
        static void set(ISceneNode* node, core::vector3df val ) {
            node->setRotation(val);
        }
    };
    struct ScaleSetter{
        static void set(ISceneNode* node, core::vector3df val ) {
            node->setScale(val);
        }
    };
    struct RGBSetter{
        static void set(ISceneNode* node, f32 val ) {
            node->getMaterial(0).DiffuseColor.setRed((u32)val%256);
            node->getMaterial(0).DiffuseColor.setGreen((u32)val%256);
            node->getMaterial(0).DiffuseColor.setBlue((u32)val%256);
        }
    };
    struct RedSetter{
        static void set(ISceneNode* node, f32 val ) {
            node->getMaterial(0).DiffuseColor.setRed((u32)val);
        }
    };
    struct GreenSetter{
        static void set(ISceneNode* node, f32 val ) {
            node->getMaterial(0).DiffuseColor.setGreen((u32)val);
        }
    };
    struct BlueSetter{
        static void set(ISceneNode* node, f32 val ) {
            node->getMaterial(0).DiffuseColor.setBlue((u32)val);
        }
    };
    struct AlphaSetter{
        static void set(ISceneNode* node, f32 val ) {
            node->getMaterial(0).DiffuseColor.setAlpha((u32)val);
        }
    };
    struct FrameSetter{
        static void set(ISceneNode* node, f32 val ) {
            if( node->getType() != ESNT_ANIMATED_MESH ) return;
            static_cast<IAnimatedMeshSceneNode*>(node)->setCurrentFrame( val );
        }
    };
} //animCallback

/*
in ObjectView:
{
    moveTo( ..., function<void()> cb = NULL) {
        ISceneNodeAnimator* anim = 
            new CustomAnimator<vector3df, Linear, PosSetter>(..., cb);
        body_->addAnimator( anim );
    }

    step1() {
        moveTo( 0, 540, 10, 2000, bind(&ObjectView::step2, this) );
    }
    step2() {
        moveTo( 420, 540, 10, 2000, bind(&ObjectView::step3, this) );
    }
    step3() {
        moveTo( 420, 0, 10, 2000, bind(&ObjectView::step4, this) );
    }
    step4() {
        moveTo( 0, 0, 10, 2000, bind(&ObjectView::step1, this) );
    }
};
*/

template <class T, template<class> class Eq, class Setter>
class CustomAnimator : public ISceneNodeAnimator
{
    typedef boost::function<void()> EndCallback;
public:

	//! constructor
    CustomAnimator(ISceneManager* smgr, u32 now, T const& startPoint, T const& endPoint, 
                   u32 timeForWay, bool loop = false, EndCallback cb = NULL) 
        : smgr_(smgr), Start(startPoint), End(endPoint), WayLength(0.0f), TimeFactor(0.0f), 
          StartTime(now), TimeForWay(timeForWay), Loop(loop), cb_(cb)
    {
	    #ifdef _DEBUG
	    setDebugName("CustomAnimator");
	    #endif
	    recalculateImidiateValues<T>();
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
            pos = Eq<T>::calculate((f32)time, pos, Vector*WayLength, (f32)TimeForWay, node);
            Setter::set(node, pos);
        }
    }

	//! Writes attributes of the scene node animator.
    virtual void serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options=0) const {}

	//! Reads attributes of the scene node animator.
    virtual void deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options=0) {}

	//! Returns type of the scene node animator
	virtual ESCENE_NODE_ANIMATOR_TYPE getType() const { return ESNAT_UNKNOWN; }

private:

    template<class T>
    void recalculateImidiateValues() {
        WayLength = End - Start;
        Vector = 1;
    }

    template<>
    void recalculateImidiateValues< irr::core::vector3df >() {
        Vector = End - Start;
	    WayLength = (f32)Vector.getLength();
	    Vector.normalize();
    }

    ISceneManager* smgr_;

    T Start;
	T End;
	T Vector;
	f32 WayLength;
	f32 TimeFactor;
	u32 StartTime;
	u32 TimeForWay;
	bool Loop;

    EndCallback cb_;
};

} // end namespace scene
} // end namespace irr

#endif