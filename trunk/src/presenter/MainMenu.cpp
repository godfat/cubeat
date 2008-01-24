
#include "presenter/MainMenu.hpp"
#include "view/Scene.hpp"
#include "view/Menu.hpp"
#include "view/Sprite.hpp"

#include "EasingEquations.hpp"
#include "Accessors.hpp"

#include <boost/tr1/functional.hpp>
#include <utility>

using namespace irr;
using namespace scene;
using namespace accessor;

using namespace psc;
using namespace presenter;
using namespace easing;

using std::tr1::bind;
using std::tr1::ref;

void MainMenu::init()
{
    mainmenu_scene_ = view::Scene::create();
    mainmenu_scene_->setTo2DView(640,480);

    view::pMenu temp = view::Menu::create("title", mainmenu_scene_);
    view::pMenu temp2= view::Menu::create("title", mainmenu_scene_);

    temp->moveTo(240, 100);
    temp2->moveTo(240, 150);
    /* TODO: add temp Menus content here. */
    /* callback sources are from MainMenu class */

    menus_.insert( std::make_pair("testmenu1", temp) );
    menus_.insert( std::make_pair("testmenu2", temp2) );
}

void MainMenu::menu1_1_click(view::Sprite& sprite)
{
}

void MainMenu::menu2_1_click(view::Sprite& sprite)
{
}

MainMenu& MainMenu::showMenu(std::string const& name)
{
    return *this;
}

MainMenu& MainMenu::hideMenu(std::string const& name)
{
    return *this;
}

MainMenu& MainMenu::cubeRearrange()
{
    return *this;
}


void MainMenu::cycle()
{
    //temp code, currently I can't think how to make it better.
    mainmenu_scene_->activate();
    IrrDevice::i().d()->getSceneManager()->drawAll();
    mainmenu_scene_->deactivate();
}
