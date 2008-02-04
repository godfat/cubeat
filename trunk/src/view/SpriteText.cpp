
#include "view/SpriteText.hpp"
#include "view/Scene.hpp"
#include "Accessors.hpp"

#ifdef _IRR_WINDOWS_
#include <windows.h>
#endif

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

    SColor col( color.rgb() );
    col.setAlpha( 255 );
#ifdef _IRR_WINDOWS_
//<MLtest>
	gui::CGUITTFont* ttfont;
    c8 tmp[512];
    GetWindowsDirectory(tmp,511);
    strcat(tmp, "/");
	strcat(tmp, font_path.c_str());
	ttfont = (gui::CGUITTFont *)smgr_->getGUIEnvironment()->getFont(tmp,size);
    ttfont->AntiAlias = true;
    ttfont->TransParency = true;
    std::wstring temp(text.length(),L' ');             //so hard to convert between wchar and char @@
    std::copy(text.begin(), text.end(), temp.begin()); //so hard to convert between wchar and char @@
    ttfont->grab();

//</MLtest>
    body_ = smgr_->addTextSceneNode(ttfont,
#else
    body_ = smgr_->addTextSceneNode(smgr_->getGUIEnvironment()->getFont("rc/fonts/fontlucida.png"),
#endif
                                    temp.c_str(), col, parent->body());

    press_.setOwner( shared_from_this() );
    scene_ = parent->scene();
}

SpriteText& SpriteText::setCenterAligned(bool const& center)
{
    if( center_ == center ) return *this;
    center_ = center;
    if( center_ )
        static_cast<ITextSceneNode*>(body_)->setCenter( true, true );
    else
        static_cast<ITextSceneNode*>(body_)->setCenter( false, false );
    return *this;
}

SpriteText::~SpriteText()
{
}
