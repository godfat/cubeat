
#ifndef _SHOOTING_CUBES_ANIMATED_SPRITE_VIEW_
#define _SHOOTING_CUBES_ANIMATED_SPRITE_VIEW_

/* ToDo:
   How to effectively and easily animate textures?

   Currently:
   load a bunch of ITextures, and set them into
   a TextureAnimator (which derives from CustomAnimator<Linear, GenericInt>)

   Should accept float number frame number ?        */

/* TODO:
   We might need a queue and loop-finite-times features to playAnime interface,
   will be implemented when needed. */

#include "view/Sprite.hpp"

#include "utils/ObjectPool.hpp"
#include <boost/tr1/memory.hpp>
#include <string>
#include <vector>
#include <map>

namespace psc { namespace view {

class AnimatedSprite : public Sprite
{
public:
    typedef std::tr1::shared_ptr< AnimatedSprite > pointer_type;
    static pointer_type create(std::string const& name, pObject const& parent) {
        pointer_type p = psc::ObjectPool< AnimatedSprite >::create(name);
        p->init(parent);
        return p;
    }

    AnimatedSprite(std::string const& name):Sprite(name){}

    virtual AnimatedSprite* clone() const;
    virtual AnimatedSprite& addAnime(std::string const& anime_name, int total_frames);
    virtual AnimatedSprite&
        playAnime(std::string const& anime_name, irr::u32 duration, bool loop = false,
                  std::tr1::function<void()> cb = 0, irr::u32 delayTime = 0);

    virtual ~AnimatedSprite();

protected:
    void init(pObject const&);

protected:
    typedef std::vector< irr::video::ITexture* > Animation2D;
    std::map< std::string, Animation2D > animations_;
};

typedef AnimatedSprite::pointer_type pAnimatedSprite;

} //view
} //psc

#endif
