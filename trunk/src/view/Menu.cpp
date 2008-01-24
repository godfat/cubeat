
#include "view/Menu.hpp"
#include "view/SpriteText.hpp"
#include "Input.hpp"
#include "IrrDevice.hpp"

#include <boost/foreach.hpp>

/* TODO:
   Fix relative position problem. This is important. */

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;

using namespace psc;
using namespace view;
using std::tr1::static_pointer_cast;

Menu* Menu::clone() const
{
    Menu* obj = new Menu(*this);
    return obj;
}

Menu& Menu::addSprite(std::string const& name,
                      std::tr1::function<void(pSprite&)> cb,
                      std::string const& texture_name)
{
    pSprite newobj = Sprite::create(texture_name.size() ? texture_name : name,
                                    static_pointer_cast<Menu>(shared_from_this()));

    sprites_.insert( std::make_pair(name, newobj) );
    if( cb ) setCallbackToSprite(name, cb);
    return *this;
}

Menu& Menu::addSpriteText(std::string const& text, std::tr1::function<void(pSprite&)> cb,
                          std::string const& font_path, int size)
{
    pSpriteText newobj = SpriteText::create(text,
                                            static_pointer_cast<Menu>(shared_from_this()),
                                            font_path, size);

    sprites_.insert( std::make_pair(text, newobj) );
    if( cb ) setCallbackToSprite(text, cb);
    return *this;
}

Menu& Menu::deleteSprite(std::string const& name)
{
    sprites_.erase(name);
    return *this;
}

Menu& Menu::setCallbackToSprite(std::string const& name,
                                std::tr1::function<void(pSprite&)> cb)
{
    if( pSprite temp = sprites_[name] ) {
        BOOST_FOREACH(ctrl::Input* it, ctrl::Input::getInputs())
            temp->onPress( &it->trig1() ) = cb;
    }
    return *this;
}

Sprite& Menu::getSprite(std::string const& name)
{
    if( pSprite temp = sprites_[name] )
        return *temp;

    return *this;
}

void Menu::init(pObject const& parent)
{
    Sprite::init(parent);
}
