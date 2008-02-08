
#include "test/TTFontTest.hpp"

#include <iostream>

using namespace irr;
using namespace core;
using namespace gui;
using namespace video;

ITexture* TTFontTest::getTextureFromText(const wchar_t* text, const c8* name)
{
    if( !AntiAlias ) AntiAlias = true;      //force this texture to be 32bit anti-aliased
    dimension2di size = getDimension(text);
    position2di offset= position2di(0,0);
    ITexture* texture = Driver->addTexture(size, name);

    u32* pixel_o = (u32*)texture->lock();
    if( !pixel_o ) { texture->unlock(); return texture; } //failed, empty texture

    u32 n = 0;
    while( *text ) {
        if( (n = getGlyphIndex(*text)) ) {
            n -= 1;  // 0 means no glyph, so all glyph index must dec by 1 to fit 0-based array
            s32 texw = Glyphs[n].texw;
            s32 texh = Glyphs[n].texh;
            s32 offx = Glyphs[n].left;
            s32 offy = Glyphs[n].size - Glyphs[n].top;
            u8* pixel_i = Glyphs[n].image;

//            int yi      = 0;
//            //test code
//            std::cout << "texw: " << texw << ", texh: " << texh << ", glyph size: " << size.Height << "\n";
//            texture->unlock();
//            Glyphs[n].tex->unlock();
//            break;
            for(int h = 0; h < texh; ++h) {
                int yo = ( h + offy )*size.Width;
                for(int w = 0; w < texw; ++w) {
                    int current_pos = yo + offset.X + w + offx;
                    if( *pixel_i )
                        pixel_o[ current_pos ] = 0xffffffff; //set to all white
                    else
                        pixel_o[ current_pos ] = 0; //else set to all black & transparent
                    ++pixel_i;
                }
            }
            Glyphs[n].tex->unlock();
        }
        offset.X += getWidthFromCharacter(*text) + GlobalKerningWidth;
        ++text;
    }

    texture->unlock();

    return texture;
}
