#ifndef _SHOOTING_CUBES_VIEW_SPRITETEXT2_
#define _SHOOTING_CUBES_VIEW_SPRITETEXT2_

#include "data/Color.hpp"
#include "view/Sprite.hpp"
#include "Accessor_proto.hpp"

#include <string>
#include <boost/foreach.hpp>

namespace psc {
namespace view {

class SpriteText2 : public Sprite
{
public:
    typedef std::tr1::shared_ptr< SpriteText2 > pointer_type;
    typedef std::tr1::weak_ptr< SpriteText2 >   wpointer_type;

    static pointer_type create(std::string const&  text,
                               pObject const&      parent,
                               bool const&         center = false)
    {
        return utils::ObjectPool< SpriteText2 >::create("", center)
                   ->init(text, parent);
    }

    SpriteText2(std::string const& name, bool const& center)
        : Sprite(name, center){}

    template <class Accessor>
    SpriteText2& set(typename Accessor::value_type const& value) {
        accessor::AT::ATEnum e = static_cast<accessor::AT::ATEnum>(Accessor::TYPE);

        if( accessor::AT::isMatrixTransformationValue( e ) )
            Accessor::set(body_, value);
        else
            BOOST_FOREACH(irr::scene::ISceneNode* it, letter_node_)
                Accessor::set(it, value);

        return *this;
    }

    template <class Accessor>
    SpriteText2& set_outline(typename Accessor::value_type const& value) {
        BOOST_FOREACH(irr::scene::ISceneNode* it, letter_node_outline_)
            Accessor::set(it, value);

        return *this;
    }

    template <class Accessor>
    typename Accessor::value_type get() const {
        typename Accessor::value_type out = typename Accessor::value_type();
        accessor::AT::ATEnum e = static_cast<accessor::AT::ATEnum>(Accessor::TYPE);

        if( !accessor::AT::isMatrixTransformationValue( e ) && letter_node_.size() )
            Accessor::get(letter_node_.front(), out);
        else
            Accessor::get(body_, out);
        return out;
    }

    template <class Accessor>
    typename Accessor::value_type get_outline() const {
        typename Accessor::value_type out = typename Accessor::value_type();
        accessor::AT::ATEnum e = static_cast<accessor::AT::ATEnum>(Accessor::TYPE);
        Accessor::get(letter_node_outline_.front(), out);

        return *this;
    }

    virtual SpriteText2& setCenterAligned(bool const&);
    virtual SpriteText2& clearTween(accessor::AT::ATEnum const&);
    SpriteText2& clearTween_outline(accessor::AT::ATEnum const&);
    virtual SpriteText2& clearAllTween();

    // SpriteText version: an animation which specified only end, will use current status as start.
    template <template <class> class Eq, class Accessor>
    SpriteText2& tween(typename Accessor::value_type const& end,
                       unsigned int                  const& duration,
                       int                           const& loop = 0,
                       std::tr1::function<void()>    const& cb = 0,
                       int                           const& delay = 0)
    {
        Object::tween<Eq, Accessor>(get<Accessor>(), end, duration, loop, cb, delay);
        return *this;
    }

    template <template <class> class Eq, class Accessor>
    SpriteText2& tween_outline(typename Accessor::value_type const& start,
                               typename Accessor::value_type const& end,
                               unsigned int                  const& duration,
                               int                           const& loop = 0,
                               std::tr1::function<void()>    const& cb = 0,
                               int                           const& delay = 0)
    {
        if( duration == 0 ) return *this;
        irr::scene::AnimatorBase* anim =
            new irr::scene::CustomAnimator<Eq, Accessor>
                (smgr_, start, end, duration, loop, cb, delay);
        anim_queue_.push_back( anim );

        startTween_outline();
        return *this;
    }

    template <template <class> class Eq, class Accessor>
    SpriteText2& tween(typename Accessor::value_type const& start,
                       typename Accessor::value_type const& end,
                       unsigned int                  const& duration,
                       int                           const& loop = 0,
                       std::tr1::function<void()>    const& cb = 0,
                       int                           const& delay = 0)
    {
        Object::tween<Eq, Accessor>(start, end, duration, loop, cb, delay);
        return *this;
    }

    // SpriteText version: an animation with speed control (current status, end)
    template <template <class> class Eq, class Accessor>
    SpriteText2& tween(typename Accessor::value_type const& end,
                       std::tr1::function<float()>   const& speedfunc,
                       int                           const& loop = 0,
                       std::tr1::function<void()>    const& cb = 0,
                       int                           const& delay = 0)
    {
        Object::tween<Eq, Accessor>(get<Accessor>(), end, speedfunc, loop, cb, delay);
        return *this;
    }

    // SpriteText version: an animation which specified only end, will use current status as start.
    template <template <class> class Eq, class Accessor>
    SpriteText2& queue(typename Accessor::value_type const& end,
                       unsigned int                  const& duration,
                       int                           const& loop = 0,
                       std::tr1::function<void()>    const& cb = 0,
                       int                           const& delay = 0)
    {
        if( duration == 0 ) return *this;
        Object::queue<Eq, Accessor>(get<Accessor>(), end, duration, loop, cb, delay);
        return *this;
    }

    // SpriteText version: an animation with speed control (current status, end)
    template <template <class> class Eq, class Accessor>
    SpriteText2& queue(typename Accessor::value_type const& end,
                       std::tr1::function<float()>   const& speedfunc,
                       int                           const& loop = 0,
                       std::tr1::function<void()>    const& cb = 0,
                       int                           const& delay = 0)
    {
        if( !speedfunc ) return *this;
        Object::queue<Eq, Accessor>(get<Accessor>(), end, speedfunc, loop, cb, delay);
        return *this;
    }

    SpriteText2& changeText(std::string const&);
    SpriteText2& showDouble(double, unsigned int precision = 2);
    SpriteText2& glow_animation();
    std::string const& getText() const { return text_; }

    virtual ~SpriteText2();

protected:
    pointer_type init(std::string const& text, pObject const& parent);
    void createText(std::string const& text);
    void clearText();

    virtual void startTween();
    void startTween_outline();

protected:
    std::string text_;

    //added these properties for SpriteText underlying mechanism corrections.
    std::vector< irr::scene::ISceneNode* > letter_node_;
    std::vector< irr::scene::ISceneNode* > letter_node_outline_;
};

typedef SpriteText2::pointer_type  pSpriteText2;
typedef SpriteText2::wpointer_type wpSpriteText2;

} //view
} //psc

#endif // _SHOOTING_CUBES_VIEW_SPRITETEXT2_
