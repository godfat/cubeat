#ifndef TTFONTTEST_HPP_INCLUDED
#define TTFONTTEST_HPP_INCLUDED

#include "CGUITTFont.h"

namespace irr {
namespace gui {

class TTFontTest : public CGUITTFont
{
public:
    video::ITexture* getTextureFromText(const wchar_t* text, const irr::c8* name);

protected:

};

} //gui
} //irr

#endif // TTFONTTEST_HPP_INCLUDED
