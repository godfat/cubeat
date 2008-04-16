
#include "view/SpriteText.hpp"
#include "view/Scene.hpp"
#include "utils/TTFont.hpp"
#include "utils/to_s.hpp"
#include "Accessors.hpp"

#include <sstream>
#include <algorithm> //for the ugly copy

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;

using namespace psc;
using namespace view;
using namespace accessor;
using std::tr1::static_pointer_cast;

pSpriteText
SpriteText::init(std::string const& text, std::string const& font_path,
                 int const& size, data::Color const& color, pObject const& parent)
{
    if( text.size() < 1 ) {
        Object::init(parent);
        return std::tr1::static_pointer_cast<SpriteText>( shared_from_this() );
    }
    setupSceneAndManager(parent);
    createText( text, font_path, size );

    setupMeshBase(parent);

    SMaterial mat;
    mat.setFlag(video::EMF_LIGHTING, true);
    mat.setFlag(video::EMF_ZWRITE_ENABLE, false);
    mat.setTexture(0, font_texture_);

    mat.MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL;
    mat.MaterialTypeParam = 0.01f;

    SColor col( color.rgb() );
    col.setAlpha( 255 );
    mat.DiffuseColor = col;

    body_->getMaterial(0) = mat;

    adjust_texcoord_for_hand_made_texture(size_.Width, size_.Height);

    pSpriteText self = static_pointer_cast<SpriteText>( shared_from_this() );
    scene()->addPickMapping( body_, self );
    return self;
}

void SpriteText::createText(std::string const& text, std::string const& font_path, int const& size)
{
    gui::IGUIEnvironment* gui = IrrDevice::i().d()->getGUIEnvironment();
    //This is rather stupid. we have multiple SceneManager, but only one GUIEnvironment,
    //so smgr_->getGUIEnvironment() is NULL !? ...well, guess I have to live with that.

    std::ostringstream oss;
    oss << "rc/fonts/" << font_path << ".ttf";

	ttfont_ = (utils::TTFont *)gui->getFont(oss.str().c_str(),size); //bad c-cast. need to fix it.

    ttfont_->AntiAlias = true;
    ttfont_->TransParency = true;

    changeText( text );
}

SpriteText& SpriteText::changeText(std::string const& new_text)
{
    if( text_ == new_text ) return *this;
    text_ = new_text;
    std::wstring temp(new_text.length(),L' ');         //so hard to convert between wchar and char @@
    std::copy(new_text.begin(), new_text.end(), temp.begin()); //so hard to convert between wchar and char @@

    font_texture_ = ttfont_->getTextureFromText(temp.c_str(), (new_text+"_font").c_str());

    dimension2di size_int = ttfont_->getDimension(temp.c_str());
    size_.Width = size_int.Width;
    size_.Height= size_int.Height;
    if( body_ ) {
        body_->setMaterialTexture(0, font_texture_);
        set<Size2D>( vec2(size_.Width, size_.Height) );
        adjust_texcoord_for_hand_made_texture(size_.Width, size_.Height);
    }
    return *this;
}

SpriteText& SpriteText::showNumber(int num, unsigned int digit)
{
    std::string str = utils::to_s(num);
    while( str.size() < digit ) {
        str = "0" + str;
    }
    changeText( str );
    return *this;
}

SpriteText& SpriteText::setCenterAligned(bool const& center)
{
    if( center_ == center ) return *this;
    Sprite::setCenterAligned(center);
    body_->setMaterialTexture(0, font_texture_);
    body_->getMaterial(0).MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL;
    body_->getMaterial(0).MaterialTypeParam = 0.01f;
    return *this;
}

SpriteText::~SpriteText()
{
}
