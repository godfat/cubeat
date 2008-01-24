#ifndef _SHOOTING_CUBES_PRESENTER_MAINMENU_
#define _SHOOTING_CUBES_PRESENTER_MAINMENU_

#include "presenter/Object.hpp"

#include "utils/ObjectPool.hpp"
#include <boost/tr1/memory.hpp>
#include <map>
#include <string>
#include <vector>

namespace psc {

namespace view {
class Scene;
typedef std::tr1::shared_ptr<Scene> pScene;
class Sprite;
typedef std::tr1::shared_ptr<Sprite> pSprite;
class Menu;
typedef std::tr1::shared_ptr<Menu> pMenu;
}

namespace presenter {

class MainMenu : public Object, public std::tr1::enable_shared_from_this<MainMenu>
{
public:
    typedef std::tr1::shared_ptr< MainMenu > pointer_type;
    typedef std::tr1::weak_ptr< MainMenu > wpointer_type;
    static pointer_type create() {
        pointer_type p = psc::ObjectPool< MainMenu >::create();
        p->init();
        return p;
    }

    virtual void cycle();

protected:
    void init();
    MainMenu& showMenu(std::string const&);
    MainMenu& hideMenu(std::string const&);
    MainMenu& cubeRearrange();

    void menu1_1_click(view::Sprite&);
    void menu2_1_click(view::Sprite&);

protected:
    view::pScene mainmenu_scene_;
    std::map<std::string, view::pMenu> menus_;
    std::vector<view::pSprite> deco_cubes_;

};

typedef MainMenu::pointer_type pMainMenu;
typedef MainMenu::wpointer_type wpMainMenu;

} //presenter
} //psc

#endif // _SHOOTING_CUBES_PRESENTER_MAINMENU_
