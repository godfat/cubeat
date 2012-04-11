#ifndef _SHOOTING_CUBES_PRESENTER_GAME_PUZZLE_HPP_
#define _SHOOTING_CUBES_PRESENTER_GAME_PUZZLE_HPP_

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

class Puzzle : public Object, public std::tr1::enable_shared_from_this<Puzzle>
{
    typedef std::tr1::shared_ptr<int> pDummy;
public:
    typedef std::tr1::shared_ptr<Puzzle> pointer_type;
    static pointer_type create(std::string const& c1p, std::string const& sc, int puzzle_level) {
        return utils::ObjectPool<Puzzle>::create()->init(c1p,sc,puzzle_level);
    }

    Puzzle();
    virtual ~Puzzle();

    virtual void cycle();

protected:
    pointer_type init(std::string const&, std::string const&, int puzzle_level);
    void cleanup();
    void end(pMap p);
    void reinit();
    void update_ui_by_second();
    void update_ui();
    void update_heatgauge(ctrl::pPlayer player, view::pSprite gauge, bool& out_flag);
    void setup_ui_by_config( std::string const& c1p, std::string const& c2p, std::string const& path );

    void setup_end_button();
    void end_sequence1();
    void pause_quit();

    //temp: for pausing
    void pause();
    void resume();

    void puzzle_started();

protected:
    view::pScene scene_;
    presenter::pStage stage_;
    presenter::pMap map0_;
    presenter::pMap map1_;
    ctrl::pPlayer player0_;

    utils::map_any uiconf_;
    view::pMenu ui_layout_;

    presenter::pPlayerView pview1_;

    view::pSprite     blocker_, win_t_, lose_t_, pause_t_, heatgauge1_;
    view::pSpriteText end_text_, end_text2_, pause_text_, pause_text2_, desc_text_;
    pDummy timer_ui_, btn_reinit_, btn_pause_;
    std::string c1p_, sconf_;
    int puzzle_level_;
    bool win_, puzzle_started_, end_;

    int min_, sec_;
    int last_garbage_1p_, last_garbage_2p_; //used for temporary state comparison
    bool gauge1_flag_;
};

} //game
} //presenter
} //psc

#endif // _SHOOTING_CUBES_PRESENTER_GAME_PUZZLE_HPP_
