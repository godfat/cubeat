
#ifndef _SHOOTING_CUBES_MAP_VIEW_
#define _SHOOTING_CUBES_MAP_VIEW_

#include "view/Object.hpp"
#include "utils/ObjectPool.hpp"
#include <boost/tr1/memory.hpp>

#include <map>
#include <vector>

namespace irr{
namespace scene{
class ISceneNode;
}
}

namespace psc { namespace view {

class Scene;
class Cube;

class Map : public Object, public std::tr1::enable_shared_from_this<Map>
{
    typedef std::tr1::weak_ptr<Scene> wpScene;
    typedef std::tr1::shared_ptr<Scene> pScene;
    typedef std::tr1::shared_ptr<Cube> pCube;
public:
    typedef std::tr1::shared_ptr< Map > pointer_type;
    static pointer_type create(int const& index, pScene const& parent) {
        pointer_type p = psc::ObjectPool< Map >::create(index);
        p->init(parent);
        return p;
    }

    Map(int index):index_(index){}

    virtual Map* clone() const;
    virtual Map& addCube(pCube);

protected:
    virtual void init(pScene const&);
    virtual void ownerHitCallback(int x, int y);
    virtual void enemyHitCallback(int x, int y);

protected:
    int index_;

    std::map< irr::scene::ISceneNode*, pCube > node2view_;
    std::vector< pCube > cubes_;
    wpScene parent_;
};

typedef Map::pointer_type pMap;

} //view
} //psc

#endif

