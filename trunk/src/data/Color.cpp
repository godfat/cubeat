
#include "data/Color.hpp"

using namespace psc;
using namespace data;

int const Color::id2rgb[] = {-1, blue,         green,        red,
                                 red_green, red_blue, green_blue,
                                 0,   white};

utils::vector_any Color::offt_ = utils::vector_any::construct( utils::fetchConfig("config/data/color.zzml") );

Color& Color::offset()
{
    switch ( rgb() ) {
        case Color::red:
            r( r() + offt_.V(0).I(0) );
            g( g() + offt_.V(0).I(1) );
            b( b() + offt_.V(0).I(2) ); break;
        case Color::green:
            r( r() + offt_.V(1).I(0) );
            g( g() + offt_.V(1).I(1) );
            b( b() + offt_.V(1).I(2) ); break;
        case Color::blue:
            r( r() + offt_.V(2).I(0) );
            g( g() + offt_.V(2).I(1) );
            b( b() + offt_.V(2).I(2) ); break;
        case Color::red_green: //yellow
            r( r() + offt_.V(3).I(0) );
            g( g() + offt_.V(3).I(1) );
            b( b() + offt_.V(3).I(2) ); break;
    }
    return *this;
}
