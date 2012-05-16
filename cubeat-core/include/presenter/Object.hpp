#ifndef _SHOOTING_CUBES_PRESENTER_OBJECT_
#define _SHOOTING_CUBES_PRESENTER_OBJECT_

#include "utils/ObjectPool.hpp"
#include "all_fwd.hpp"

#include <boost/tr1/functional.hpp>

namespace psc { namespace presenter {

class Object
{
public:
    typedef std::tr1::shared_ptr< Object > pointer_type;
    static pointer_type create() {
        pointer_type p = utils::ObjectPool< Object >::create();
        p->init();
        return p;
    }

    Object();

    virtual void cycle();
    virtual ~Object();

protected:
    void init();

protected:
    std::tr1::function<void(int)> loading_complete_;
};

} //presenter
} //psc

#endif // _SHOOTING_CUBES_PRESENTER_OBJECT_
