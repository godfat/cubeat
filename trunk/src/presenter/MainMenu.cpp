
#include "presenter/MainMenu.hpp"
#include "view/Scene.hpp"
#include "view/Menu.hpp"
#include "view/Sprite.hpp"
#include "data/Color.hpp"
#include "utils/to_s.hpp"

#include "EasingEquations.hpp"
#include "Accessors.hpp"
#include "App.hpp"
#include "Conf.hpp"
#include "EventDispatcher.hpp"
#include "Input.hpp"
#include "Sound.hpp"

#include <tr1/functional>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/function.hpp>
#include <utility>
#include <sstream>
#include <iostream>

using namespace psc;
using namespace presenter;
using namespace accessor;
using namespace easing;

using std::tr1::bind;
using std::tr1::ref;
using std::tr1::cref;
using std::tr1::function;
using namespace std::tr1::placeholders;

namespace BLL = boost::lambda;

pMainMenu MainMenu::init()
{
    App::i().setLoading(1);

    config = utils::map_any::construct(
        utils::fetchConfig( Conf::i().CONFIG_PRESENTER_PATH +"mainmenu.zzml" ) );

    utils::map_any title = config.M("title");
    utils::map_any text = config.M("text");
    int const m_text_size = Conf::i().SCREEN_W/30;

    mainmenu_scene_ = view::Scene::create("MainMenu");
    mainmenu_scene_->setTo2DView();

    int const width = title.I("orig_w") * (Conf::i().SCREEN_W/1280.0f); //1280 is best screen size.
    int const height= title.I("orig_h") * (Conf::i().SCREEN_W/1280.0f); //1280 is best screen size.

    view::pMenu temp =
        view::Menu::create(title.S("path"), mainmenu_scene_, width, height, true);

    menus_.insert( std::make_pair("start_menu", temp) );
    temp->moveTo( Conf::i().SCREEN_W/2, Conf::i().SCREEN_H/2 - temp->get<Size2D>().Y/3 )
         .setDepth( title.I("depth") );

    function<void(view::pSprite&)> click1 = bind(&MainMenu::push_start, this);

    temp->addSpriteText("text", text.S("text"), text.S("font"), click1, m_text_size, true)
         .getSprite("text").set<Pos2D>( vec2( 0, height ) )
         .tween<SineCirc, Alpha>(0, (unsigned int)text.I("glow_period"), -1);

    ctrl::EventDispatcher::i().subscribe_timer( bind(&MainMenu::initDecorator, this), shared_from_this(), 30 );

    return shared_from_this();
}

void MainMenu::end()
{
    App::i().launchMultiplayer();
    std::cout << "MainMenu end call finished.\n";
}

void MainMenu::initDecorator()
{
    utils::map_any deco = config.M("decorator");

    int const w = Conf::i().SCREEN_W;
    int const h = Conf::i().SCREEN_H;
    int const size = w * deco.F("size_factor");
    int const num_w = (w/size)*2;
    int const num_h = (h/size)*2 + 3;
    int const outgoing = size * 1.41f;
    int const contract = size / 4;

    vec2 start1( -outgoing, contract),  end1(w+outgoing, contract);     //line1
    vec2 start2(w-contract, -outgoing), end2(w-contract, h+outgoing);   //line2
    vec2 start3(w+outgoing, h-contract),end3( -outgoing, h-contract);   //line3
    vec2 start4( contract,  h+outgoing),end4( contract,  -outgoing);    //line4

//    int const time_w = 10000;            //1280 768 is best factor
//    int const time_h = time_w * (w/h);   //1280 768 is best factor

    int const color_num = deco.I("color_num");
    for(int i = 0; i < color_num; ++i ) {
        paths.push_back( deco.S("path") +
            std::string( utils::to_s((i%4)+1) ) );
    }

    std::vector<vec3> waypoints;
    waypoints.push_back( vec3(contract, -contract, 200) );
    waypoints.push_back( vec3(w-contract, -(contract), 100) );
    waypoints.push_back( vec3(w-contract, -(h-contract), 0) );
    waypoints.push_back( vec3(contract, -(h-contract), -100) );
    waypoints.push_back( vec3(contract, outgoing, -200) );

    int capacity = (num_w + num_h - 6)*2;
    for(int i=0; i < capacity; ++i ) {
        data::Color col = data::Color::from_id(0, color_num);
        col.offset();
        int rand_size = size * (utils::random(33)/100.0f + 1);
        view::pSprite temp = view::Sprite::create(
            paths[utils::random(paths.size())], mainmenu_scene_, rand_size, rand_size, true);

        data::WaypointParam<Linear, Pos3D> p1;
        p1.waypoints(waypoints).duration(20000).loop(-1).delay(-(float)i/capacity*20000);
        data::AnimatorParam<Linear, Rotation> p2;
        p2.end(vec3(0,0,360)).duration(utils::random(2000)+3000).loop(-1);

        temp->set<ColorDiffuse>( 0xff000000 | col.rgb() ).tween(p1).tween(p2);

        deco_cubes_.push_back( temp );
    }

    Sound::i().play("title.mp3", true);

    App::i().setLoading(100);
}

void MainMenu::menu1_1_click(view::pSprite& sprite)
{
    if( animating_ ) return;
    animating_ = true;
}

void MainMenu::menu2_1_click(view::pSprite& sprite)
{
    if( animating_ ) return;
    animating_ = true;
}

void MainMenu::fadeAllOut(int dur)
{
    BOOST_FOREACH(MenuPair& mp, menus_)
        mp.second->tweenAll<Linear, Alpha>(0, dur);

    BOOST_FOREACH(view::pSprite& sp, deco_cubes_)
        sp->tween<Linear, Alpha>(0, dur);

    ctrl::EventDispatcher::i().subscribe_timer(bind(&Sound::stopAll, &Sound::i()), dur);
    ctrl::EventDispatcher::i().subscribe_timer(bind(&App::setLoading, &App::i(), 1), dur);
}

void MainMenu::push_start()
{
    fadeAllOut(1000);
    function<void()> cb = bind(&MainMenu::end, this);
    ctrl::EventDispatcher::i().subscribe_timer(cb, shared_from_this(), 1100);
}

MainMenu& MainMenu::showMenu(std::string const& name)
{
    view::pMenu sprite = menus_[name];
    boost::function<void()> const& f = (BLL::var(animating_) = false);
    sprite->tween<OCirc, Pos2D>(vec2(320, 200), 2000, 0, f );
    sprite->tweenAll<Linear, Alpha>(255, 1000);
    sprite->set<Visible>(true);
    return *this;
}

MainMenu& MainMenu::hideMenu(std::string const& name)
{
    view::pMenu sprite = menus_[name];
    function<void()> const& endcall = bind(&view::Sprite::set<Visible>, sprite.get(), false);
    sprite->tween<ICirc, Pos2D>(vec2(-200, 200), 2000, 0, endcall);
    sprite->tweenAll<Linear, Alpha>(0, 1000);
    return *this;
}

MainMenu& MainMenu::cubeRearrange()
{
    return *this;
}

void MainMenu::cycle()
{
    mainmenu_scene_->redraw();
}
