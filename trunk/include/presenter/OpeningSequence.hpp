#ifndef _SHOOTING_CUBES_PRESENTER_OPENING_SEQUENCE_
#define _SHOOTING_CUBES_PRESENTER_OPENING_SEQUENCE_

#include "presenter/Object.hpp"
#include "utils/ObjectPool.hpp"
#include "all_fwd.hpp"

namespace psc {

namespace presenter {

class OpeningSequence : public Object, public std::tr1::enable_shared_from_this<OpeningSequence>
{
public:
    typedef std::tr1::shared_ptr< OpeningSequence > pointer_type;
    static pointer_type create() {
        return utils::ObjectPool< OpeningSequence >::create()->init();
    }

    OpeningSequence();
    ~OpeningSequence();

    virtual void cycle();

protected:
    void end();
    pointer_type init();

protected:
    view::pScene scene_;
    view::pSpriteMovie movie_;
    bool deletion_scheduled_;
};

} //presenter
} //psc

#endif // _SHOOTING_CUBES_PRESENTER_OPENING_SEQUENCE_
