#ifndef _SHOOTING_CUBES_VIEW_SPRITETEXT_
#define _SHOOTING_CUBES_VIEW_SPRITETEXT_

#include "view/Sprite.hpp"

#include <string>

namespace psc { namespace view {

class SpriteText : public Sprite
{
public:
    typedef std::tr1::shared_ptr< SpriteText > pointer_type;
    static pointer_type create(std::string const& text,
                               pObject const& parent,
                               std::string const& font_path = "/fonts/simhei.ttf",
                               int size = 12)
    {
        pointer_type p = psc::ObjectPool< SpriteText >::create("");
        p->init(text, font_path, size, parent);
        return p;
    }

    SpriteText(std::string const& name):Sprite(name){}

    virtual SpriteText* clone() const { return 0; }

    virtual ~SpriteText() {}

protected:
    void init(std::string const& text, std::string const& font_path,
              int size, pObject const& parent);

};

typedef SpriteText::pointer_type pSpriteText;
typedef std::tr1::weak_ptr<SpriteText> wpSpriteText;

} //view
} //psc

#endif // _SHOOTING_CUBES_VIEW_SPRITETEXT_
