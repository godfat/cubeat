#ifndef _SHOOTING_CUBES_PRESENTER_OBJECT_
#define _SHOOTING_CUBES_PRESENTER_OBJECT_

#include "utils/ObjectPool.hpp"
#include "all_fwd.hpp"

namespace psc { namespace presenter {

class Object
{
public:
    typedef std::tr1::shared_ptr< Object > pointer_type;
    static pointer_type create() {
        pointer_type p = psc::ObjectPool< Object >::create();
        p->init();
        return p;
    }

    virtual void cycle();

protected:
    void init();

protected:


};

} //presenter
} //psc

#endif // _SHOOTING_CUBES_PRESENTER_OBJECT_
