
/* 2007.11.9
   Nothing changed.
*/

#ifndef _SHOOTING_CUBE_LOCATION_HPP_
#define _SHOOTING_CUBE_LOCATION_HPP_

#ifndef NDEBUG
#include <iostream>
#endif

struct Location{
    Location(): x(0), y(0), z(0){}
    Location(int xx, int yy, int zz): x(xx), y(yy), z(zz){}
	Location& operator=(Location const& that){
		this->x = that.x;
		this->y = that.y;
		this->z = that.z;
		return *this;
	}
#ifndef NDEBUG
    friend std::ostream& operator<<(std::ostream& lhs, Location const& rhs){
        lhs << "(" << rhs.x << ", " << rhs.y << ", " << rhs.z << ")";
        return lhs;
    }
#endif
    double x, y, z;
};

inline bool operator==(Location const& lhs, Location const& rhs){
    return
        lhs.x == rhs.x &&
        lhs.y == rhs.y &&
        lhs.z == rhs.z;
}
inline bool operator!=(Location const& lhs, Location const& rhs){
    return
        lhs.x != rhs.x ||
        lhs.y != rhs.y ||
        lhs.z != rhs.z;
}

#endif
