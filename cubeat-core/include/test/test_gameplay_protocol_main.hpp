#ifndef _SHOOTING_CUBES_TEST_TEST_GAMEPLAY_PROTOCOL_MAIN_
#define _SHOOTING_CUBES_TEST_TEST_GAMEPLAY_PROTOCOL_MAIN_

#include "utils/dictionary.hpp"

#include <boost/tr1/memory.hpp>
#include <string>

namespace psc {

namespace presenter {
class Stage;
typedef std::tr1::shared_ptr<Stage> pStage;
class Map;
typedef std::tr1::shared_ptr<Map> pMap;
class PlayerView;
typedef std::tr1::shared_ptr<PlayerView> pPlayerView;
}

namespace view {
class Scene;
typedef std::tr1::shared_ptr<Scene> pScene;
class Menu;
typedef std::tr1::shared_ptr<Menu> pMenu;
class SpriteText;
typedef std::tr1::shared_ptr<SpriteText> pSpriteText;
}

namespace ctrl {
class Player;
typedef std::tr1::shared_ptr<Player> pPlayer;
}

namespace test {

class TestNetGame{
    typedef std::tr1::shared_ptr<int> pDummy;
public:
    TestNetGame(int type);
    ~TestNetGame();
    void cycle();

    void ready_go(int step);
    void game_start();
    void setup_ui_by_config(std::string const& c1p, std::string const& c2p, std::string const& path);
    void update_ui();
    void update_ui_by_second();

    void on_connected(std::string const& msg);
    void on_matched(std::string const& msg);
    void on_disconnected(std::string const& msg);
    void on_received(std::string const& msg);

    void update_cursor_pos();
    void initiate_matching();

private:
    void on_mov(int, int);

    int type_;

    utils::map_any gameplay_;

    view::pScene scene_;
    presenter::pStage stage_;
    presenter::pMap map0_;
    presenter::pMap map1_;
    ctrl::pPlayer player0_;
    ctrl::pPlayer player1_;

    utils::map_any uiconf_;
    view::pMenu ui_layout_;

    view::pSpriteText ready_go_text_;
    pDummy timer_ui_;

    presenter::pPlayerView pview1_;
    presenter::pPlayerView pview2_;

    int min_, sec_;
    int last_garbage_1p_, last_garbage_2p_; //used for temporary state comparison
};

} //test
} //psc

#endif //_SHOOTING_CUBES_TEST_TEST_GAMEPLAY_PROTOCOL_MAIN_
