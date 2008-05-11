#ifndef _SHOOTING_CUBES_PRESENTER_MAINMENU_
#define _SHOOTING_CUBES_PRESENTER_MAINMENU_

#include "presenter/Object.hpp"

#include "utils/ObjectPool.hpp"
#include "utils/dictionary.hpp"
#include "data/BasicViewTypes.hpp"
#include "all_fwd.hpp"
#include <map>
#include <string>
#include <vector>
#include <tr1/functional>

namespace psc {

namespace presenter {

class MainMenu : public Object, public std::tr1::enable_shared_from_this<MainMenu>
{
    typedef std::tr1::shared_ptr<int> pDummy;
public:
    typedef std::tr1::shared_ptr< MainMenu > pointer_type;
    static pointer_type create() {
        return utils::ObjectPool< MainMenu >::create()->init();
    }

    MainMenu(): animating_(false){}
    ~MainMenu();

    virtual void cycle();

protected:
    pointer_type init();

    MainMenu& showMenu(std::string const&);
    MainMenu& hideMenu(std::string const&);
    MainMenu& cubeRearrange();

    void setupMenus();
    void menu1_1_click(view::pSprite&);
    void menu2_1_click(view::pSprite&);

    void player1_select(int);
    void player2_select(int);
    void player1_checked();
    void player2_checked();
    void stage_choosing();
    void stage_select(view::pSprite&, std::string);
    void mode_select(view::pSprite&, int);

    void fadeAllOut(int);
    void push_start();
    void end();

    void initDecorator();

protected:
    view::pScene mainmenu_scene_;

    typedef std::pair<std::string const, view::pMenu> MenuPair;
    std::map<std::string, view::pMenu> menus_;
    std::vector<view::pSprite> deco_cubes_;

    bool animating_;

    utils::map_any config;
    std::vector<std::string> paths;

    enum { SINGLE, MULTI, PUZZLE };
    int player1focus_, player2focus_, player1num_, player2num_, stagenum_;
    bool two_players_;
    int game_mode_;
    std::vector<pPlayerView> pvlist_;
    view::pSpriteText player1text_, player2text_;
    std::string conf1p_, conf2p_, stage_;
    pDummy btn_start_, btn_choose_player1_, btn_choose_player2_;
};

} //presenter
} //psc

#endif // _SHOOTING_CUBES_PRESENTER_MAINMENU_
