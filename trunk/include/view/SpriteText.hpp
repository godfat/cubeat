#ifndef _SHOOTING_CUBES_VIEW_SPRITETEXT_
#define _SHOOTING_CUBES_VIEW_SPRITETEXT_

#include "data/Color.hpp"
#include "view/Sprite.hpp"
#include "all_fwd.hpp"
#include <string>

namespace psc { namespace view {

class SpriteText : public Sprite
{
public:
    typedef std::tr1::shared_ptr< SpriteText > pointer_type;
    static pointer_type create(std::string const& text,
                               pObject const&     parent,
                               std::string const& font_path,
                               int                size = 12,
                               bool const&        center = false,
                               data::Color const& color = data::Color(0,0,0) )
    {
        pointer_type p = utils::ObjectPool< SpriteText >::create("", center);
        p->init(text, font_path, size, color, parent);
        return p;
    }

    SpriteText(std::string const& name, bool const& center)
        : Sprite(name, center){}

    virtual SpriteText* clone() const { return 0; }
    virtual SpriteText& setCenterAligned(bool const&);

    virtual ~SpriteText();

protected:
    void init(std::string const& text, std::string const& font_path,
              int size, data::Color const& color, pObject const& parent);

protected:
    irr::video::ITexture* font_texture_;

};

} //view
} //psc

#endif // _SHOOTING_CUBES_VIEW_SPRITETEXT_
