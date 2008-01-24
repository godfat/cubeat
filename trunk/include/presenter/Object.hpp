#ifndef _SHOOTING_CUBES_PRESENTER_OBJECT_
#define _SHOOTING_CUBES_PRESENTER_OBJECT_

#include "utils/ObjectPool.hpp"

#include <boost/tr1/memory.hpp>

namespace psc { namespace presenter {

class Object
{
public:
    typedef std::tr1::shared_ptr< Object > pointer_type;
    typedef std::tr1::weak_ptr< Object > wpointer_type;
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

typedef Object::pointer_type pObject;
typedef Object::pointer_type wpObject;

} //presenter
} //psc

#endif // _SHOOTING_CUBES_PRESENTER_OBJECT_
