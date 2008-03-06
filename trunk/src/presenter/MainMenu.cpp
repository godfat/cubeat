
#include "presenter/MainMenu.hpp"
#include "view/Scene.hpp"
#include "view/Menu.hpp"
#include "view/Sprite.hpp"
#include "data/Color.hpp"

#include "EasingEquations.hpp"
#include "Accessors.hpp"
#include "App.hpp"
#include "Conf.hpp"
#include "EventDispatcher.hpp"

#include <tr1/functional>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/function.hpp>
#include <boost/lexical_cast.hpp>
#include <utility>
#include <sstream>
#include <iostream>

using namespace psc;
using namespace presenter;
using namespace accessor;
using namespace easing;

using std::tr1::bind;
using std::tr1::ref;
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

    function<void(view::pSprite&)> click1_1 = bind(&MainMenu::menu1_1_click, this, _1);
    function<void(view::pSprite&)> click2_1 = bind(&MainMenu::menu2_1_click, this, _1);

    temp->addSpriteText("text", text.S("text"), text.S("font"), click1_1, m_text_size, true)
         .getSprite("text").set<Pos2D>( vec2( 0, height ) )
         .tween<SineCirc, Alpha>(0, text.I("glow_period"), -1);

    ctrl::EventDispatcher::i().subscribe_timer( bind(&MainMenu::initDecorator, this), shared_from_this(), 30 );

    return shared_from_this();
}

void MainMenu::initDecorator()
{
    utils::map_any deco = config.M("decorator");

    int const w = Conf::i().SCREEN_W;
    int const h = Conf::i().SCREEN_H;
    int const size = w * deco.F("size_factor");
    int const num_w = (w/size)*1.5;
    int const num_h = (h/size)*1.5 + 1;
    int const outgoing = size * 1.41f;
    int const contract = size / 3;

    vec2 start1( -outgoing, contract),  end1(w+outgoing, contract);     //line1
    vec2 start2(w-contract, -outgoing), end2(w-contract, h+outgoing);   //line2
    vec2 start3(w+outgoing, h-contract),end3( -outgoing, h-contract);   //line3
    vec2 start4( contract,  h+outgoing),end4( contract,  -outgoing);    //line4

    int const time_w = 10000;            //1280 768 is best factor
    int const time_h = time_w * (w/h);   //1280 768 is best factor

    int const color_num = deco.I("color_num");
    for(int i=0; i < color_num * 4; ++i) {
        std::string name;
        switch( i/4 ) {
            case 0: name += "-r-"; break;
            case 1: name += "-g-"; break;
            case 2: name += "-b-"; break;
            case 3: name += "-y-"; break;
        }
        paths.push_back( deco.S("path") + name +
            std::string( boost::lexical_cast<std::string>((i%4)+1) ) );
    }

    function<void()> step4 =
        bind(&MainMenu::initDecoInner_, this, size, start4, end4, num_h, time_h, 100, function<void()>());
    function<void()> step3 =
        bind(&MainMenu::initDecoInner_, this, size, start3, end3, num_w, time_w, 75, step4);
    function<void()> step2 =
        bind(&MainMenu::initDecoInner_, this, size, start2, end2, num_h, time_h, 50, step3);
    function<void()> step1 =
        bind(&MainMenu::initDecoInner_, this, size, start1, end1, num_w, time_w, 25, step2);

    ctrl::EventDispatcher::i().subscribe_timer(step1, shared_from_this(), 0);
}

//helper
void color_offset(data::Color& col, utils::vector_any& offset)
{
    int const blue = 0xff;
    int const green = blue<<8;
    int const red = green<<8;
    int const yellow = red|green;
    switch ( col.rgb() ) {
        case red:
            col.r( col.r() + offset.V(0).I(0) );
            col.g( col.g() + offset.V(0).I(1) );
            col.b( col.b() + offset.V(0).I(2) ); break;
        case green:
            col.r( col.r() + offset.V(1).I(0) );
            col.g( col.g() + offset.V(1).I(1) );
            col.b( col.b() + offset.V(1).I(2) ); break;
        case blue:
            col.r( col.r() + offset.V(2).I(0) );
            col.g( col.g() + offset.V(2).I(1) );
            col.b( col.b() + offset.V(2).I(2) ); break;
        case yellow:
            col.r( col.r() + offset.V(3).I(0) );
            col.g( col.g() + offset.V(3).I(1) );
            col.b( col.b() + offset.V(3).I(2) ); break;
    }
}

void MainMenu::initDecoInner_(int base_size, vec2 const& from, vec2 const& dest,
                              int const& num, int const& time, int const& load_stat,
                              function<void()> const& cb)
{
    utils::map_any    deco = config.M("decorator");
    utils::vector_any offset = deco.V("color_offset");
    int const size_random = deco.I("size_random_percent");
    int const rot_dur     = deco.I("rotation_duration_ms");
    int const rot_rand    = deco.I("rotation_random_ms");
//    int const color_num   = deco.I("color_num");
    int const bias        = base_size / 3;

    int const length = (dest - from).getLength();
    int const gap = length / num;
    base_size *= ( 1 + (size_random/100.0f) );

    for(int i=0, range=0; i < num; ++i, range += gap)
    {
//        data::Color col = data::Color::from_id(0, color_num);
//        color_offset( col, offset );

        int rand_bias   = utils::random( bias ) - bias/2;
        vec3 from3d( from.X + rand_bias, -from.Y + rand_bias, 100 );
        vec3 dest3d( dest.X + rand_bias, -dest.Y + rand_bias, -100 );
        view::pSprite temp = view::Sprite::create(
            paths[ utils::random(paths.size()) ], mainmenu_scene_, base_size, base_size, true);

        float scale = utils::random( size_random )/100.0f + 1;
        float rot_duration = rot_dur + utils::random( rot_rand );
        float delay = utils::random( rot_duration );
        float time_position = time * range / length;
        temp->//set<ColorDiffuse>( 0xff000000 | col.rgb() )
              set<GradientDiffuse>(255)
             .set<Scale>(vec3(scale, scale, 1))
             .tween<Linear, Rotation>(vec3(0, 0, 360), rot_duration, -1, 0, -delay )
             .tween<Linear, Pos3D>(from3d, dest3d, time, -1, 0, -time_position );

        temp->tween<IOCubic, GradientDiffuse>(128, 255, time, -1, 0, -time_position );

//        if( col.r() > 0 )
//            temp->tween<IOCubic, Red>(  col.r()>>2, col.r(), time, -1, 0, -time_position );
//        if( col.g() > 0 )
//            temp->tween<IOCubic, Green>(col.g()>>2, col.g(), time, -1, 0, -time_position );
//        if( col.b() > 0 )
//            temp->tween<IOCubic, Blue>( col.b()>>2, col.b(), time, -1, 0, -time_position );

        deco_cubes_.push_back( temp );
    }
    App::i().setLoading(load_stat);
    if( cb )
        ctrl::EventDispatcher::i().subscribe_timer(cb, shared_from_this(), 10);
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

MainMenu& MainMenu::showMenu(std::string const& name)
{
    view::pMenu sprite = menus_[name];
    boost::function<void()> f = (BLL::var(animating_) = false);
    sprite->tween<OCirc, Pos2D>(vec2(320, 200), 2000, 0, f );
    sprite->tweenAll<Linear, Alpha>(255, 1000);
    sprite->set<Visible>(true);
    return *this;
}

MainMenu& MainMenu::hideMenu(std::string const& name)
{
    view::pMenu sprite = menus_[name];
    function<void()> endcall = bind(&view::Sprite::set<Visible>, sprite.get(), false);
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
