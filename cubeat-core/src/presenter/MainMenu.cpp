
#include "presenter/MainMenu.hpp"
#include "presenter/PlayerView.hpp"
#include "view/Scene.hpp"
#include "view/Menu.hpp"
#include "view/Sprite.hpp"
#include "view/SpriteText.hpp"
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
using utils::to_s;

using std::tr1::bind;
using std::tr1::ref;
using std::tr1::cref;
using std::tr1::function;
using namespace std::tr1::placeholders;

namespace BLL = boost::lambda;

MainMenu::~MainMenu()
{
    std::cout << "MainMenu destructing ..." << std::endl;
}

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

    temp->addSpriteText("text", text.S("text"), text.S("font"), 0, m_text_size, true)
         .getSpriteText("text").tween<SineCirc, Alpha>(0, (unsigned int)text.I("glow_period"), -1)
         .set<Pos2D>( vec2( 0, height ) );


    setupMenus();

    ctrl::EventDispatcher::i().subscribe_timer( bind(&MainMenu::initDecorator, this), shared_from_this(), 30 );

    function<void(int, int)> clickA = bind(&MainMenu::push_start, this);
    btn_start_ = pDummy(new int);
    BOOST_FOREACH(ctrl::Input const* input, ctrl::InputMgr::i().getInputs())
        ctrl::EventDispatcher::i().subscribe_btn_event( clickA, btn_start_, &input->trig1(), ctrl::BTN_PRESS);

    player1focus_ = 0;
    player2focus_ = 0;
    player1num_ = 0;
    player2num_ = 0;
    stagenum_ = 0;
    game_mode_ = PPL_VS_PPL;
    two_players_ = true;

    return shared_from_this();
}

//temporary menu layer setup, need config.
void MainMenu::setupMenus()
{
    view::pMenu mode = view::Menu::create("", mainmenu_scene_, 1, 1, true);
    menus_.insert( std::make_pair("mode_select", mode) );

    mode->addSpriteText("text", "choose a game mode", "Star Jedi", 0, 40, false);
    mode->getSprite("text").set<Pos2D>( vec2(-300, -160) );
    mode->addSpriteText("multi_no_cpu", "player vs player", "Star Jedi", 0, 40, false);
    mode->getSprite("multi_no_cpu").set<Pos2D>( vec2(-150, -100) );
    mode->addSpriteText("multi_one_cpu", "player vs cpu", "Star Jedi", 0, 40, false);
    mode->getSprite("multi_one_cpu").set<Pos2D>( vec2(-150, -40) );
    mode->addSpriteText("multi_two_cpu", "cpu vs cpu demo", "Star Jedi", 0, 40, false);
    mode->getSprite("multi_two_cpu").set<Pos2D>( vec2(-150, 20) );
    mode->addSpriteText("puzzle", "puzzle game", "Star Jedi", 0, 40, false);
    mode->getSprite("puzzle").set<Pos2D>( vec2(-150, 80) );

    view::pMenu temp = view::Menu::create("", mainmenu_scene_, 1, 1, true);
    menus_.insert( std::make_pair("player_select", temp) );

    temp->addSpriteText("text", "choose character", "Star Jedi", 0, 40, true);
    temp->getSprite("text").set<Pos2D>( vec2(-150, -160) );
    for( int i = 1; i <= 5; ++i ) {
        pvlist_.push_back(
            PlayerView::create(("config/char/char"+to_s(i))+".zzml", temp, vec2(-500 + i*150, 20) ) );
        pvlist_.back()->switchCharacterState( PlayerView::STAND );
        pvlist_.back()->switchCharacterFace( PlayerView::NORMAL );

        view::pMenu m = pvlist_.back()->getView();
        m->addSprite("hitarea", 0, 96, 192, true);
        m->getSprite("hitarea").setDepth(100).set<Pos2D>(vec2(-48, 48)).set<GradientDiffuse>(128);
        if( i < 3 )
            pvlist_.back()->flipPosition();
        else {
            vec2 pos = m->get<Pos2D>(); pos.X += 100;
            m->set<Pos2D>( pos );
        }
        m->set<Scale>(vec3(1.2, 1.2, 1.2));
    }

    view::pMenu temp2= view::Menu::create("", mainmenu_scene_, 1, 1, true);
    menus_.insert( std::make_pair("stage_select", temp2) );

    function<void(view::pSprite&)> stage1 = bind(&MainMenu::stage_select, this, _1, "jungle");
    function<void(view::pSprite&)> stage2 = bind(&MainMenu::stage_select, this, _1, "school");
    function<void(view::pSprite&)> stage3 = bind(&MainMenu::stage_select, this, _1, "street");
    function<void(view::pSprite&)> stage4 = bind(&MainMenu::stage_select, this, _1, "hospital");
    temp2->addSpriteText("text", "choose stage", "Star Jedi", 0, 40, true);
    temp2->addSprite("stage1", stage1, 250, 150, true, "stage_preview/jungle_thumb")
          .addSprite("stage2", stage2, 250, 150, true, "stage_preview/school_thumb")
          .addSprite("stage3", stage3, 250, 150, true, "stage_preview/street_thumb")
          .addSprite("stage4", stage4, 250, 150, true, "stage_preview/hospital_thumb");
    temp2->getSprite("text").set<Pos2D>( vec2(-150, -160) );
    temp2->getSprite("stage1").set<Pos2D>( vec2(-200, -40) );
    temp2->getSprite("stage2").set<Pos2D>( vec2(200, -40) );
    temp2->getSprite("stage3").set<Pos2D>( vec2(-200, 130) );
    temp2->getSprite("stage4").set<Pos2D>( vec2(200, 130) );

    temp->set<Pos2D>( vec2(-300, Conf::i().SCREEN_H/2) );
    temp2->set<Pos2D>( vec2(-400, Conf::i().SCREEN_H/2) );
    mode->set<Pos2D>( vec2(-300, Conf::i().SCREEN_H/2) );

    player1text_ = view::SpriteText::create("player1", temp, "Star Jedi", 24, true, data::Color(255,0,0));
    player2text_ = view::SpriteText::create("player2", temp, "Star Jedi", 24, true, data::Color(0,0,255));
    player1text_->set<Pos2D>( vec2(-300, 150) ).set<Alpha>(100);
    player2text_->set<Pos2D>( vec2(-300, 180) ).set<Alpha>(100);

    hideMenu("mode_select").hideMenu("player_select").hideMenu("stage_select");
}

void MainMenu::end()
{
    if( game_mode_ == PPL_VS_PPL )
        App::i().launchMultiplayer(conf1p_, conf2p_, stage_, 0);
    else if( game_mode_ == PPL_VS_CPU )
        App::i().launchMultiplayer(conf1p_, conf2p_, stage_, 1);
    else if( game_mode_ == CPU_VS_CPU )
        App::i().launchMultiplayer(conf1p_, conf2p_, stage_, 2);
    else if( game_mode_ == PUZZLE )
        App::i().launchPuzzle(conf1p_, stage_, 3);
    std::cout << "MainMenu end call finished." << std::endl;
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
    waypoints.push_back( vec3(contract, -contract, 1000) );
    waypoints.push_back( vec3(w-contract, -(contract), 500) );
    waypoints.push_back( vec3(w-contract, -(h-contract), 0) );
    waypoints.push_back( vec3(contract, -(h-contract), -500) );
    waypoints.push_back( vec3(contract, outgoing, -1000) );

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
    if( animating_ ) return;

    Sound::i().play("4/4b.wav");
    btn_start_.reset();
    //menus_["start_menu"]->getSpriteText("text").tween<SineCirc, Alpha>(0, 255, 120u, 2);

    hideMenu("start_menu").showMenu("mode_select");
    setup_mode_selecting_buttons();

    animating_ = true;
}

//temp: button setup
void MainMenu::setup_mode_selecting_buttons()
{
    function<void(int, int)> clickB = bind(&MainMenu::go_back_from_to, this, "mode_select", "start_menu");
    btn_back1_ = pDummy(new int);
    btn_back2_ = pDummy(new int);
    ctrl::Input const* input1 = ctrl::InputMgr::i().getInputByIndex(0);
    ctrl::Input const* input2 = ctrl::InputMgr::i().getInputByIndex(1);
    ctrl::EventDispatcher::i().subscribe_btn_event( clickB, btn_back1_, &input1->trig2(), ctrl::BTN_PRESS);
    ctrl::EventDispatcher::i().subscribe_btn_event( clickB, btn_back2_, &input2->trig2(), ctrl::BTN_PRESS);

    function<void(view::pSprite&)> mode1 = bind(&MainMenu::mode_select, this, _1, (int)PPL_VS_PPL);
    function<void(view::pSprite&)> mode2 = bind(&MainMenu::mode_select, this, _1, (int)PPL_VS_CPU);
    function<void(view::pSprite&)> mode3 = bind(&MainMenu::mode_select, this, _1, (int)CPU_VS_CPU);
    function<void(view::pSprite&)> mode4 = bind(&MainMenu::mode_select, this, _1, (int)PUZZLE);
    menus_["mode_select"]->setCallbackToSprite("multi_no_cpu", mode1);
    menus_["mode_select"]->setCallbackToSprite("multi_one_cpu", mode2);
    menus_["mode_select"]->setCallbackToSprite("multi_two_cpu", mode3);
    menus_["mode_select"]->setCallbackToSprite("puzzle", mode4);
}

void MainMenu::go_back_from_to(std::string const& from, std::string const& to)
{
    if( animating_ ) return;

    Sound::i().play("4/4c.wav");
    hideMenu(from).showMenu(to);
    animating_ = true;
    btn_back1_.reset();
    btn_back2_.reset();

    if( to == "start_menu" ) {
        function<void(int, int)> clickA = bind(&MainMenu::push_start, this);
        btn_start_ = pDummy(new int);
        BOOST_FOREACH(ctrl::Input const* input, ctrl::InputMgr::i().getInputs())
            ctrl::EventDispatcher::i().subscribe_btn_event( clickA, btn_start_, &input->trig1(), ctrl::BTN_PRESS);
    }
    else if( to == "mode_select" ) {
        player1focus_ = 0;
        player2focus_ = 0;
        player1num_ = 0;
        player2num_ = 0;
        two_players_ = true;
        game_mode_ = PPL_VS_PPL;
        setup_mode_selecting_buttons();
    }
    else if( to == "player_select" ) {
        player1text_->set<Pos2D>( vec2(-300, 150) ).set<Alpha>(100);
        player2text_->set<Pos2D>( vec2(-300, 180) ).set<Alpha>(100);
        player1focus_ = 0;
        player2focus_ = 0;
        player1num_ = 0;
        player2num_ = 0;
        setup_player_selecting_buttons();
    }
    else if( to == "stage_select" ) {
    }
}

void MainMenu::mode_select(view::pSprite& sv, int mode)
{
    if( animating_ ) return;

    game_mode_ = mode;
    Sound::i().play("4/4b.wav");

    menus_["mode_select"]->setCallbackToSprite("multi_no_cpu", 0);
    menus_["mode_select"]->setCallbackToSprite("multi_one_cpu", 0);
    menus_["mode_select"]->setCallbackToSprite("multi_two_cpu", 0);
    menus_["mode_select"]->setCallbackToSprite("puzzle", 0);

    if( game_mode_ == SINGLE || game_mode_ == PUZZLE )
        two_players_ = false;

    player_choosing();
}

void MainMenu::player_choosing()
{
    hideMenu("mode_select").showMenu("player_select");
    animating_ = true;

    player1focus_ = 0;
    player2focus_ = 0;
    player1num_ = 0;
    player2num_ = 0;

    setup_player_selecting_buttons();
}

//temp: very temp =_=
void MainMenu::setup_player_selecting_buttons()
{
    player1text_->set<Pos2D>( vec2(-300, 150) ).set<Alpha>(100);
    player2text_->set<Pos2D>( vec2(-300, 180) ).set<Alpha>(100);

    ctrl::Input const* input1 = ctrl::InputMgr::i().getInputByIndex(0);
    ctrl::Input const* input2 = ctrl::InputMgr::i().getInputByIndex(1);

    //temp: button "back"
    function<void(int, int)> clickB = bind(&MainMenu::go_back_from_to, this, "player_select", "mode_select");
    btn_back1_ = pDummy(new int);
    btn_back2_ = pDummy(new int);
    ctrl::EventDispatcher::i().subscribe_btn_event( clickB, btn_back1_, &input1->trig2(), ctrl::BTN_PRESS);
    ctrl::EventDispatcher::i().subscribe_btn_event( clickB, btn_back2_, &input2->trig2(), ctrl::BTN_PRESS);

    btn_choose_player1_ = pDummy(new int);

    ctrl::EventDispatcher::i().subscribe_btn_event(
        bind(&MainMenu::player1_select, this, -1), btn_choose_player1_, &input1->wep1(), ctrl::BTN_PRESS);
    ctrl::EventDispatcher::i().subscribe_btn_event(
        bind(&MainMenu::player1_select, this, 1), btn_choose_player1_, &input1->wep2(), ctrl::BTN_PRESS);
    ctrl::EventDispatcher::i().subscribe_btn_event(
        bind(&MainMenu::player1_checked, this), btn_choose_player1_, &input1->trig1(), ctrl::BTN_PRESS);

    int i = 1;
    BOOST_FOREACH(pPlayerView& pv, pvlist_) {
        pv->getView()->getSprite("body").onEnterFocus( input1 ) =
            bind(&MainMenu::player1_getfocus, this, i);
        ++i;
    }

    player1focus_ = 1;

    if( two_players_ ) {
        btn_choose_player2_ = pDummy(new int);

        ctrl::EventDispatcher::i().subscribe_btn_event(
            bind(&MainMenu::player2_select, this, -1), btn_choose_player2_, &input2->wep1(), ctrl::BTN_PRESS);
        ctrl::EventDispatcher::i().subscribe_btn_event(
            bind(&MainMenu::player2_select, this, 1), btn_choose_player2_, &input2->wep2(), ctrl::BTN_PRESS);
        ctrl::EventDispatcher::i().subscribe_btn_event(
            bind(&MainMenu::player2_checked, this), btn_choose_player2_, &input2->trig1(), ctrl::BTN_PRESS);

        i = 1;
        BOOST_FOREACH(pPlayerView& pv, pvlist_) {
            pv->getView()->getSprite("hitarea").onEnterFocus( input2 ) =
                bind(&MainMenu::player2_getfocus, this, i);
            ++i;
        }

        player2focus_ = 1;
        player2text_->set<Visible>(true);
    }
    else {
        player2text_->set<Visible>(false);
    }
}

void MainMenu::player1_select(int num)
{
    if( player1focus_ + num < 1 ) return;
    if( player1focus_ + num > 5 ) return;
    Sound::i().play("4/4a.wav");
    player1focus_ += num;
    vec2 pos = player1text_->get<Pos2D>();
    pos.X += 150 * num;
    player1text_->set<Pos2D>(pos);
}

void MainMenu::player2_select(int num)
{
    if( player2focus_ + num < 1 ) return;
    if( player2focus_ + num > 5 ) return;
    Sound::i().play("4/4a.wav");
    player2focus_ += num;
    vec2 pos = player2text_->get<Pos2D>();
    pos.X += 150 * num;
    player2text_->set<Pos2D>(pos);
}

void MainMenu::player1_getfocus(int num)
{
    if( player1num_ != 0 ) return; //checked;
    player1focus_ = num;
    vec2 pos = player1text_->get<Pos2D>();
    pos.X = -300 + 150 * (num-1);
    player1text_->set<Pos2D>(pos);
}

void MainMenu::player2_getfocus(int num)
{
    if( player2num_ != 0 ) return; //checked;
    player2focus_ = num;
    vec2 pos = player2text_->get<Pos2D>();
    pos.X = -300 + 150 * (num-1);
    player2text_->set<Pos2D>(pos);
}

void MainMenu::player1_checked()
{
    if( animating_ ) return;
    Sound::i().play("4/4b.wav");
    player1num_ = player1focus_;
    player1text_->set<Alpha>(255);
    btn_choose_player1_.reset();
    if( two_players_ ) {
        if( player1num_ != 0 && player2num_ != 0 )
            stage_choosing();
    }
    else
        stage_choosing();
}

void MainMenu::player2_checked()
{
    if( animating_ ) return;
    Sound::i().play("4/4b.wav");
    player2num_ = player2focus_;
    player2text_->set<Alpha>(255);
    btn_choose_player2_.reset();
    if( player1num_ != 0 && player2num_ != 0 )
        stage_choosing();
}

void MainMenu::stage_choosing()
{
    animating_ = true;
    hideMenu("player_select").showMenu("stage_select");

    //temp: button "back"
    function<void(int, int)> clickB = bind(&MainMenu::go_back_from_to, this, "stage_select", "player_select");
    btn_back1_ = pDummy(new int);
    btn_back2_ = pDummy(new int);
    ctrl::Input const* input1 = ctrl::InputMgr::i().getInputByIndex(0);
    ctrl::Input const* input2 = ctrl::InputMgr::i().getInputByIndex(1);
    ctrl::EventDispatcher::i().subscribe_btn_event( clickB, btn_back1_, &input1->trig2(), ctrl::BTN_PRESS);
    ctrl::EventDispatcher::i().subscribe_btn_event( clickB, btn_back2_, &input2->trig2(), ctrl::BTN_PRESS);
}

void MainMenu::stage_select(view::pSprite& sp, std::string name)
{
    if( animating_ ) return;
    conf1p_ = "config/char/char"+to_s(player1num_)+".zzml";
    stage_ = "config/stage/"+name+".zzml";

    if( two_players_ )
        conf2p_ = "config/char/char"+to_s(player2num_)+".zzml";

    Sound::i().play("4/4b.wav");

    fadeAllOut(1000);
    function<void()> cb = bind(&MainMenu::end, this);
    ctrl::EventDispatcher::i().subscribe_timer(cb, shared_from_this(), 1100);

    menus_["stage_select"]->setCallbackToSprite("stage1", 0);
    menus_["stage_select"]->setCallbackToSprite("stage2", 0);
    menus_["stage_select"]->setCallbackToSprite("stage3", 0);
    menus_["stage_select"]->setCallbackToSprite("stage4", 0);
}

MainMenu& MainMenu::showMenu(std::string const& name)
{
    view::pMenu sprite = menus_[name];
    boost::function<void()> const& f = (BLL::var(animating_) = false);
    int y = Conf::i().SCREEN_H/2;
    sprite->tweenAll<Linear, Alpha>(255, 1000u);
    if( name == "start_menu" ) { //temp: special case
        y = Conf::i().SCREEN_H/2 - sprite->get<Size2D>().Y/3;
        sprite->getSpriteText("text")
               .tween<SineCirc, Alpha>(255, (unsigned int)config.M("text").I("glow_period"), -1);
    }
    sprite->tween<OCirc, Pos2D>(vec2(Conf::i().SCREEN_W/2, y), 1000, 0, f );

    sprite->set<Visible>(true);

    //temp: special case
    if( name == "player_select" ) {
        BOOST_FOREACH(pPlayerView& pv, pvlist_) {
            pv->getView()->tweenAll<Linear, Alpha>(255, 1000u);
            //Warning: stupid code: wipe out hitarea's tween..
            //pv->getView()->getSprite("hitarea").tween<Linear, Alpha>(0, 10u);
        }
    }
    return *this;
}

MainMenu& MainMenu::hideMenu(std::string const& name)
{
    view::pMenu sprite = menus_[name];
    function<void()> const& endcall = bind(&view::Sprite::set<Visible>, sprite.get(), false);
    int y = Conf::i().SCREEN_H/2;
    if( name == "start_menu" ) //temp: special case
        y = Conf::i().SCREEN_H/2 - sprite->get<Size2D>().Y/3;
    sprite->tween<ICirc, Pos2D>(vec2(-200, y), 1000, 0, endcall);
    sprite->tweenAll<Linear, Alpha>(0, 500u);

    //temp: special case
    if( name == "player_select" ) {
        BOOST_FOREACH(pPlayerView& pv, pvlist_) {
            pv->getView()->tweenAll<Linear, Alpha>(0, 1000u);
            //Warning: stupid code: wipe out hitarea's tween..
            //pv->getView()->getSprite("hitarea").tween<Linear, Alpha>(0, 10u);
        }
    }
    return *this;
}

MainMenu& MainMenu::cubeRearrange()
{
    return *this;
}

void MainMenu::cycle()
{
    BOOST_FOREACH(pPlayerView& pv, pvlist_) {
        if( pv->getState() == PlayerView::NONE )
            pv->switchCharacterState( PlayerView::STAND );
    }
    mainmenu_scene_->redraw();
}
