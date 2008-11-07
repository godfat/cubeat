#ifndef _SHOOTING_CUBES_VIEW_SPRITETEXT_
#define _SHOOTING_CUBES_VIEW_SPRITETEXT_

#include "data/Color.hpp"
#include "view/Sprite.hpp"
#include "Accessor_proto.hpp"

#include "all_fwd.hpp"
#include <string>

namespace irr {
namespace gui {
class IGUITTFont;
}
}

namespace psc {
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
    virtual SpriteText& clearTween(accessor::AT::ATEnum const&);
    virtual SpriteText& clearAllTween();

    // SpriteText version: an animation which specified only end, will use current status as start.
    template <template <class> class Eq, class Accessor>
    SpriteText& tween(typename Accessor::value_type const& end,
                      unsigned int                  const& duration,
                      int                           const& loop = 0,
                      std::tr1::function<void()>    const& cb = 0,
                      int                           const& delay = 0)
    {
        typename Accessor::value_type start = typename Accessor::value_type();
        if( !accessor::AT::isMatrixTransformationValue( static_cast<accessor::AT::ATEnum>(Accessor::TYPE) )
            && letter_node_.size() )
            Accessor::get(letter_node_.front(), start);
        else
            Accessor::get(body_, start);
        Object::tween<Eq, Accessor>(start, end, duration, loop, cb, delay);
        return *this;
    }

    // SpriteText version: an animation with speed control (current status, end)
    template <template <class> class Eq, class Accessor>
    SpriteText& tween(typename Accessor::value_type const& end,
                      std::tr1::function<float()>   const& speedfunc,
                      int                           const& loop = 0,
                      std::tr1::function<void()>    const& cb = 0,
                      int                           const& delay = 0)
    {
        typename Accessor::value_type start = typename Accessor::value_type();
        if( !accessor::AT::isMatrixTransformationValue( static_cast<accessor::AT::ATEnum>(Accessor::TYPE) )
            && letter_node_.size() )
            Accessor::get(letter_node_.front(), start);
        else
            Accessor::get(body_, start);
        Object::tween<Eq, Accessor>(start, end, speedfunc, loop, cb, delay);
        return *this;
    }

    // SpriteText version: an animation which specified only end, will use current status as start.
    template <template <class> class Eq, class Accessor>
    SpriteText& queue(typename Accessor::value_type const& end,
                      unsigned int                  const& duration,
                      int                           const& loop = 0,
                      std::tr1::function<void()>    const& cb = 0,
                      int                           const& delay = 0)
    {
        if( duration == 0 ) return *this;
        typename Accessor::value_type start = typename Accessor::value_type();
        if( !accessor::AT::isMatrixTransformationValue( static_cast<accessor::AT::ATEnum>(Accessor::TYPE) )
            && letter_node_.size() )
            Accessor::get(letter_node_.front(), start);
        else
            Accessor::get(body_, start);
        Object::queue<Eq, Accessor>(start, end, duration, loop, cb, delay);
        return *this;
    }

    // SpriteText version: an animation with speed control (current status, end)
    template <template <class> class Eq, class Accessor>
    SpriteText& queue(typename Accessor::value_type const& end,
                      std::tr1::function<float()>   const& speedfunc,
                      int                           const& loop = 0,
                      std::tr1::function<void()>    const& cb = 0,
                      int                           const& delay = 0)
    {
        if( !speedfunc ) return *this;
        typename Accessor::value_type start = typename Accessor::value_type();
        if( !accessor::AT::isMatrixTransformationValue( static_cast<accessor::AT::ATEnum>(Accessor::TYPE) )
            && letter_node_.size() )
            Accessor::get(letter_node_.front(), start);
        else
            Accessor::get(body_, start);
        Object::queue<Eq, Accessor>(start, end, speedfunc, loop, cb, delay);
        return *this;
    }

    SpriteText& changeText(std::string const&);
    SpriteText& showNumber(int, unsigned int digit = 0);
    SpriteText& setTextColor(data::Color const&);
    SpriteText& setTextColor(irr::video::SColor const&);
    SpriteText& setTextColor(int const&, int const&, int const&, int const&);
    SpriteText& setTextColor(int const&);
    SpriteText& setTextAlpha(int const&);

    virtual ~SpriteText();

protected:
    pointer_type init(std::string const& text, std::string const& font_path,
                      int const& size, data::Color const& color, pObject const& parent);
    void createText(std::string const& text, std::string const& font_path, int const& size);
    void generateLetter(char const& c, char const& last_c, int& current_xpos);
    void clearText();

    virtual void startTween();

protected:
    int fsize_;
    std::string fpath_;
    std::string text_;
    irr::gui::IGUITTFont* ttfont_;

    //added these properties for SpriteText underlying mechanism corrections.
    std::vector< irr::scene::ISceneNode* > letter_node_;
    std::vector< irr::scene::IMesh* >      letter_mesh_;
};

} //view
} //psc

#endif // _SHOOTING_CUBES_VIEW_SPRITETEXT_
