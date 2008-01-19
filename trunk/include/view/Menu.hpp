
#ifndef _SHOOTING_CUBES_MENU_VIEW_
#define _SHOOTING_CUBES_MENU_VIEW_

#include "view/Sprite.hpp"
#include "utils/ObjectPool.hpp"
#include <boost/tr1/memory.hpp>

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
    static pointer_type create(pObject const& parent) {
        pointer_type p = psc::ObjectPool< Menu >::create();
        p->init(parent);
        return p;
    }
    static pointer_type create(std::string const& name, pObject const& parent) {
        pointer_type p = psc::ObjectPool< Menu >::create(name);
        p->init(parent);
        return p;
    }

    Menu():Sprite(""){}
    Menu(std::string const& name):Sprite(name){}

    virtual Menu* clone() const;

    virtual Menu&   addSprite(std::string const& name,
                              std::tr1::function<void(pSprite&)> cb = 0,
                              std::string const& texture_name = "");
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

typedef Menu::pointer_type pMenu;

} //view
} //psc

#endif
