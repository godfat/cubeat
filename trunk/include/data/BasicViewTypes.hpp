#ifndef _SHOOTING_CUBES_DATA_BASIC_VIEW_TYPES_HPP_
#define _SHOOTING_CUBES_DATA_BASIC_VIEW_TYPES_HPP_

/*  basic core type system, currently have vector types,
    type defined dependently from Irrlicht Engine core types.

    TODO:
    * implement a less dependent core type system,
      so we can have it work with most of the 3D engines
      like Ogre, and maybe physics engines too. */

#include "vector2d.h"
#include "vector3d.h"

namespace psc {

typedef irr::core::vector2df vec2;
typedef irr::core::vector3df vec3;
float const PI = irr::core::PI;

} //psc

#endif // _SHOOTING_CUBES_DATA_BASIC_VIEW_TYPES_HPP_
