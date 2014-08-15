
/*2007.11.9
  Nothing changed.
*/

#ifndef _SHOOTING_CUBE_COLOR_HPP_
#define _SHOOTING_CUBE_COLOR_HPP_
#include "utils/Random.hpp"
#include <algorithm> // for find
#include <iterator> // for distance

namespace psc{ namespace data{

int const COLOR_MAX = 9;

class Color{
    // index 0 is dummy..
    static int const id2rgb[];
    static int const id2tuned_rgb[];
    static int const id2tuned_rgb_bright1[];
    static int const id2tuned_rgb_bright2[];

public:
    static Color from_id(int i, int limit = 6){
        int rgb;
        switch(i){
            case  0: return from_id(random_id(limit)); break;
            default: rgb = id2rgb[i];
        }
        return Color(rgb, i);
    }

    static int for_id(Color const& color){
        return std::distance(id2rgb, std::find(id2rgb+1, id2rgb+8, color.rgb()));
    }
    static Color random_rgb_color(){
        return Color(utils::ya_random(white+1), 0);
    }

public:
    Color(): rgb_(0), id_(0) {}
    explicit Color(int rgb, int id): rgb_(rgb), id_(id){}
    Color(int r, int g, int b):
        rgb_( r<<16 | g<<8 | b ), id_(0)
    {}

    int rgb() const{ return rgb_; }
    int argb() const{ return 0xff000000 | rgb_; }
    int r() const{ return (rgb_&red) >> 16; }
    int g() const{ return (rgb_&green) >> 8; }
    int b() const{ return rgb_&blue; }
    int get_bright1_rgb() const;
    int get_bright1_argb() const;
    int get_bright2_rgb() const;
    int get_bright2_argb() const;

    Color& rgb(int new_rgb){ rgb_ = new_rgb; return *this; }
    Color& r(int new_r){ rgb_ &= green_blue; rgb_ |= new_r<<16; return *this; }
    Color& g(int new_g){ rgb_ &= red_blue; rgb_ |= new_g<<8; return *this; }
    Color& b(int new_b){ rgb_ &= red_green; rgb_ |= new_b; return *this; }

    Color& offset();

private:
    static int random_id(int limit){
        return utils::ya_random(limit)+1;
    }

private:
    int rgb_;
    int id_;
    enum{
        blue=0xff, green=blue<<8, red=green<<8,
        green_blue = green|blue,
        red_blue = red|blue,
        red_green = red|green,
        white = red|green|blue
    };
};

inline bool operator==(Color const& lhs, Color const& rhs){ return lhs.rgb() == rhs.rgb(); }
inline bool operator!=(Color const& lhs, Color const& rhs){ return lhs.rgb() != rhs.rgb(); }

}} // end of namespace

#endif
