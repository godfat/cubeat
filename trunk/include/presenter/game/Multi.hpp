#ifndef _SHOOTING_CUBES_PRESENTER_GAME_MULTI_
#define _SHOOTING_CUBES_PRESENTER_GAME_MULTI_

#include "presenter/Object.hpp"
#include "utils/ObjectPool.hpp"
#include "utils/dictionary.hpp"
#include "all_fwd.hpp"

#include <string>

namespace psc {
namespace presenter {
namespace game {

class Multi : public Object, public std::tr1::enable_shared_from_this<Multi>
{
public:
    typedef std::tr1::shared_ptr<Multi> pointer_type;
    static pointer_type create() {
        return utils::ObjectPool<Multi>::create()->init();
    }

    Multi();
    ~Multi();

    virtual void cycle();

protected:
    pointer_type init();

protected:


};

} //game
} //presenter
} //psc


#endif // _SHOOTING_CUBES_PRESENTER_GAME_MULTI_
