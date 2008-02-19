
#ifndef _SHOOTING_CUBES_ANIMATED_SCENE_OBJECT_VIEW_
#define _SHOOTING_CUBES_ANIMATED_SCENE_OBJECT_VIEW_

#include "view/SceneObject.hpp"

#include "utils/ObjectPool.hpp"
#include "all_fwd.hpp"

namespace psc { namespace view {

class AnimatedSceneObject : public SceneObject
{
public:
    typedef std::tr1::shared_ptr< AnimatedSceneObject > pointer_type;
    static pointer_type create(std::string const& path, pObject const& parent) {
        pointer_type p = utils::ObjectPool< AnimatedSceneObject >::create(path);
        p->init(parent);
        return p;
    }

    AnimatedSceneObject(std::string const& path):SceneObject(path){}

    virtual AnimatedSceneObject* clone() const;

protected:
    void init(pObject const&);

};

}  //view
}  //psc

#endif
