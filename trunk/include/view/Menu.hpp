
#ifndef _SHOOTING_CUBES_MENU_VIEW_
#define _SHOOTING_CUBES_MENU_VIEW_

#include "view/detail/CustomAnimator.hpp"
#include "view/Sprite.hpp"
#include "data/Color.hpp"
#include "utils/ObjectPool.hpp"
#include "all_fwd.hpp"

#include <boost/foreach.hpp>
#include <map>
#include <string>
#include <utility> //for pair


namespace psc { namespace view {

class Menu : public Sprite
{
public:
    typedef std::tr1::shared_ptr< Menu > pointer_type;
    static pointer_type create(pObject const& parent,
                               int const& w = 100,
                               int const& h = 100,
                               bool const& center = false)
    {
        return utils::ObjectPool< Menu >::create(center)->init(parent, w, h);
    }
    static pointer_type create(std::string const& name,
                               pObject const& parent,
                               int const& w = 100,
                               int const& h = 100,
                               bool const& center = false)
    {
        return utils::ObjectPool< Menu >::create(name, center)->init(parent, w, h);
    }

    Menu(bool const& center):Sprite("", center){}
    Menu(std::string const& name, bool const& center)
        :Sprite(name, center){}

    virtual ~Menu(){}
    virtual Menu* clone() const;

    virtual Menu&   addSprite(std::string const& name,
                              std::tr1::function<void(pSprite&)> cb = 0,
                              int const& w = 100,
                              int const& h = 100,
                              bool const& center = false,
                              std::string const& texture_name = "");

    virtual Menu&   addSpriteText(std::string const& name,
                                  std::string const& text,
                                  std::string const& font_path,
                                  std::tr1::function<void(pSprite&)> cb = 0,
                                  int size = 12,
                                  bool const& center = false,
                                  data::Color const& color = data::Color(255,255,255) );

    virtual Menu&   deleteSprite(std::string const& name);
    virtual Sprite& getSprite(std::string const& name);    //non-const-getter
    virtual Menu&   setCallbackToSprite(std::string const& name,
                                        std::tr1::function<void(pSprite&)> cb);

    //Important Note: Use for material-related tweening only
    template <template <class> class Eq, class Accessor>
    Menu& tweenAll(typename Accessor::value_type const& start,
                   typename Accessor::value_type const& end,
                   int                           const& duration,
                   int                           const& loop = 0,
                   std::tr1::function<void()>    const& cb = 0,
                   int                           const& delay = 0)
    {
        Object::tween<Eq, Accessor>( start, end, duration, loop, cb, delay );
        BOOST_FOREACH( SpriteListItem& it, sprites_ )
            it.second->tween<Eq, Accessor>( start, end, duration, loop, cb, delay );
        //bad usage for map, but heck, I don't want multi_index...
        return *this;
    }

    //Important Note: Use for material-related tweening only
    template <template <class> class Eq, class Accessor>
    Menu& tweenAll(typename Accessor::value_type const& end,
                   int                           const& duration,
                   int                           const& loop = 0,
                   std::tr1::function<void()>    const& cb = 0,
                   int                           const& delay = 0)
    {
        typename Accessor::value_type start = typename Accessor::value_type();
        Accessor::get(body_, start);
        Object::tween<Eq, Accessor>(start, end, duration, loop, cb, delay);
        BOOST_FOREACH( SpriteListItem& it, sprites_ )
            it.second->tween<Eq, Accessor>( end, duration, loop, cb, delay );
        //bad usage for map, but heck, I don't want multi_index...
        return *this;
    }

protected:
    pointer_type init(pObject const&, int const&, int const&);

protected:
    typedef std::pair< const std::string, pSprite > SpriteListItem;
    typedef std::map< std::string, pSprite > SpriteList;

    SpriteList sprites_;

};

} //view
} //psc

#endif
