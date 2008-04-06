#ifndef _SHOOTING_CUBES_VIEW_SPRITETEXT_
#define _SHOOTING_CUBES_VIEW_SPRITETEXT_

#include "data/Color.hpp"
#include "view/Sprite.hpp"
#include "all_fwd.hpp"
#include <string>

namespace psc {

namespace utils {
class TTFont;
}

namespace view {

class SpriteText : public Sprite
{
public:
    typedef std::tr1::shared_ptr< SpriteText > pointer_type;
    static pointer_type create(std::string const& text,
                               pObject const&     parent,
                               std::string const& font_path,
                               int                size = 12,
                               bool const&        center = false,
                               data::Color const& color = data::Color(255,255,255) )
    {
        return utils::ObjectPool< SpriteText >::create("", center)
                   ->init(text, font_path, size, color, parent);
    }

    SpriteText(std::string const& name, bool const& center)
        : Sprite(name, center){}

    virtual SpriteText* clone() const { return 0; }
    virtual SpriteText& setCenterAligned(bool const&);

    SpriteText& changeText(std::string const&);
    SpriteText& showNumber(int, unsigned int digit = 0);

    virtual ~SpriteText();

protected:
    pointer_type init(std::string const& text, std::string const& font_path,
                      int const& size, data::Color const& color, pObject const& parent);
    void createText(std::string const& text, std::string const& font_path, int const& size);

protected:
    std::string text_;
    irr::video::ITexture* font_texture_;
    utils::TTFont* ttfont_;
};

} //view
} //psc

#endif // _SHOOTING_CUBES_VIEW_SPRITETEXT_
