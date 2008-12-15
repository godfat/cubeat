
#ifndef _SHOOTING_CUBES_CUBE_VIEW_
#define _SHOOTING_CUBES_CUBE_VIEW_

#include "view/SceneObject.hpp"

#include "utils/ObjectPool.hpp"
#include <boost/tr1/memory.hpp>

namespace view {

class Map;

class Cube : public SceneObject
{
public:
    typedef std::tr1::shared_ptr<Cube> pointer_type;
    static pointer_type create(Map* map) {
        pointer_type p = psc::ObjectPool<Cube>::create();
        p->init(map);
        return p;
    }

    Cube():SceneObject("test_cube"){}
    virtual void init(Map*);

    virtual Cube* clone() const;
    virtual void ownerHit();
    virtual void enemyHit();

    virtual ~Cube(){}

private:

};

typedef Cube::pointer_type pCube;

} //view

#endif
