#ifndef _SHOOTING_CUBES_PRESENTER_STAGE2_
#define _SHOOTING_CUBES_PRESENTER_STAGE2_

#include "utils/ObjectPool.hpp"
#include "utils/dictionary.hpp"

#include <string>
#include <boost/tr1/memory.hpp>

struct lua_State;

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

class Stage2 : public std::tr1::enable_shared_from_this<Stage2>
{
    typedef std::vector< view::pAnimatedSceneObject > SceneObjList;

public:
    typedef std::tr1::shared_ptr<Stage2> pointer_type;
    static pointer_type create( std::string const& path ) {
        return utils::ObjectPool< Stage2 >::create()->init(path);
    }

    Stage2();
    ~Stage2();

    virtual void cycle();
    Stage2& loadBGM();
    Stage2& playBGM(time_t const& t = 200);
    Stage2& playFastBGM(time_t const& t = 200);
    Stage2& releaseResource(); //tell engine to release some of the cached resources.

protected:
    pointer_type init( std::string const& );

protected:
    bool need_release_;
    view::pScene scene_;
    utils::map_any conf_;
    std::string music_path_a_, music_path_b_, fmusic_path_a_, fmusic_path_b_;

    lua_State* L_;
};

typedef Stage2::pointer_type pStage2;

} //presenter
} //psc

#endif // _SHOOTING_CUBES_PRESENTER_STAGE2_
