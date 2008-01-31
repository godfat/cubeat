
#include "view/SpriteText.hpp"

#ifdef _IRR_WINDOWS_
#include <windows.h>
#endif

#include <algorithm> //for the ugly copy
#include <iostream>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;

using namespace psc;
using namespace view;

//still some problem with material settings. I'll fix that later.
void SpriteText::init(std::string const& text, std::string const& font_path,
                      int size, pObject const& parent)
{
    if( text.size() < 1 ) {
        Object::init(parent);
        return;
    }
/*
    video::IVideoDriver* driver = smgr_->getVideoDriver();

    SMaterial mat_;
    mat_.setFlag(video::EMF_LIGHTING, true);
    mat_.setTexture(0, driver->getTexture(""));

    mat_.MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL;
    mat_.MaterialTypeParam = 0.01f;

    mat_.DiffuseColor.set(255,255,255,255);
*/
#ifdef _IRR_WINDOWS_
//<MLtest>
	gui::CGUITTFont* ttfont;
    c8 tmp[512];
    GetWindowsDirectory(tmp,511);
	strcat(tmp, font_path.c_str());
	ttfont = (gui::CGUITTFont *)smgr_->getGUIEnvironment()->getFont(tmp,size);
    ttfont->AntiAlias = true;
    std::wstring temp(text.length(),L' ');             //so hard to convert between wchar and char @@
    std::copy(text.begin(), text.end(), temp.begin()); //so hard to convert between wchar and char @@

    //somebody dropped it twice, so the program crashed on exit.
    ttfont->grab();
//</MLtest>
    body_ = smgr_->addTextSceneNode(ttfont,
#else
    body_ = smgr_->addTextSceneNode(smgr_->getGUIEnvironment()->getBuiltInFont(),
#endif
                                    temp.c_str(),
                                    video::SColor(255,100,100,100), parent->body());

    body_->setScale(vector3df(10.0/5.0,1.0,1.0));

    //body_->getMaterial(0) = mat_;
    press_.setOwner( shared_from_this() );
    scene_ = parent->scene();
}
