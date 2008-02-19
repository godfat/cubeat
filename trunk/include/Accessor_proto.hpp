
#ifndef _SHOOTING_CUBES_ACCESSOR_PROTO_
#define _SHOOTING_CUBES_ACCESSOR_PROTO_

namespace irr{

namespace core{
template<class T>
class vector3d;
typedef vector3d<float> vector3df;
}

namespace scene{
class ISceneNode;
}
}

template <class T>
struct Accessor {
    typedef T value_type;
    static void set(irr::scene::ISceneNode* node, value_type const& val ){}
    static void get(irr::scene::ISceneNode const* node, value_type& out){}
    static inline void scaleWrap(irr::scene::ISceneNode const* node, irr::core::vector3df& out);
    static inline void scaleUnwrap(irr::scene::ISceneNode* node, irr::core::vector3df& out);
};

#endif
