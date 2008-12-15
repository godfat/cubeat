
#ifndef _SHOOTING_CUBES_ACCESSOR_PROTO_
#define _SHOOTING_CUBES_ACCESSOR_PROTO_

namespace irr{
namespace scene{
    class ISceneNode;
}
}

template <class T>
struct Accessor {
    typedef T value_type;
    static void set(irr::scene::ISceneNode* node, value_type const& val ){}
    static void get(irr::scene::ISceneNode const* node, value_type& out){}
};

#endif
