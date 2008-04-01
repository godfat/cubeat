
#include "view/AnimatedSprite.hpp"
#include "view/Scene.hpp"
#include "view/detail/TextureAnimator.hpp"
#include "EasingEquations.hpp"
#include "IrrDevice.hpp"

#include <sstream>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;

using namespace psc;
using namespace view;
using namespace easing;
using std::tr1::static_pointer_cast;

AnimatedSprite* AnimatedSprite::clone() const
{
    AnimatedSprite* obj = new AnimatedSprite(*this);
    return obj;
}

pAnimatedSprite AnimatedSprite::init(pObject const& parent, int const& w, int const& h)
{
    setupSceneAndManager(parent);

    size_.Width  = w;
    size_.Height = h;

    SMaterial mat;
    mat.setFlag(video::EMF_LIGHTING, true);
    mat.setFlag(video::EMF_ZWRITE_ENABLE, false);
    mat.MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL;
    mat.MaterialTypeParam = 0.01f;

    mat.DiffuseColor.set(255,255,255,255);

    setupMeshBase(parent);
    body_->getMaterial(0) = mat;

    pAnimatedSprite self = static_pointer_cast<AnimatedSprite>( shared_from_this() );
    scene()->addPickMapping( body_, self );
    return self;
}

AnimatedSprite& AnimatedSprite::addAnime(std::string const& anime_name, int total_frames)
{
    video::IVideoDriver* driver = smgr_->getVideoDriver();
    driver->setTextureCreationFlag(video::ETCF_OPTIMIZED_FOR_QUALITY, true);

    Animation2D animation;
    for(int i=0; i < total_frames; ++i ) {
        std::ostringstream oss;
        oss << "rc/texture/" << name_ << "/" << anime_name << "/" << i << ".png";
        ITexture* tex = driver->getTexture( oss.str().c_str() );
        animation.push_back( tex );
    }

    animations_.insert( std::make_pair( anime_name, animation ) );

    return *this;
}

AnimatedSprite&
AnimatedSprite::playAnime(std::string const& anime_name, unsigned int const& duration, int const& loop,
                          std::tr1::function<void()> cb, int const& delayTime)
{
    if( !duration ) return *this;
    ISceneNodeAnimator* animator =
        new TextureAnimator<SineCirc>( smgr_, animations_[anime_name], duration, loop, cb, delayTime );
    body_->addAnimator( animator );
    animator->drop();

    return *this;
}

AnimatedSprite::~AnimatedSprite()
{
}
