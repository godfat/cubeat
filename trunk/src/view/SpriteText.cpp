
#include "view/SpriteText.hpp"
#include "view/Scene.hpp"
#include "Accessors.hpp"
#include "test/TTFontTest.hpp"

#include <sstream>
#include <algorithm> //for the ugly copy

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace accessor;

using namespace psc;
using namespace view;

void SpriteText::init(std::string const& text, std::string const& font_path,
                      int size, data::Color const& color, pObject const& parent)
{
    if( text.size() < 1 ) {
        Object::init(parent);
        return;
    }

    std::ostringstream oss;
    oss << "rc/fonts/" << font_path << ".ttf";

    SColor col( color.rgb() );
    col.setAlpha( 255 );

//	gui::CGUITTFont* ttfont;
    gui::TTFontTest* ttfont;
	ttfont = (gui::TTFontTest *)smgr_->getGUIEnvironment()->getFont(oss.str().c_str(),size);
    ttfont->AntiAlias = true;
    ttfont->TransParency = true;
    std::wstring temp(text.length(),L' ');             //so hard to convert between wchar and char @@
    std::copy(text.begin(), text.end(), temp.begin()); //so hard to convert between wchar and char @@

    font_texture_ = ttfont->getTextureFromText(temp.c_str(), "test_texture");
    dimension2di size_int = ttfont->getDimension(temp.c_str());
    size_.Width = size_int.Width;
    size_.Height= size_int.Height;

    SMaterial mat;
    mat.setFlag(video::EMF_LIGHTING, true);
    mat.setTexture(0, font_texture_);

    mat.MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL;
    mat.MaterialTypeParam = 0.01f;

    mat.DiffuseColor.set(200,200,200,200);

    setupMeshBase(parent);
    body_->getMaterial(0) = mat;

    press_.setOwner( shared_from_this() );
    scene_ = parent->scene();
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
    thismesh_->drop();
}
