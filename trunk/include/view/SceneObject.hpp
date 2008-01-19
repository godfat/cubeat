
#ifndef _SHOOTING_CUBES_SCENE_OBJECT_VIEW_
#define _SHOOTING_CUBES_SCENE_OBJECT_VIEW_

#include "view/Object.hpp"
#include "utils/ObjectPool.hpp"
#include <boost/tr1/memory.hpp>

#include <string>

namespace psc { namespace view {

class SceneObject : public Object, public std::tr1::enable_shared_from_this<SceneObject>
{
public:
    typedef std::tr1::shared_ptr< SceneObject > pointer_type;
    static pointer_type create(std::string const& name, pObject const& parent) {
        pointer_type p = psc::ObjectPool< SceneObject >::create(name);
        p->init(parent);
        return p;
    }

    SceneObject(std::string const& name):Object(name){}
    
    virtual SceneObject* clone() const;

    virtual ~SceneObject(){}

protected:
    void init(pObject const&);

protected:

};

typedef SceneObject::pointer_type pSceneObject;

} //view
} //psc

#endif
