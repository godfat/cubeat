#ifndef _SHOOTING_CUBES_PRESENTER_STAGE_
#define _SHOOTING_CUBES_PRESENTER_STAGE_

#include "presenter/Object.hpp"
#include "utils/ObjectPool.hpp"
#include "utils/dictionary.hpp"
#include "all_fwd.hpp"

#include <string>

namespace psc {
namespace presenter {

class Stage : public Object, public std::tr1::enable_shared_from_this<Stage>
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
    Stage& hitGroup(int const&);
    Stage& playBGM();
    Stage& releaseResource(); //tell engine to release some of the cached resources.

protected:
    pointer_type init( std::string const& );

protected:
    bool need_release_;
    view::pScene scene_;
    utils::map_any conf_;
    std::vector< SceneObjList > slists_;
};

} //presenter
} //psc

#endif // _SHOOTING_CUBES_PRESENTER_STAGE_
