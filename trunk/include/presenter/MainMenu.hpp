#ifndef _SHOOTING_CUBES_PRESENTER_MAINMENU_
#define _SHOOTING_CUBES_PRESENTER_MAINMENU_

#include "presenter/Object.hpp"

#include "utils/ObjectPool.hpp"
#include "all_fwd.hpp"
#include <map>
#include <string>
#include <vector>

namespace psc {

namespace presenter {

class MainMenu : public Object, public std::tr1::enable_shared_from_this<MainMenu>
{
public:
    typedef std::tr1::shared_ptr< MainMenu > pointer_type;
    static pointer_type create() {
        pointer_type p = utils::ObjectPool< MainMenu >::create();
        p->init();
        return p;
    }

    MainMenu(): animating_(false){}

    virtual void cycle();

protected:
    void init();
    MainMenu& showMenu(std::string const&);
    MainMenu& hideMenu(std::string const&);
    MainMenu& cubeRearrange();

    void menu1_1_click(view::pSprite&);
    void menu2_1_click(view::pSprite&);

protected:
    view::pScene mainmenu_scene_;
    std::map<std::string, view::pMenu> menus_;
    std::vector<view::pSprite> deco_cubes_;

    bool animating_;
};

} //presenter
} //psc

#endif // _SHOOTING_CUBES_PRESENTER_MAINMENU_
