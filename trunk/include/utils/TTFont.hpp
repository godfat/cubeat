#ifndef _SHOOTING_CUBES_TRUETYPE_FONT_
#define _SHOOTING_CUBES_TRUETYPE_FONT_

#include "CGUITTFont.h"  //include from irrlicht

namespace psc {
namespace utils {

class TTFont : public irr::gui::CGUITTFont
{
public:
    irr::video::ITexture* getTextureFromText(const wchar_t* text, const irr::c8* name);

protected:

};

} //psc
} //utils

#endif // _SHOOTING_CUBES_TTFONT_
