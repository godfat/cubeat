
/* This file is added by arch_jslin 2008.11.02
   purpose: to supplement TrueType font class a proper interface in Irrlicht */

#ifndef __I_GUI_TTFONT_H_INCLUDED__
#define __I_GUI_TTFONT_H_INCLUDED__

#include <irrlicht.h>

namespace irr
{
namespace gui
{

class IGUITTFont : public IGUIFont
{
public:
	//! destructor
	virtual ~IGUITTFont() {}

// added by arch_jslin 2008.11.02
    virtual u32 getFontSize() const =0;
    virtual void setAntiAlias(bool const& flag) = 0;
    virtual void setTransparent(bool const& flag) = 0;
    virtual bool isAntiAlias()   const = 0;
    virtual bool isTransparent() const = 0;
    virtual bool isAttached()    const = 0;
    virtual irr::video::ITexture* getTextureFromText(const wchar_t* text, const irr::c8* name) = 0;
	virtual scene::ISceneNode *createBillboard(const wchar_t* text,scene::ISceneManager *scene,
	                                           scene::ISceneNode *parent = 0,s32 id = -1) = 0;
};

} // end namespace gui
} // end namespace irr

#endif


