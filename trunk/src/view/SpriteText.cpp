
#include "view/SpriteText.hpp"
#include "utils/TTFont.hpp"

#include <sstream>
#include <algorithm> //for the ugly copy

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;

using namespace psc;
using namespace view;

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

    SMaterial mat;
    mat.setFlag(video::EMF_LIGHTING, true);
    mat.setFlag(video::EMF_ZWRITE_ENABLE, false);
    mat.setTexture(0, font_texture_);

    mat.MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL;
    mat.MaterialTypeParam = 0.01f;

    SColor col( color.rgb() );
    col.setAlpha( 255 );
    mat.DiffuseColor = col;

    setupMeshBase(parent);
    body_->getMaterial(0) = mat;

    adjust_texcoord_for_hand_made_texture(size_.Width, size_.Height);

    press_.setOwner( shared_from_this() );
    return std::tr1::static_pointer_cast<SpriteText>( shared_from_this() );
}

void SpriteText::createText(std::string const& text, std::string const& font_path, int const& size)
{
    gui::IGUIEnvironment* gui = IrrDevice::i().d()->getGUIEnvironment();
    //This is rather stupid. we have multiple SceneManager, but only one GUIEnvironment,
    //so smgr_->getGUIEnvironment() is NULL !? ...well, guess I have to live with that.

    std::ostringstream oss;
    oss << "rc/fonts/" << font_path << ".ttf";

    utils::TTFont* ttfont;
	ttfont = (utils::TTFont *)gui->getFont(oss.str().c_str(),size);
    ttfont->AntiAlias = true;
    ttfont->TransParency = true;
    std::wstring temp(text.length(),L' ');             //so hard to convert between wchar and char @@
    std::copy(text.begin(), text.end(), temp.begin()); //so hard to convert between wchar and char @@

    font_texture_ = ttfont->getTextureFromText(temp.c_str(), (text+"_font").c_str());
    dimension2di size_int = ttfont->getDimension(temp.c_str());
    size_.Width = size_int.Width;
    size_.Height= size_int.Height;
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
