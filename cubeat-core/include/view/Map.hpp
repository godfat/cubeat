
#ifndef _SHOOTING_CUBES_MAP_VIEW_
#define _SHOOTING_CUBES_MAP_VIEW_

#include "view/Object.hpp"
#include "utils/ObjectPool.hpp"
#include "all_fwd.hpp"

#include <map>
#include <vector>

/* FIXME:
   No, you can't put picking and node2view conversion here.
   Too many restrictions. Better move to Player class or Scene class
   when I got one. */

namespace irr{
namespace scene{
class ISceneNode;
}
}

namespace psc { namespace view {

class Map : public Object
{
public:
    typedef std::tr1::shared_ptr< Map > pointer_type;
    static pointer_type create(int const& index, pScene const& parent) {
        return utils::ObjectPool< Map >::create(index)->init(parent);
    }

    Map(int index):index_(index){}

    virtual Map& addCube(pCube);
    virtual ~Map();

protected:
    pointer_type init(pScene const&);
    void ownerHitCallback(int x, int y);
    void enemyHitCallback(int x, int y);

protected:
    int index_;

    std::map< irr::scene::ISceneNode*, wpCube > node2view_;
    std::vector< wpCube > cubes_;
    wpScene parent_;
};

} //view
} //psc

#endif

