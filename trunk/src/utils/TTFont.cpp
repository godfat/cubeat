
#include "utils/TTFont.hpp"

using namespace irr;
using namespace core;
using namespace gui;
using namespace video;

using namespace psc;
using namespace utils;

ITexture* TTFont::getTextureFromText(const wchar_t* text, const c8* name)
{
    if( !AntiAlias ) AntiAlias = true;      //force this texture to be 32bit anti-aliased
    dimension2di size = getDimension(text);
    position2di offset= position2di(0,0);

    s32 w = 1, h = 1;
    while( w < size.Width ) {w <<= 1;} size.Width = w;
    while( h < size.Height) {h <<= 1;} size.Height = h;

    ITexture* texture = Driver->addTexture(size, name);

    u32* pixel_o = (u32*)texture->lock();
    if( !pixel_o ) { texture->unlock(); return texture; } //failed, empty texture

    for( int i = 0; i < size.Height; ++i ) {
        int yo = i * size.Width;
        for( int j = 0; j < size.Width; ++j )
            pixel_o[ yo + j ] = 0;
    }

    unsigned int n = 0;
    while( *text ) {
        if( (n = getGlyphIndex(*text)) ) {
            n -= 1;  // 0 means no glyph, so all glyph index must dec by 1 to fit 0-based array
            s32 texw = Glyphs[n].texw;
            s32 texh = Glyphs[n].texh;
            s32 offx = Glyphs[n].left;
            s32 offy = Glyphs[n].size - Glyphs[n].top - 1;
            u8* pixel_i = Glyphs[n].image;
            int bound = Glyphs[n].size;
            for(int h = 0; h < texh; ++h) {
                int ypos = h + offy;
                if( ypos < 0 )           ypos = 0;
                else if( ypos >= bound ) ypos = bound-1;
                int yo = ypos*size.Width;
                for(int w = 0; w < texw; ++w) {
                    int current_pos = yo + offset.X + w + offx;
                    if( *pixel_i )
                        pixel_o[ current_pos ] = ((*pixel_i) << 24) | 0x00ffffff;
                    else
                        pixel_o[ current_pos ] = 0; //else set to all black & transparent
                    ++pixel_i;
                }
            }
        }
        offset.X += getWidthFromCharacter(*text) + GlobalKerningWidth;
        ++text;
    }

    texture->unlock();

    return texture;
}
