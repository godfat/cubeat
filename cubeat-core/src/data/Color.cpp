
#include "data/Color.hpp"
#include "utils/Random.hpp"
#include "Conf.hpp"

using namespace psc;
using namespace data;



int const Color::id2rgb[] = {-1, blue,         green,        red,
                                 red_green, red_blue, green_blue,
                                 0,   white};

int const Color::MAX = 9; //make it the same as id2rgb.

Color& Color::offset()
{
    static utils::map_any offt = Conf::i().config_of("data/color");
    switch ( rgb() ) {
        case Color::red:
            r( r() + offt.V("red").I(0) );
            g( g() + offt.V("red").I(1) );
            b( b() + offt.V("red").I(2) ); break;
        case Color::green:
            r( r() + offt.V("green").I(0) );
            g( g() + offt.V("green").I(1) );
            b( b() + offt.V("green").I(2) ); break;
        case Color::blue:
            r( r() + offt.V("blue").I(0) );
            g( g() + offt.V("blue").I(1) );
            b( b() + offt.V("blue").I(2) ); break;
        case Color::red_green: //yellow
            r( r() + offt.V("yellow").I(0) );
            g( g() + offt.V("yellow").I(1) );
            b( b() + offt.V("yellow").I(2) ); break;
    }
    return *this;
}

Color Color::from_id(int const& i, int const& limit) {
    int rgb;
    switch(i) {
        case  0: return from_id( utils::random(limit)+1 ); break;
        default: rgb = id2rgb[i];
    }
    return Color(rgb);
}

Color Color::random_rgb_color() {
    return Color(utils::random(white+1));
}
