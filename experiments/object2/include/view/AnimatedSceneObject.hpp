
#ifndef _SHOOTING_CUBES_ANIMATED_SCENE_OBJECT_VIEW_
#define _SHOOTING_CUBES_ANIMATED_SCENE_OBJECT_VIEW_

#include "view/SceneObject.hpp"

namespace view {

class AnimatedSceneObject : public SceneObject
{
public:
    AnimatedSceneObject(std::string path):SceneObject(path){}
    virtual void init(Object const*);

    virtual AnimatedSceneObject* clone() const;

protected:

};

}  //view

#endif
