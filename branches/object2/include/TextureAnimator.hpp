
#ifndef _SHOOTING_CUBES_TEXTURE_ANIMATOR_
#define _SHOOTING_CUBES_TEXTURE_ANIMATOR_

#include "Accessor_proto.hpp"
#include "CustomAnimator.hpp"

//for debug & warning purpose
#include <iostream>

#include <vector>

namespace irr{

namespace video { class ITexture; }
namespace scene{

template <template <class> class Eq>
class TextureAnimator : public CustomAnimator<Eq, Accessor<s32> >
{
    typedef std::vector< video::ITexture* >    Animation2D;
    typedef CustomAnimator<Eq, Accessor<s32> > TextureAnimatorBase;
    typedef typename TextureAnimatorBase::T    Primitive;

public:
    TextureAnimator(Animation2D const& anime, u32 duration, bool loop = true,
                    std::tr1::function<void()> cb = 0, u32 delayTime = 0)
        :TextureAnimatorBase(0, static_cast<int>(anime.size()), duration, loop, cb, delayTime),
         animation_(anime)
    {
        if( anime.size() < 1 )
            std::cout << "Cannot construct animation with no frame, Error!\n";
    }

    virtual ~TextureAnimator(){}

    virtual void animateNode(ISceneNode* node, u32 timeMs) {
        if ( !node ) return;
        if ( timeMs < this->startTime_ ) return;
        if ( animation_.size() < 1 ) return;

        u32 t = ( timeMs - this->startTime_ );
        Primitive pos = this->start_;

        f32 dur = static_cast<f32>( this->duration_ );
        if ( !this->loop_ && t >= this->duration_ ) {
            pos = Eq<Primitive>::calculate(dur, pos, this->distance_, dur, node);
            if( pos >= static_cast<s32>( animation_.size() ) )
                pos = static_cast<s32>( animation_.size() ) - 1; //size bound safe guard

            node->setMaterialTexture(0, animation_[pos]);
            if( this->cb_ ) this->cb_();
            this->smgr_->addToAnimatorDeletionQueue(this, node);
        }
        else {
            f32 time = static_cast<f32>( t % this->duration_ );
            pos = Eq<Primitive>::calculate(time, pos, this->distance_, dur, node);
            if( pos >= static_cast<s32>( animation_.size() ) )
                pos = static_cast<s32>( animation_.size() ) - 1; //size bound safe guard

            node->setMaterialTexture(0, animation_[pos]);
        }
    }

protected:
    Animation2D animation_;

};

} // scene
} // irr

#endif
