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

class Stage;
class Map;
class PlayerView;
typedef std::tr1::shared_ptr<Stage> pStage;
typedef std::tr1::shared_ptr<Map> pMap;
typedef std::tr1::shared_ptr<PlayerView> pPlayerView;

namespace game {

class Demo : public Object, public std::tr1::enable_shared_from_this<Demo>
{
    typedef std::tr1::shared_ptr<int> pDummy;
public:
    typedef std::tr1::shared_ptr<Demo> pointer_type;
    static pointer_type create() {
        return utils::ObjectPool<Demo>::create()->init();
    }

    Demo();
    ~Demo();

    virtual void cycle();
    void init_vs_cpu();
    void init_vs_ppl();

    view::pScene get_ui_scene();

protected:
    pointer_type init();
    void cleanup();
    void end(pMap p);
    void reinit();
    void game_start();

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

private:
    void init_(int const&);

protected:
    view::pScene scene_;
    view::pScene ui_scene_;
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
    pDummy timer_ui_, btn_reinit_, btn_pause_;
    std::string c1p_, c2p_, sconf_;

    int min_, sec_;
    int last_garbage_1p_, last_garbage_2p_; //used for temporary state comparison
    int num_of_cpu_, ai_level_;
    bool some_ui_inited_;
    bool gauge1_flag_, gauge2_flag_;

    lua_State* L_;
};

typedef Demo::pointer_type pDemo;

} //game
} //presenter
} //psc


#endif // _SHOOTING_CUBES_PRESENTER_GAME_MULTI_

