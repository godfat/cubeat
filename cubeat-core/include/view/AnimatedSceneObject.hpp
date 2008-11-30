
#ifndef _SHOOTING_CUBES_ANIMATED_SCENE_OBJECT_VIEW_
#define _SHOOTING_CUBES_ANIMATED_SCENE_OBJECT_VIEW_

#include "view/SceneObject.hpp"
#include "Accessors.hpp"
#include "utils/ObjectPool.hpp"
#include "all_fwd.hpp"

#include <map>

namespace psc { namespace view {

//struct KeyFrameData {
//    KeyFrameData(float const& s, float const& e):start(s), end(e){}
//    float start;
//    float end;
//};

class AnimatedSceneObject : public SceneObject
{
public:
    typedef std::tr1::shared_ptr< AnimatedSceneObject > pointer_type;
    static pointer_type create(std::string const& path, pObject const& parent) {
        return utils::ObjectPool< AnimatedSceneObject >::create(path)->init(parent);
    }

    AnimatedSceneObject(std::string const& path):SceneObject(path){}

    virtual AnimatedSceneObject* clone() const;
    virtual bool isPlaying() const;

protected:
    pointer_type init(pObject const&);
};

}  //view
}  //psc

#endif
