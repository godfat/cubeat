
#ifndef _SHOOTING_CUBES_ACCESSOR_PROTO_
#define _SHOOTING_CUBES_ACCESSOR_PROTO_

#include "ESceneNodeAnimatorTypes.h"

namespace irr{
namespace scene{
class ISceneNode;
}
}

namespace psc{
namespace accessor{

// potential conflicting enum:
//     POSITION <-> {X,Y,Z}    (but X /Y /Z  are not conflicting among themselves)
//     DIFFUSE  <-> {R,G,B}    (but R /G /B  are not conflicting among themselves)
//     EMISSIVE <-> {RE,GE,BE} (but RE/GE/BE are not conflicting among themselves)
//     SCALE    <-> SIZE2D

struct AT{
    enum ATEnum {POSITION = irr::scene::ESNAT_UNKNOWN+1, X, Y, Z,
                 ROTATION, SCALE, DIFFUSE, EMISSIVE, R, G, B, A, RE, GE, BE, AE,
                 FRAME, VISIBLE, ID, SIZE2D, TEXTURE, UNKNOWN};

    static bool isMatrixTransformationValue(ATEnum const& eType) {
        switch (eType) {
            case AT::POSITION:
            case AT::X:
            case AT::Y:
            case AT::Z:
            case AT::ROTATION:
            case AT::SCALE:
                return true;
            default:
                return false;
        }
    }
};

template <class T, int U = AT::UNKNOWN>
struct Accessor {
    typedef T value_type;
    enum { TYPE = U };
    static void set(irr::scene::ISceneNode* node, value_type const& val ){}
    static void get(irr::scene::ISceneNode const* node, value_type& out){}
};
} //accessor
} //psc
#endif
