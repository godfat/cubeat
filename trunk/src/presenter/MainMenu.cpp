
#include "presenter/MainMenu.hpp"
#include "view/Scene.hpp"
#include "view/Menu.hpp"
#include "view/Sprite.hpp"
#include "data/Color.hpp"

#include "EasingEquations.hpp"
#include "Accessors.hpp"

#include <boost/tr1/functional.hpp>
#include <utility>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <iostream>

using namespace irr;
using namespace core;
using namespace scene;
using namespace accessor;

using namespace psc;
using namespace presenter;
using namespace easing;

using std::tr1::bind;
using std::tr1::ref;
using namespace std::tr1::placeholders;

/* MainMenu has bug */

void MainMenu::init()
{
    mainmenu_scene_ = view::Scene::create();
    mainmenu_scene_->setTo2DView();

    view::pMenu temp = view::Menu::create("title", mainmenu_scene_, 150, 60);
    view::pMenu temp2= view::Menu::create("title", mainmenu_scene_, 150, 60);

    menus_.insert( std::make_pair("testmenu1", temp) );
    menus_.insert( std::make_pair("testmenu2", temp2) );

    temp->moveTo(240, 100);
    temp2->moveTo(340, 100);

    std::tr1::function<void(view::pSprite&)> click1_1 = bind(&MainMenu::menu1_1_click, this, _1);
    std::tr1::function<void(view::pSprite&)> click2_1 = bind(&MainMenu::menu2_1_click, this, _1);

    temp->addSprite("title", click1_1, 100, 40).getSprite("title").set<Pos2D>(vector2df(0, -50));
    temp2->addSprite("title", click2_1, 100, 40).getSprite("title").set<Pos2D>(vector2df(0, -50));

    temp2->set<Visible>(false);

    for(int i=0, ran=0; i < 50; ++i, ran+=50) { //640, 1120, 1760, 2240
        std::ostringstream oss;
        oss << "cubes/cube" << utils::Random::i().random(4)+1;
        data::Color col = data::Color::from_id(0, 4);

        view::pSprite temp = view::Sprite::create(oss.str(), mainmenu_scene_, 80, 80, true);
        temp->set<ColorDiffuse>( 0xff000000 | col.rgb() );
        temp->tween<Linear, Rotation>(vector3df(0, 0, 360), 3000);

        if( ran < 640 ) {
            temp->moveTo( ran + 40, 0 );
        }
        else if( ran < 1120 ) {
            temp->moveTo( 620, ran - 620 );
        }
        else if( ran < 1760 ) {
            temp->moveTo( 640 - (ran-1120), 450 );
        }
        else {
            temp->moveTo( 40, 480 - (ran-1760) );
        }
        deco_cubes_.push_back( temp );
    }
}

void MainMenu::menu1_1_click(view::pSprite& sprite)
{
    showMenu("testmenu2").hideMenu("testmenu1");
}

void MainMenu::menu2_1_click(view::pSprite& sprite)
{
    showMenu("testmenu1").hideMenu("testmenu2");
}

MainMenu& MainMenu::showMenu(std::string const& name)
{
    menus_[name]->set<Visible>(true);
    return *this;
}

MainMenu& MainMenu::hideMenu(std::string const& name)
{
    menus_[name]->set<Visible>(false);
    return *this;
}

MainMenu& MainMenu::cubeRearrange()
{
    return *this;
}

void MainMenu::cycle()
{
    mainmenu_scene_->activate().redraw();
    mainmenu_scene_->deactivate();
}
