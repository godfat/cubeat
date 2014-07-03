
#include "data/Color.hpp"
#include "Conf.hpp"

using namespace psc;
using namespace data;


int const Color::id2rgb[] = {-1, blue,         green,        red,
                                 red_green, red_blue, green_blue,
                                 0,   white};
//                                                  r             g          b
int const Color::id2tuned_rgb[] = {-1, blue      + (50 << 16) + ( 50 << 8) + 0,
                                       green     + (0  << 16) + (-30 << 8) + 0,
                                       red       + (0  << 16) + ( 30 << 8) + 20,
                                       red_green + (0  << 16) + ( 0  << 8) + 20,
                                       red_blue, green_blue, 0, white};

// reference: 0xff553300                                                  aarrggbb   aa should be ignored
int const Color::id2tuned_rgb_bright1[] = {-1, Color::id2tuned_rgb[1] | 0x00104000,
                                               Color::id2tuned_rgb[2] | 0x0040ff00,
                                               Color::id2tuned_rgb[3] | 0x00004000,
                                               Color::id2tuned_rgb[4] | 0x00000018,
                                               red_blue, green_blue, 0, white};

// reference: 0xffaa7744                                                  aarrggbb   aa should be ignored
int const Color::id2tuned_rgb_bright2[] = {-1, Color::id2tuned_rgb[1] | 0x00307700,
                                               Color::id2tuned_rgb[2] | 0x0080ff20,
                                               Color::id2tuned_rgb[3] | 0x00006030,
                                               Color::id2tuned_rgb[4] | 0x00000050,
                                               red_blue, green_blue, 0, white};

int Color::get_bright1_rgb() const
{
    return id2tuned_rgb_bright1[id_];
}

int Color::get_bright1_argb() const
{
    return 0xff000000 | get_bright1_rgb();
}

int Color::get_bright2_rgb() const
{
    return id2tuned_rgb_bright2[id_];
}

int Color::get_bright2_argb() const
{
    return 0xff000000 | get_bright2_rgb();
}

Color& Color::offset()
{
    switch ( rgb() ) {
        case Color::red:       rgb_ = id2tuned_rgb[3]; break;
        case Color::green:     rgb_ = id2tuned_rgb[2]; break;
        case Color::blue:      rgb_ = id2tuned_rgb[1]; break;
        case Color::red_green: rgb_ = id2tuned_rgb[4]; break;
        default: break;
    }
    return *this;
}
