
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
//     DIFFUSE  <-> {R,G,B}    (but R /G /B  are not conflicting among themselves)
//     EMISSIVE <-> {RE,GE,BE} (but RE/GE/BE are not conflicting among themselves)
//     SCALE    <-> SIZE2D

struct AT{
    enum ATEnum {POSITION = irr::scene::ESNAT_UNKNOWN+1,
                 ROTATION, SCALE, DIFFUSE, EMISSIVE, R, G, B, A, RE, GE, BE, AE,
                 FRAME, VISIBLE, ID, SIZE2D, TEXTURE, UNKNOWN};
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
