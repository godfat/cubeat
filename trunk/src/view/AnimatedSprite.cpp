
#include "view/AnimatedSprite.hpp"
#include "TextureAnimator.hpp"
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
    size_.Width  = w;
    size_.Height = h;

    SMaterial mat_;

    mat_.setFlag(video::EMF_LIGHTING, true);
    mat_.setFlag(video::EMF_ZWRITE_ENABLE, false);
    mat_.MaterialType = video::EMT_ONETEXTURE_BLEND;

    mat_.MaterialTypeParam =
        video::pack_texureBlendFunc(EBF_SRC_ALPHA, EBF_ONE_MINUS_SRC_ALPHA, EMFN_MODULATE_1X);

    mat_.DiffuseColor.set(255,255,255,255);

    setupMeshBase(parent);
    body_->getMaterial(0) = mat_;

    press_.setOwner( static_pointer_cast<AnimatedSprite>(shared_from_this()) );
    scene_ = parent->scene();

    return static_pointer_cast<AnimatedSprite>(shared_from_this());
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
AnimatedSprite::playAnime(std::string const& anime_name, int duration, bool loop,
                          std::tr1::function<void()> cb, int delayTime)
{
    ISceneNodeAnimator* animator =
        new TextureAnimator<SineCirc>( animations_[anime_name], duration, loop, cb, delayTime );
    body_->addAnimator( animator );
    animator->drop();

    return *this;
}

AnimatedSprite::~AnimatedSprite()
{
}
