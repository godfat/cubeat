
#ifndef _SHOOTING_CUBES_ANIMATED_SPRITE_VIEW_
#define _SHOOTING_CUBES_ANIMATED_SPRITE_VIEW_

/* ToDo:
   How to effectively and easily animate textures?
   Currently: load a bunch of ITextures, and set them into
   a TextureAnimator (which derives from CustomAnimator<Linear, GenericInt>)

   Should accept float number frame number ?        */

/* TODO:
   We might need a queue and loop-finite-times features to playAnime interface,
   will be implemented when needed. */

#include "SpriteView.hpp"

#include <string>
#include <vector>
#include <map>

class AnimatedSpriteView : public SpriteView
{
public:
    AnimatedSpriteView(std::string const& name):name_(name){}

    virtual AnimatedSpriteView* clone() const;
    virtual AnimatedSpriteView& init(ObjectView const*);
    virtual AnimatedSpriteView& addAnime(std::string const& anime_name, int total_frames);
    virtual AnimatedSpriteView&
        playAnime(std::string const& anime_name, irr::u32 duration, bool loop = false,
                  std::tr1::function<void()> cb = 0, irr::u32 delayTime = 0);

    virtual ~AnimatedSpriteView();

protected:
    typedef std::vector< irr::video::ITexture* > Animation2D;
    std::map< std::string, Animation2D > animations_;
    std::string name_;
};

#endif
