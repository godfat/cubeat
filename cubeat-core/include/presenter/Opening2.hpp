#ifndef _SHOOTING_CUBES_PRESENTER_OPENING2_
#define _SHOOTING_CUBES_PRESENTER_OPENING2_

#include "presenter/Object.hpp"
#include "utils/ObjectPool.hpp"
#include "utils/dictionary.hpp"
#include "all_fwd.hpp"

namespace psc {

namespace presenter {

class Opening2 : public Object, public std::tr1::enable_shared_from_this<Opening2>
{
public:
    typedef std::tr1::shared_ptr< Opening2 > pointer_type;
    static pointer_type create() {
        return utils::ObjectPool< Opening2 >::create()->init();
    }

    Opening2();
    virtual ~Opening2();

    virtual void cycle();

protected:
    void end();
    pointer_type init();

protected:
    view::pScene scene_;
    view::pSprite startup_page_;
    view::pSpriteMovie movie_;
    bool deletion_scheduled_;

    utils::map_any config_;
};

typedef std::tr1::shared_ptr< Opening2 > pOpening2;

} //presenter
} //psc

#endif // _SHOOTING_CUBES_PRESENTER_OPENING2_

