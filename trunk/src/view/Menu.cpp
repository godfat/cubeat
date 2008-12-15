
#include "view/Menu.hpp"
#include "view/SpriteText.hpp"
#include "view/AnimatedSprite.hpp"
#include "view/Scene.hpp"
#include "Input.hpp"
#include "IrrDevice.hpp"
#include "Accessors.hpp"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;

using namespace psc;
using namespace view;
using namespace accessor;
using std::tr1::static_pointer_cast;
using std::tr1::dynamic_pointer_cast;

Menu* Menu::clone() const
{
    Menu* obj = new Menu(*this);
    return obj;
}

Menu& Menu::addSprite(std::string const& name,
                      std::tr1::function<void(pSprite&)> cb,
                      int const& w,
                      int const& h,
                      bool const& center,
                      std::string const& texture_name)
{
    pSprite newobj = Sprite::create(texture_name.size() ? texture_name : name,
                                    static_pointer_cast<Menu>(shared_from_this()), w, h, center);

    sprites_.insert( std::make_pair(name, newobj) );
    if( cb ) setCallbackToSprite(name, cb);
    return *this;
}

Menu& Menu::addSpriteText(std::string const& name, std::string const& text,
                          std::string const& font_path, std::tr1::function<void(pSprite&)> cb,
                          int size, bool const& center, data::Color const& color)
{
    pSpriteText newobj = SpriteText::create(text,
                                            static_pointer_cast<Menu>(shared_from_this()),
                                            font_path, size, center, color);

    sprites_.insert( std::make_pair(name, newobj) );
    if( cb ) setCallbackToSprite(name, cb);
    return *this;
}

Menu& Menu::addAnimSprite(std::string const& name, std::string const& sprite_name,
                          std::tr1::function<void(pSprite&)> cb, int const& w, int const& h,
                          bool const& center)
{
    pAnimatedSprite newobj = AnimatedSprite::create(sprite_name,
                                                    static_pointer_cast<Menu>(shared_from_this()),
                                                    w, h, center);
    sprites_.insert( std::make_pair(name, newobj) );
    if( cb ) setCallbackToSprite(name, cb);
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
        BOOST_FOREACH(ctrl::Input* it, ctrl::InputMgr::i().getInputs())
            temp->onPress( &it->trig1() ) = cb;
    }
    return *this;
}

Menu& Menu::flipH()
{
    textureFlipH();
    BOOST_FOREACH( SpriteListItem& it, sprites_ ) {
        it.second->textureFlipH();
        vec2 pos;
        if( center_ == true ) {
            pos = it.second->get<Pos2D>();
            pos.X *= -1;
            it.second->set<Pos2D>(pos);
        }
        else {
            pos = it.second->get<Pos2D>();
            pos.X = size_.Width - pos.X;
            it.second->set<Pos2D>(pos);
        }
    }
    return *this;
}

Menu& Menu::flipV()
{
    textureFlipV();
    BOOST_FOREACH( SpriteListItem& it, sprites_ ) {
        it.second->textureFlipV();
        vec2 pos;
        if( center_ == true ) {
            pos = it.second->get<Pos2D>();
            pos.Y *= -1;
            it.second->set<Pos2D>(pos);
        }
        else {
            pos = it.second->get<Pos2D>();
            pos.Y = size_.Height - pos.Y;
            it.second->set<Pos2D>(pos);
        }
    }
    return *this;
}

Sprite& Menu::getSprite(std::string const& name)
{
    if( pSprite temp = sprites_[name] )
        return *temp;

    return *this;
}

SpriteText& Menu::getSpriteText(std::string const& name)
{
    if( pSpriteText temp = dynamic_pointer_cast<SpriteText>( sprites_[name] ) )
        return *temp;

    std::cout << "You accessed a SpriteText, which is not in this Menu." << std::endl;
    return *pSpriteText();
}

AnimatedSprite& Menu::getAnimSprite(std::string const& name)
{
    if( pAnimatedSprite temp = dynamic_pointer_cast<AnimatedSprite>( sprites_[name] ) )
        return *temp;

    std::cout << "You accessed a AnimatedSprite, which is not in this Menu." << std::endl;
    return *pAnimatedSprite();
}

pMenu Menu::init(pObject const& parent, int const& w, int const& h)
{
    Sprite::init(parent, w, h);
    return static_pointer_cast<Menu>( shared_from_this() );
}
