
#ifndef _SHOOTING_CUBES_SCENE_OBJECT_VIEW_
#define _SHOOTING_CUBES_SCENE_OBJECT_VIEW_

#include "view/Object.hpp"

#include <string>

namespace view {

class SceneObject : public Object
{
public:
    SceneObject(std::string path):path_(path){}
    virtual void init(Object const*);

    virtual SceneObject* clone() const;

    virtual ~SceneObject(){}

protected:
    std::string path_;
};

} //view

#endif
