#ifndef __C_GUI_TTFONT_H_INCLUDED__
#define __C_GUI_TTFONT_H_INCLUDED__

#include <ft2build.h>
#include <freetype/freetype.h>

#include <irrlicht.h>
// >> Add by uirou for Multilingual start
#ifdef USE_ICONV
#include <langinfo.h>
#include <iconv.h>
#endif
// << Add by uirou for Multilingual end

namespace irr
{
namespace gui
{

class CGUITTFace : public IReferenceCounted
{
public:
	CGUITTFace();
	virtual ~CGUITTFace();
	bool loaded;
	FT_Library	library;
	FT_Face		face;
// >> Add by uirou for Multilingual start
#ifdef USE_ICONV
	iconv_t cd;
#endif
// << Add by uirou for Multilingual end
	bool load(const c8* filename);
};

class CGUITTGlyph : public IReferenceCounted
{
public:
	video::IVideoDriver* Driver;
	CGUITTGlyph();
	virtual ~CGUITTGlyph();
// >> Add solehome's code for memory access error begin
	void init();
// << Add solehome's code for memory access error end
	void cache(u32 idx);

    bool cached;
    video::ITexture *tex;
    video::ITexture *tex16;
    u8 *image;
    FT_Face *face;
	u32 size;
	u32 top;
	u32 left;
	u32 texw;
	u32 texh;
	u32 imgw;
	u32 imgh;
	u32 top16;
	u32 left16;
	u32 texw16;
	u32 texh16;
	u32 imgw16;
	u32 imgh16;
	s32 offset;
};

class CGUITTFont : public IGUITTFont
{
public:
	//! constructor
	CGUITTFont(IGUIEnvironment *env);

	//! destructor
	virtual ~CGUITTFont();

	//! loads a truetype font file
	bool attach(CGUITTFace *Face,u32 size);

	//! draws an text and clips it to the specified rectangle if wanted
	virtual void draw(const wchar_t* text, const core::rect<s32>& position, video::SColor color, bool hcenter=false, bool vcenter=false, const core::rect<s32>* clip=0);

    //! returns the dimension of a text
	virtual core::dimension2d<s32> getDimension(const wchar_t* text) const;

	//! Calculates the index of the character in the text which is on a specific position.
	virtual s32 getCharacterFromPos(const wchar_t* text, s32 pixel_x) const;

// >> Add by MadHyde for Ver.1.3 new functions begin
	//! set an Pixel Offset on Drawing ( scale position on width )
	virtual void setKerningWidth (s32 kerning);
	virtual void setKerningHeight (s32 kerning);

	//! set an Pixel Offset on Drawing ( scale position on width )
	virtual s32 getKerningWidth(const wchar_t* thisLetter=0, const wchar_t* previousLetter=0) const;
	virtual s32 getKerningHeight() const;
// << Add by MadHyde for Ver.1.3 new functions end

// > added by arch_jslin 2008.11.02
    virtual u32 getFontSize() const { return size; }
    virtual void setAntiAlias(bool const& flag)   { AntiAlias = flag; }
    virtual void setTransparent(bool const& flag) { TransParency = flag; }
    virtual bool isAntiAlias()   const { return AntiAlias; }
    virtual bool isTransparent() const { return TransParency; }
    virtual bool isAttached()    const { return attached; }

    irr::video::ITexture* getTextureFromText(const wchar_t* text, const irr::c8* name);
// < end of arch_jslin modifications

	scene::ISceneNode *createBillboard(const wchar_t* text,scene::ISceneManager *scene,scene::ISceneNode *parent = 0,s32 id = -1);

// changed from private to protected by arch_jslin 2008.02.06
protected:
// properties moved to protected scope instead of public.
	u32 size;
	bool AntiAlias;
	bool TransParency;
	bool attached;

	virtual s32  getWidthFromCharacter(const wchar_t c) const;
    virtual u32  getGlyphIndex(const wchar_t c) const;
	video::IVideoDriver* Driver;
	mutable core::array< CGUITTGlyph > Glyphs;
	CGUITTFace *tt_face;
// >> Add by MadHyde for Ver.1.3 new functions begin
	s32 GlobalKerningWidth, GlobalKerningHeight;
// << Add by MadHyde for Ver.1.3 new functions end
};

} // end namespace gui
} // end namespace irr

#endif

