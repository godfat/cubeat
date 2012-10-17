#ifndef _SHOOTING_CUBES_PRESENTER_GAME_DEMO_
#define _SHOOTING_CUBES_PRESENTER_GAME_DEMO_

#include "presenter/Object.hpp"
#include "utils/ObjectPool.hpp"
#include "utils/dictionary.hpp"

#include <string>

struct lua_State;

namespace psc {

namespace ctrl {
class Input;
class Player;
typedef std::tr1::shared_ptr<Player> pPlayer;
}

namespace view {
class Scene;
class Sprite;
class SpriteText;
class AnimatedSprite;
class Menu;
typedef std::tr1::shared_ptr<Scene> pScene;
typedef std::tr1::shared_ptr<Sprite> pSprite;
typedef std::tr1::shared_ptr<SpriteText> pSpriteText;
typedef std::tr1::shared_ptr<AnimatedSprite> pAnimatedSprite;
typedef std::tr1::shared_ptr<Menu> pMenu;
}

namespace presenter {

class Stage2;
class Map;
class PlayerView;
typedef std::tr1::shared_ptr<Stage2> pStage2;
typedef std::tr1::shared_ptr<Map> pMap;
typedef std::tr1::shared_ptr<PlayerView> pPlayerView;

namespace game {

class Demo : public Object, public std::tr1::enable_shared_from_this<Demo>
{
    typedef std::tr1::shared_ptr<int> pDummy;

    // added prelimenary enums for game_mode_, but actually I really need to be able to distinguish "how many player and how many AI"
    enum { GM_PUZZLE = -1, GM_PVP = 0, GM_PVC, GM_CVC, GM_LOG, GM_TUT1 };

public:
    typedef std::tr1::shared_ptr<Demo> pointer_type;
    static pointer_type create() {
        return utils::ObjectPool<Demo>::create()->init();
    }

    Demo();
    ~Demo();

    virtual void cycle();
    void init_vs_cpu(std::string const&, std::string const&, std::string const&, int const&);
    void init_vs_ppl(std::string const&, std::string const&, std::string const&);
    void init_cpudemo(std::string const&, std::string const&, std::string const&);
    void init_ai_logging(std::string const&, std::string const&, std::string const&);
    void init_puzzle(std::string const&, std::string const&);
    void init_tutorial(std::string const&, std::string const&, std::string const&);
    void quit();

    void leaving_effect();
    void starting_effect(bool const&);

    view::pScene get_ui_scene();

protected:
    pointer_type init();
    void cleanup();
    void end(pMap p);
    void reinit();
    void game_start();
    void game_stop();

    void ready_go(int);
    void update_ui_by_second();
    void update_ui();
    void update_heatgauge(ctrl::pPlayer player, view::pSprite gauge, bool& out_flag);
    void setup_ui();

    void setup_end_button();
    void end_sequence1();
    void pause_quit();

    //temp: for pausing
    void pause(ctrl::Input const*);
    void resume(ctrl::Input const*);

    //lua UI integration
    void ask_for_tutorial();
    void hide_upper_layer_ui();

    bool ai_logging(pMap);
    void run_next_log();

private:
    void init_(int const&, std::string const&, std::string const&, std::string const&, bool const& inplace = false);
    void init_for_puzzle_(std::string const&, std::string const&, int const&, bool const& inplace = false);

    //temp: hack for puzzle integrated demo
    void puzzle_started();

    //temp: hack for music test
    void music_state(bool);

    //temp: helper for push_garbage periodically when in tutorial or challenge
    void garbage_timer(pMap, std::time_t, int amount);

protected:
    view::pScene scene_;
    view::pScene ui_scene_;
    presenter::pStage2 stage_;
    presenter::pMap map0_;
    presenter::pMap map1_;
    ctrl::pPlayer player0_;
    ctrl::pPlayer player1_;

    utils::map_any gameplay_;
    utils::map_any uiconf_;
    utils::map_any passive_conf0_, passive_conf1_;
    utils::map_any ai_logging_conf_;
    view::pMenu ui_layout_;

    presenter::pPlayerView pview1_;
    presenter::pPlayerView pview2_;

    view::pAnimatedSprite item_;
    view::pSprite         blocker_, win_t_, lose_t_, pause_t_, heatgauge1_, heatgauge2_;
    view::pSpriteText     end_text_, end_text2_, pause_text_, pause_text2_, pause_note_text_, ready_go_text_, desc_text_;
    pDummy timer_ui_, btn_reinit_, btn_pause_;
    std::string c1p_, c2p_, sconf_;

    int min_, sec_;
    int last_garbage_1p_, last_garbage_2p_; //used for temporary state comparison
    int game_mode_, ai_level_, ai_logging_times_, ai_logging_rounds_;
    bool some_ui_inited_;
    bool gauge1_flag_, gauge2_flag_;

    // temp: puzzle demo integrated hack
    int puzzle_level_;
    bool win_, puzzle_started_, end_;

    // temp: hack, just for test
    bool music_state_, music_state_old_;
    pDummy timer_music_state_;
    pDummy tutorial_map1_purge_timer_;

    lua_State* L_;
};

typedef Demo::pointer_type pDemo;

} //game
} //presenter
} //psc


#endif // _SHOOTING_CUBES_PRESENTER_GAME_MULTI_

