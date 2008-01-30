#include "irrlicht.h"
#include "CGUITTFont.h"
// >> Add by uirou for Multilingual start
#include "os.h"
// << Add by uirou for Multilingual end
namespace irr
{
namespace gui
{

CGUITTGlyph::CGUITTGlyph() : IReferenceCounted()
{
	tex = NULL;
	tex16 = NULL;
	image = NULL;
}

CGUITTGlyph::~CGUITTGlyph()
{
	if (image) delete image;
	if (tex){
		Driver->removeTexture(tex);
	}
	if (tex16){
		Driver->removeTexture(tex16);
	}
}

// >> add solehome's code for memory access error begin
void CGUITTGlyph::init()
{
   #ifdef _DEBUG
   setDebugName("CGUITTGlyph");
   #endif

   tex = NULL;
   tex16 = NULL;
   image = NULL;
}
// << add solehome's code for memory access error end

void CGUITTGlyph::cache(u32 idx)
{
	FT_Set_Pixel_Sizes(*face,0,size);
	if (!FT_Load_Glyph(*face,idx,FT_LOAD_NO_HINTING|FT_LOAD_NO_BITMAP)){
		FT_GlyphSlot glyph = (*face)->glyph;
		FT_Bitmap  bits;
		if (glyph->format == ft_glyph_format_outline ){
			if (!FT_Render_Glyph( glyph, FT_RENDER_MODE_NORMAL)){
				bits = glyph->bitmap;
				u8 *pt = bits.buffer;
				image = new u8[bits.width * bits.rows];
				memcpy(image,pt,bits.width * bits.rows);
				top = glyph->bitmap_top;
				left = glyph->bitmap_left;
				imgw = 1;
				imgh = 1;
				texw = bits.width;
				texh = bits.rows;
				for(;;){
					if (imgw > texw){
						break;
					} else {
						imgw <<= 1;
					}
				}
				for(;;){
					if (imgh > texh){
						break;
					} else {
						imgh <<= 1;
					}
				}
				if (imgw > imgh){
					imgh = imgw;
				} else {
					imgw = imgh;
				}
				u32 *texd = new u32[imgw*imgh];
				memset(texd,0,imgw*imgh*sizeof(u32));
				u32 *texp = texd;
				offset = size - bits.rows;
				bool cflag = (Driver->getDriverType() == video::EDT_DIRECT3D8);
				for (int i = 0;i < bits.rows;i++){
					u32 *rowp = texp;
					for (int j = 0;j < bits.width;j++){
						if (*pt){
							if (cflag){
								*rowp = *pt;
								*rowp *= 0x01010101;
							} else {
								*rowp = *pt << 24;
								*rowp |= 0xffffff;
							}
						} else {
							*rowp = 0;
						}
						pt++;
						rowp++;
					}
					texp += imgw;
				}
				c8 name[128];
				sprintf(name,"TTFontGlyph%d",idx);
				video::IImage *img = Driver->createImageFromData(video::ECF_A8R8G8B8,core::dimension2d<s32>(imgw,imgh),texd);
				bool flg16 = Driver->getTextureCreationFlag(video::ETCF_ALWAYS_16_BIT);
				bool flg32 = Driver->getTextureCreationFlag(video::ETCF_ALWAYS_32_BIT);
				bool flgmip = Driver->getTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS);
				Driver->setTextureCreationFlag(video::ETCF_ALWAYS_16_BIT,false);
				Driver->setTextureCreationFlag(video::ETCF_ALWAYS_32_BIT,true);
				Driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS,false);
				if (tex){
					Driver->removeTexture(tex);
				}
				tex = Driver->addTexture(name,img);
				img->drop();
//				tex->drop();
				Driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS,flgmip);
				Driver->setTextureCreationFlag(video::ETCF_ALWAYS_32_BIT,flg32);
				Driver->setTextureCreationFlag(video::ETCF_ALWAYS_16_BIT,flg16);
				delete texd;
				cached = true;
			}
		}
	}
	if (!FT_Load_Glyph(*face,idx,FT_LOAD_NO_HINTING|FT_LOAD_RENDER|FT_LOAD_MONOCHROME)){
		FT_GlyphSlot glyph = (*face)->glyph;
		FT_Bitmap bits = glyph->bitmap;
		u8 *pt = bits.buffer;
		top16 = glyph->bitmap_top;
		left16 = glyph->bitmap_left;
		imgw16 = 1;
		imgh16 = 1;
		texw16 = bits.width;
		texh16 = bits.rows;
		for(;;){
			if (imgw16 >= texw16){
				break;
			} else {
				imgw16 <<= 1;
			}
		}
		for(;;){
			if (imgh16 >= texh16){
				break;
			} else {
				imgh16 <<= 1;
			}
		}
		if (imgw16 > imgh16){
			imgh16 = imgw16;
		} else {
			imgw16 = imgh16;
		}
		u16 *texd16 = new u16[imgw16*imgh16];
		memset(texd16,0,imgw16*imgh16*sizeof(u16));
		u16 *texp16 = texd16;
		offset = size - bits.rows;
		for (int y = 0;y < bits.rows;y++){
			u16 *rowp = texp16;
			for (int x = 0;x < bits.width;x++){
				if (pt[y * bits.pitch + (x / 8)] & (0x80 >> (x % 8))){
					*rowp = 0xffff;
				}
				rowp++;
			}
			texp16 += imgw16;
		}
		c8 name[128];
		sprintf(name,"TTFontGlyph%d_16",idx);
		video::IImage *img = Driver->createImageFromData(video::ECF_A1R5G5B5,core::dimension2d<s32>(imgw16,imgh16),texd16);
		bool flg16 = Driver->getTextureCreationFlag(video::ETCF_ALWAYS_16_BIT);
		bool flg32 = Driver->getTextureCreationFlag(video::ETCF_ALWAYS_32_BIT);
		bool flgmip = Driver->getTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS);
		Driver->setTextureCreationFlag(video::ETCF_ALWAYS_16_BIT,true);
		Driver->setTextureCreationFlag(video::ETCF_ALWAYS_32_BIT,false);
		Driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS,false);
		tex16 = Driver->addTexture(name,img);
		img->drop();
		Driver->makeColorKeyTexture(tex16,video::SColor(0,0,0,0));
		Driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS,flgmip);
		Driver->setTextureCreationFlag(video::ETCF_ALWAYS_32_BIT,flg32);
		Driver->setTextureCreationFlag(video::ETCF_ALWAYS_16_BIT,flg16);
		delete texd16;

        //added
        cached = true;
	}
}

//! constructor
CGUITTFont::CGUITTFont(IGUIEnvironment *env)
// >> Modified by MadHyde for Ver.1.3 new functions begin
: Driver(0), GlobalKerningWidth(0), GlobalKerningHeight(0)
//: Driver(driver)
// << Modified by MadHyde for Ver.1.3 new functions end
{
	#ifdef _DEBUG
	setDebugName("CGUITTFont");
	#endif

// >> Add by MadHyde for Ver.1.3 begin
	if (env)
	{
		// don't grab environment, to avoid circular references
		Driver = env->getVideoDriver();
	}
// << Modified by MadHyde for Ver.1.3 end

	if (Driver)
		Driver->grab();
	AntiAlias = false;
	TransParency = false;
	attached = false;
}



//! destructor
CGUITTFont::~CGUITTFont()
{
//	for (int i = 0;i < Glyphs.size();i++){
//		if (Glyphs[i].cached){
//		}
//	}
	if (attached)	tt_face->drop();
	attached = false;
	if (Driver)	Driver->drop();
}

CGUITTFace::CGUITTFace()
{
	#ifdef _DEBUG
	setDebugName("CGUITTFace");
	#endif
	loaded = false;
}

CGUITTFace::~CGUITTFace()
{
	if (loaded){
		FT_Done_Face(face);
		FT_Done_FreeType(library);
	}
}

//! loads a font file
bool CGUITTFace::load(const c8* filename)
{
	if (FT_Init_FreeType( &library )){
// >> Add by uirou for Multilingual start
		os::Printer::log("FT_Init_FreeType() fail.");
// << Add by uirou for Multilingual end
		return	false;
	}
	if (FT_New_Face( library,filename,0,&face )){
// >> Add by uirou for Multilingual start
		os::Printer::log("FT_New_Face() fail.");
// << Add by uirou for Multilingual end
		return	false;
	}
// >> Add by uirou for Multilingual start
#ifdef USE_ICONV
	// XXXX WARNING magic ward "UTF-8"...
	if ((cd = iconv_open("UTF-8", nl_langinfo(CODESET))) < 0){
		os::Printer::log("iconv(\"UTF-8\", this locale codeset); fail.", ELL_WARNING);
		FT_Done_Face(face);
		return	false;
	}
	// XXXX WARNING magic ward "ft_encoding_unicode".
	for(int i = 0; i < face->num_charmaps; i++){
		char *cp = (char *)&(face->charmaps[i]->encoding);
		if(face->charmaps[i]->encoding == ft_encoding_unicode){
			if (FT_Set_Charmap(face, face->charmaps[i]) == 0){
				loaded = true;
				return true;
			}
		}
	}
	os::Printer::log("can not set codeset of unicode in this font.", ELL_WARNING);
	FT_Done_Face(face);
	iconv_close(cd);
	return false;
#else
// << Add by uirou for Multilingual end
	loaded = true;
	return true;
// >> Add by uirou for Multilingual start
#endif
// << Add by uirou for Multilingual end
}

bool CGUITTFont::attach(CGUITTFace *Face,u32 size)
{
	if (!Driver)
		return false;

	if (attached){
		tt_face->drop();
	}
	tt_face = Face;
	tt_face->grab();

	this->size = size;

// >> add solehome's code for memory access error begin
	CGUITTGlyph tmp;  // for access the __vfptr of CGUITTGlyph
	int* tmp2;
// << add solehome's code for memory access error end

	Glyphs.reallocate(tt_face->face->num_glyphs);
	Glyphs.set_used(tt_face->face->num_glyphs);
	for (int i = 0;i < tt_face->face->num_glyphs;i++){
		Glyphs[i].Driver = Driver;
		Glyphs[i].size = size;
		Glyphs[i].face = &(tt_face->face);
		Glyphs[i].cached = false;
// >> add solehome's code for memory access error begin
		tmp2 = (int*)&(Glyphs[i]);
		*tmp2 = *(int*)(&tmp);// the __vfptr is arranged in the front of the memory block by VC++
		Glyphs[i].init(); // initialize the other memebers
// << add solehome's code for memory access error end
	}
	attached = true;
	return true;
}

u32 CGUITTFont::getGlyphIndex(const wchar_t c) const
{
// >> Add by uirou for Multilingual start
#ifdef USE_ICONV
	FT_ULong ft_char;
	char imb[32], omb[32], *ip, *op;
	size_t ilen, olen;

	ilen = wctomb(imb, c);
	if(ilen < 0){
	    os::Printer::log("wctomb fail.", ELL_WARNING);
	    return 0;
	}
	imb[ilen] = '\0';

	ip = imb; op = omb;
	olen = sizeof(omb);
	// XXX need iconv state flash "iconv(cd, NULL, NULL, NULL, NULL)"?
	iconv(tt_face->cd, (const char **)&ip, &ilen, &op, &olen);
	if(sizeof(omb) <= olen){
	    os::Printer::log("iconv fail", ELL_WARNING);
	    return 0;
	}

	olen = sizeof(omb) - olen;
	ft_char = 0;

	// UTF-8 to unicode... 
	if((omb[0] & 0x80) == 0){ // 1byte
	    ft_char = omb[0];
	}else if((omb[0] & 0xe0) == 0xc0){ // 2bytes
	    ft_char = (omb[0] & 0x1f); ft_char <<= 6;
	    ft_char += omb[1] & 0x3f;
	}else if((omb[0] & 0xf0) == 0xe0){ // 3bytes
	    ft_char = (omb[0] & 0x0f); ft_char <<= 6;
	    ft_char += omb[1] & 0x3f; ft_char <<= 6;
	    ft_char += omb[2] & 0x3f;
	}else if((omb[0] & 0xf8) == 0xf0){ // 4bytes
	    ft_char = (omb[0] & 0x07); ft_char <<= 6;
	    ft_char += omb[1] & 0x3f; ft_char <<= 6;
	    ft_char += omb[2] & 0x3f; ft_char <<= 6;
	    ft_char += omb[3] & 0x3f;
	}else if((omb[0] & 0xfc) == 0xf8){ // 5bytes
	    ft_char = (omb[0] & 0x03); ft_char <<= 6;
	    ft_char += omb[1] & 0x3f; ft_char <<= 6;
	    ft_char += omb[2] & 0x3f; ft_char <<= 6;
	    ft_char += omb[3] & 0x3f; ft_char <<= 6;
	    ft_char += omb[4] & 0x3f;
	}else if((omb[0] & 0xfe) == 0xfc){ // 6bytes
	    ft_char = (omb[0] & 0x01); ft_char <<= 6;
	    ft_char += omb[1] & 0x3f; ft_char <<= 6;
	    ft_char += omb[2] & 0x3f; ft_char <<= 6;
	    ft_char += omb[3] & 0x3f; ft_char <<= 6;
	    ft_char += omb[4] & 0x3f; ft_char <<= 6;
	    ft_char += omb[5] & 0x3f;
	}
	u32 idx = FT_Get_Char_Index( tt_face->face, ft_char );
#else
// << Add by uirou for Multilingual end
	u32 idx = FT_Get_Char_Index( tt_face->face, c );
// >> Add by uirou for Multilingual start
#endif
// << Add by uirou for Multilingual end
	if (idx && !Glyphs[idx - 1].cached)	
        Glyphs[idx - 1].cache(idx);

	return idx;
}


//! returns the dimension of a text
core::dimension2d<s32> CGUITTFont::getDimension(const wchar_t* text) const
{
// >> Modified by MadHyde for Ver.1.3 begin
	core::dimension2d<s32> dim(0, 0);
	core::dimension2d<s32> thisLine(0, Glyphs[0].size);

	for (const wchar_t* p = text; *p; ++p)
	{
		bool lineBreak=false;
		if (*p == L'\r') // Mac or Windows breaks
		{
			lineBreak = true;
			if (p[1] == L'\n') // Windows breaks
				++p;
		}
		else if (*p == L'\n') // Unix breaks
		{
			lineBreak = true;
		}
		if (lineBreak)
		{
			dim.Height += thisLine.Height;
			if (dim.Width < thisLine.Width)
				dim.Width = thisLine.Width;
			thisLine.Width = 0;
			continue;
		}
        thisLine.Width += getWidthFromCharacter(*p) + GlobalKerningWidth;
	}

	dim.Height += thisLine.Height;
	if (dim.Width < thisLine.Width)
		dim.Width = thisLine.Width;

	return dim;
// << Modified by MadHyde for Ver.1.3 end
}


s32 CGUITTFont::getWidthFromCharacter(const wchar_t c) const
{
	u32 n = getGlyphIndex(c);
	if ( n > 0){
		int w = Glyphs[n - 1].texw;
		s32 left = Glyphs[n - 1].left;
		if (w + left > 0) return w + left;
	}
	if (c >= 0x2000){
		return Glyphs[0].size;
	} else {
		return Glyphs[0].size / 2;
	}
}


//! draws an text and clips it to the specified rectangle if wanted
void CGUITTFont::draw(const wchar_t* text, const core::rect<s32>& position, video::SColor color, bool hcenter, bool vcenter, const core::rect<s32>* clip)
{
	if (!Driver)
		return;

	core::dimension2d<s32> textDimension;
	core::position2d<s32> offset = position.UpperLeftCorner;
	video::SColor colors[4];

	for (int i = 0;i < 4;i++){
		colors[i] = color;
	}

    if (hcenter || vcenter)
	{
		textDimension = getDimension(text);

		if (hcenter)
			offset.X = ((position.getWidth() - textDimension.Width)>>1) + offset.X;

		if (vcenter)
			offset.Y = ((position.getHeight() - textDimension.Height)>>1) + offset.Y;
	}

	u32 n;
	
	while(*text)
	{
		n = getGlyphIndex(*text);
		if ( n > 0){
			if (AntiAlias){
				s32 imgw = Glyphs[n-1].imgw;
				s32 imgh = Glyphs[n-1].imgh;
				s32 texw = Glyphs[n-1].texw;
				s32 texh = Glyphs[n-1].texh;
				s32 offx = Glyphs[n-1].left;
				s32 offy = Glyphs[n-1].size - Glyphs[n-1].top;
				if (Driver->getDriverType() != video::EDT_SOFTWARE){
					if (!TransParency)	color.color |= 0xff000000;
					Driver->draw2DImage(Glyphs[n-1].tex,core::position2d<s32>(offset.X+offx,offset.Y+offy),core::rect<s32>(0,0,imgw-1,imgh-1),clip,color,true);
				} else {
					s32 a = color.getAlpha();
					s32 r = color.getRed();
					s32 g = color.getGreen();
					s32 b = color.getBlue();
					u8 *pt = Glyphs[n-1].image;
					if (!TransParency)	a = 255;
					for (int y = 0;y < texh;y++){
						for (int x = 0;x < texw;x++){
							if (!clip || clip->isPointInside(core::position2d<s32>(offset.X+x+offx,offset.Y+y+offy))){
								if (*pt){
									Driver->draw2DRectangle(video::SColor((a * *pt)/255,r,g,b),core::rect<s32>(offset.X+x+offx,offset.Y+y+offy,offset.X+x+offx+1,offset.Y+y+offy+1));
								}
								pt++;
							}
						}
					}
				}
			} else {
				s32 imgw = Glyphs[n-1].imgw16;
				s32 imgh = Glyphs[n-1].imgh16;
				s32 texw = Glyphs[n-1].texw16;
				s32 texh = Glyphs[n-1].texh16;
				s32 offx = Glyphs[n-1].left16;
				s32 offy = Glyphs[n-1].size - Glyphs[n-1].top16;
				if (!TransParency){
					color.color |= 0xff000000;
				}
				Driver->draw2DImage(Glyphs[n-1].tex16,core::position2d<s32>(offset.X+offx,offset.Y+offy),core::rect<s32>(0,0,imgw-1,imgh-1),clip,color,true);
			}
// >> Modified by MadHyde for Ver.1.3 new functions begin
			offset.X += getWidthFromCharacter(*text) + GlobalKerningWidth;
// << Modified by MadHyde for Ver.1.3 new functions end
		} else {
// >> Modified by MadHyde for Ver.1.3 new functions begin
			offset.X += getWidthFromCharacter(*text) + GlobalKerningWidth;
// << Modified by MadHyde for Ver.1.3 new functions end
		}
		++text;
	}
}


//! Calculates the index of the character in the text which is on a specific position.
s32 CGUITTFont::getCharacterFromPos(const wchar_t* text, s32 pixel_x) const
{
	s32 x = 0;
	s32 idx = 0;

	while (text[idx])
	{
		x += getWidthFromCharacter(text[idx]);

		if (x >= pixel_x)
			return idx;

		++idx;
	}
	return -1;
}

// >> Add by MadHyde for Ver.1.3 new funtcions begin
//! set an Pixel Offset on Drawing ( scale position on width )
void CGUITTFont::setKerningWidth ( s32 kerning )
{
	GlobalKerningWidth = kerning;
}

//! set an Pixel Offset on Drawing ( scale position on width )
s32 CGUITTFont::getKerningWidth(const wchar_t* thisLetter, const wchar_t* previousLetter) const
{
	// TODO - overhang? underhang? 10 Jul 2007 - MadHyde
	return GlobalKerningWidth;
}

//! set an Pixel Offset on Drawing ( scale position on height )
void CGUITTFont::setKerningHeight ( s32 kerning )
{
	GlobalKerningHeight = kerning;
}

//! set an Pixel Offset on Drawing ( scale position on height )
s32 CGUITTFont::getKerningHeight () const
{
	return GlobalKerningHeight;
}
// << Add by MadHyde for Ver.1.3 new functions end

scene::ISceneNode*
CGUITTFont::createBillboard(const wchar_t* text,scene::ISceneManager *scene,scene::ISceneNode *parent,s32 id)
{
	scene::ISceneNode *node = scene->addEmptySceneNode(parent,id);
	
	core::dimension2d<s32> textDimension;
	textDimension = getDimension(text);
	core::vector3df offset = core::vector3df(0.0f,0.0f,0.0f);

	offset.X = 0.0f - (textDimension.Width >> 1);
	offset.Y = 0.0f - (textDimension.Height >>1);

	u32 n;
	while(*text)
	{
		if (n = getGlyphIndex(*text)){
			s32 imgw,imgh,texw,texh,offx,offy;
			video::ITexture *tex;
			if (AntiAlias){
				imgw = Glyphs[n-1].imgw;
				imgh = Glyphs[n-1].imgh;
				texw = Glyphs[n-1].texw;
				texh = Glyphs[n-1].texh;
				offx = Glyphs[n-1].left;
				offy = Glyphs[n-1].size - Glyphs[n-1].top;
				tex = Glyphs[n-1].tex;
			} else {
				imgw = Glyphs[n-1].imgw16;
				imgh = Glyphs[n-1].imgh16;
				texw = Glyphs[n-1].texw16;
				texh = Glyphs[n-1].texh16;
				offx = Glyphs[n-1].left16;
				offy = Glyphs[n-1].size - Glyphs[n-1].top16;
				tex = Glyphs[n-1].tex16;
			}
		}
		offset.X += getWidthFromCharacter(*text);
		++text;
	}
	return node;
}
#if	0

#endif
} // end namespace gui
} // end namespace irr
