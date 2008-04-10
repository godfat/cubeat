
#ifndef _SHOOTING_CUBES_TEXTURE_ANIMATOR_
#define _SHOOTING_CUBES_TEXTURE_ANIMATOR_

#include "Accessor_proto.hpp"
#include "view/detail/CustomAnimator.hpp"

//for debug & warning purpose
#include <iostream>
#include <vector>

namespace irr{

namespace video { class ITexture; }
namespace scene{

template <template <class> class Eq>
class TextureAnimator : public
    CustomAnimator<Eq, psc::accessor::Accessor<s32, psc::accessor::AT::TEXTURE> >
{
    typedef std::vector< video::ITexture* >     Animation2D;
    typedef CustomAnimator
        <Eq, psc::accessor::Accessor
            <s32, psc::accessor::AT::TEXTURE> > TextureAnimatorBase;
    typedef typename TextureAnimatorBase::T     T;

public:
    TextureAnimator(ISceneManager* smgr, Animation2D const& anime, u32 const& duration,
                    int const& loop = 0, std::tr1::function<void()>const& cb = 0,
                    s32 const& delayTime = 0)
        :TextureAnimatorBase(smgr, 0, static_cast<int>(anime.size()), duration, loop, cb, delayTime),
         animation_(anime)
    {
        if( anime.size() < 1 ) {
            std::cout << "Cannot construct animation with no frame, Error!\n";
            return;
        }
    }

    virtual ~TextureAnimator(){}

    virtual void animateNode(ISceneNode* node, u32 timeMs) {
        if ( !node ) return;
        if ( static_cast<s32>(timeMs) < this->startTime_ ) return;
        if ( animation_.size() < 1 ) {
            this->smgr_->addToAnimatorDeletionQueue(this, node);
            return;
        }

        if( timeMs - this->startTime_ >= this->duration_ ) {
            /* we can add periodic callback here.
            if( this->periodic_cb() ) this->periodic_callback(); */
            this->startTime_ = timeMs; //or should be += this->duration_ ?
            if( this->loop_ == 0 ) {
                T pos = Eq<T>::calculate(1, this->start_, this->distance_, 1, node);
                if( pos >= this->end_ ) pos = this->end_ - 1; //size bound safe guard
                node->setMaterialTexture(0, animation_[pos]);
                if( this->cb_ ) this->cb_();
                this->smgr_->addToAnimatorDeletionQueue(this, node);
                return;   //DAMN....... WHY FORGOT THIS.........
            }
            else if( this->loop_ > 0 ) this->loop_ -= 1;
        }

        T pos = this->updatePos(node, timeMs);
        if( pos >= this->end_ ) pos = this->end_ - 1;   //size bound safe guard
        node->setMaterialTexture(0, animation_[pos]);
    }

protected:
    Animation2D animation_;

};

} // scene
} // irr

#endif
