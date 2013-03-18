#ifndef _SHOOTING_CUBES_PRESENTER_GAME_MULTI_
#define _SHOOTING_CUBES_PRESENTER_GAME_MULTI_

#include "presenter/Object.hpp"
#include "utils/ObjectPool.hpp"
#include "utils/dictionary.hpp"
#include "all_fwd.hpp"

#include <string>

namespace psc {

namespace ctrl {
class Input;
class Player;
typedef std::tr1::shared_ptr<Player> pPlayer;
typedef std::tr1::weak_ptr<Player> wpPlayer;
}

namespace presenter {
namespace game {

class Multi : public Object, public std::tr1::enable_shared_from_this<Multi>
{
    typedef std::tr1::shared_ptr<int> pDummy;
public:
    typedef std::tr1::shared_ptr<Multi> pointer_type;
    static pointer_type create(std::string const& c1p, std::string const& c2p,
                            std::string const& sc, int num_of_cpu, int ai_level) {
        return utils::ObjectPool<Multi>::create()->init(c1p,c2p,sc, num_of_cpu, ai_level);
    }

    Multi();
    virtual ~Multi();

    virtual void precondition(){}
    virtual void cycle();

protected:
    pointer_type init(std::string const&, std::string const&, std::string const&, int, int);
    void cleanup();
    void end(pMap p);
    void reinit();
    void game_start();

    void ready_go(int);
    void update_ui_by_second();
    void update_ui();
    void update_heatgauge(ctrl::pPlayer player, view::pSprite gauge, bool& out_flag);
    void item_creation();
    void item_destruction();
    void setup_ui_by_config( std::string const& c1p, std::string const& c2p, std::string const& path);
    void eat_item(ctrl::wpPlayer, int);

    void setup_end_button();
    void end_sequence1();
    void pause_quit();

    //temp: for pausing
    void pause(ctrl::Input const*);
    void resume(ctrl::Input const*);

protected:
    view::pScene scene_;
    presenter::pStage stage_;
    presenter::pMap map0_;
    presenter::pMap map1_;
    ctrl::pPlayer player0_;
    ctrl::pPlayer player1_;

    utils::map_any gameplay_;
    utils::map_any uiconf_;
    view::pMenu ui_layout_;

    presenter::pPlayerView pview1_;
    presenter::pPlayerView pview2_;

    view::pAnimatedSprite item_;
    view::pSprite         blocker_, win_t_, lose_t_, pause_t_, heatgauge1_, heatgauge2_;
    view::pSpriteText     end_text_, end_text2_, pause_text_, pause_text2_, pause_note_text_, ready_go_text_;
    pDummy timer_item_, timer_ui_, btn_reinit_, btn_pause_;
    std::string c1p_, c2p_, sconf_;

    int min_, sec_;
    int last_garbage_1p_, last_garbage_2p_; //used for temporary state comparison
    int num_of_cpu_, ai_level_;
    bool gauge1_flag_, gauge2_flag_;
};

} //game
} //presenter
} //psc


#endif // _SHOOTING_CUBES_PRESENTER_GAME_MULTI_
