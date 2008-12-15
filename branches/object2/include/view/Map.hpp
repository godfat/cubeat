
#ifndef _SHOOTING_CUBES_MAP_VIEW_
#define _SHOOTING_CUBES_MAP_VIEW_

#include "view/Object.hpp"

#include <map>
#include <vector>

namespace irr{
namespace scene{
class ISceneNode;
}
}

namespace view {

class Scene;
class Cube;

class Map : public Object
{
public:
    Map(int index):index_(index){}

    virtual Map* clone() const;
    virtual Map& init(Scene const*);
    virtual Map& addCube(Cube*);

protected:
    virtual void ownerHitCallback(int x, int y);
    virtual void enemyHitCallback(int x, int y);

protected:
    int index_;

    std::map< irr::scene::ISceneNode*, Cube* > node2view_;
    std::vector< Cube* > cubes_;
    Scene const* parent_;
};

};

#endif

