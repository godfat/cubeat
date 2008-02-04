
#ifndef _SHOOTING_CUBES_MENU_VIEW_
#define _SHOOTING_CUBES_MENU_VIEW_

#include "view/Sprite.hpp"
#include "data/Color.hpp"
#include "utils/ObjectPool.hpp"
#include "all_fwd.hpp"

#include <map>
#include <string>
#include <utility> //for pair

/* TODO:
   Fix relative position problem. This is important. */

namespace psc { namespace view {

class Menu : public Sprite
{
public:
    typedef std::tr1::shared_ptr< Menu > pointer_type;
    static pointer_type create(pObject const& parent, bool const& center = false) {
        pointer_type p = utils::ObjectPool< Menu >::create(center);
        p->init(parent);
        return p;
    }
    static pointer_type create(std::string const& name,
                               pObject const& parent,
                               bool const& center = false)
    {
        pointer_type p = utils::ObjectPool< Menu >::create(name, center);
        p->init(parent);
        return p;
    }

    Menu(bool const& center):Sprite("", center){}
    Menu(std::string const& name, bool const& center)
        :Sprite(name, center){}

    virtual Menu* clone() const;

    virtual Menu&   addSprite(std::string const& name,
                              std::tr1::function<void(pSprite&)> cb = 0,
                              std::string const& texture_name = "");
    virtual Menu&   addSpriteText(std::string const& text,
                                  std::tr1::function<void(pSprite&)> cb = 0,
                                  std::string const& font_path = "/fonts/simhei.ttf",
                                  int size = 12,
                                  data::Color const& color = data::Color(0,0,0) );
    virtual Menu&   deleteSprite(std::string const& name);
    virtual Sprite& getSprite(std::string const& name);    //non-const-getter
    virtual Menu&   setCallbackToSprite(std::string const& name,
                                        std::tr1::function<void(pSprite&)> cb);

    virtual ~Menu(){}

protected:
    void init(pObject const&);

protected:
    typedef std::pair< std::string, pSprite > SpriteListItem;
    typedef std::map< std::string, pSprite > SpriteList;

    SpriteList sprites_;

};

} //view
} //psc

#endif
