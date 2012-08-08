#ifndef _SHOOTING_CUBES_PRESENTER_STAGE_
#define _SHOOTING_CUBES_PRESENTER_STAGE_

#include "utils/ObjectPool.hpp"
#include "utils/dictionary.hpp"

#include <string>
#include <boost/tr1/memory.hpp>

namespace psc {

namespace view {
class Scene;
class AnimatedSceneObject;
typedef std::tr1::shared_ptr<AnimatedSceneObject> pAnimatedSceneObject;
typedef std::tr1::shared_ptr<Scene> pScene;
}

namespace audio {
class SoundObject;
typedef std::tr1::weak_ptr<SoundObject> wpSoundObject;
}

namespace presenter {

class Stage : public std::tr1::enable_shared_from_this<Stage>
{
    typedef std::vector< view::pAnimatedSceneObject > SceneObjList;

public:
    typedef std::tr1::shared_ptr<Stage> pointer_type;
    static pointer_type create( std::string const& path ) {
        return utils::ObjectPool< Stage >::create()->init(path);
    }

    Stage();
    ~Stage();

    virtual void cycle();
    Stage& loadBGM();
    Stage& hitGroup(int const&);
    Stage& playBGM(time_t const& t = 200);
    Stage& playFastBGM(time_t const& t = 200);
    Stage& playNormalBGM_without_leading(time_t const& lt, time_t const& ft = 200);
    Stage& releaseResource(); //tell engine to release some of the cached resources.

protected:
    pointer_type init( std::string const& );

protected:
    bool need_release_;
    view::pScene scene_;
    utils::map_any conf_;
    std::vector< SceneObjList > slists_;
    std::string music_path_a_, music_path_b_, fmusic_path_a_, fmusic_path_b_;

    struct AnimParam {
        int s; //start frame
        int e; //end frame
        int dur; //duration of anim
        int loop; //loop times (-1 means forever)
    };

    typedef std::map<std::string, AnimParam> AnimSets;
    typedef std::vector<AnimSets>            AnimList;
    typedef std::vector<AnimList>            AnimGroup;

    AnimGroup anim_group_;
};

} //presenter
} //psc

#endif // _SHOOTING_CUBES_PRESENTER_STAGE_
