
#ifndef _SHOOTING_CUBES_CUBE_VIEW_
#define _SHOOTING_CUBES_CUBE_VIEW_

#include "view/SceneObject.hpp"

#include "utils/ObjectPool.hpp"
#include "all_fwd.hpp"

namespace psc { namespace view {

class Cube : public SceneObject
{
    typedef std::tr1::function<void()> HitCallback;
public:
    typedef std::tr1::shared_ptr<Cube> pointer_type;
    static pointer_type create(pMap& map) {
        pointer_type p = utils::ObjectPool<Cube>::create();
        p->init(map);
        return p;
    }

    Cube():SceneObject("test_cube"), owner_hit_(0), enemy_hit_(0){}

    virtual Cube* clone() const;
    virtual void ownerHit();
    virtual void enemyHit();
    virtual Cube& setOwnerHit( HitCallback const& );
    virtual Cube& setEnemyHit( HitCallback const& );

    virtual ~Cube(){}

protected:
    void init(pMap&);

protected:
    HitCallback owner_hit_;
    HitCallback enemy_hit_;
};

} //view
} //psc

#endif
