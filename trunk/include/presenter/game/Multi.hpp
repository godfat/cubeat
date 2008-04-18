#ifndef _SHOOTING_CUBES_PRESENTER_GAME_MULTI_
#define _SHOOTING_CUBES_PRESENTER_GAME_MULTI_

#include "presenter/Object.hpp"
#include "utils/ObjectPool.hpp"
#include "utils/dictionary.hpp"
#include "all_fwd.hpp"

#include <string>

namespace psc {

namespace ctrl {
class Player;
typedef std::tr1::shared_ptr<Player> pPlayer;
}

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
    void end();
    void update_ui_by_second();
    void update_ui();
    void item_creation();
    void item_destruction();
    void setup_ui_by_config( std::string const& path );
    void eat_item(ctrl::pPlayer, int);

protected:
    view::pScene scene_;
    presenter::pStage stage_;
    presenter::pMap map0_;
    presenter::pMap map1_;
    ctrl::pPlayer player0_;
    ctrl::pPlayer player1_;

    utils::map_any uiconf_;
    view::pMenu ui_layout_;

    presenter::pPlayerView pview1_;
    presenter::pPlayerView pview2_;

    view::pAnimatedSprite item_;

    int min_, sec_;
    int last_garbage_1p_, last_garbage_2p_; //used for temporary state comparison
};

} //game
} //presenter
} //psc


#endif // _SHOOTING_CUBES_PRESENTER_GAME_MULTI_
