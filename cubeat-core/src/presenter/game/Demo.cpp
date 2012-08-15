
#include "presenter/game/Demo.hpp"

#include "data/ViewSetting.hpp"

#include "view/Scene.hpp"
#include "view/AnimatedSprite.hpp"
#include "view/SpriteText.hpp"
#include "view/Menu.hpp"
#include "Accessors.hpp"
#include "EasingEquations.hpp"

#include "presenter/Stage.hpp"
#include "presenter/PlayerView.hpp"
#include "presenter/Map.hpp"
#include "presenter/cube/ViewSpriteMaster.hpp"
#include "presenter/PlayerAbility.hpp"

#include "EventDispatcher.hpp"
#include "ctrl/TimerDispatcher.hpp"
#include "Input.hpp"
#include "Player.hpp"
#include "ctrl/AIPlayer.hpp"
#include "Weapon.hpp"
#include "audio/Sound.hpp"
#include "Conf.hpp"
#include "App.hpp"

#include "utils/Random.hpp"
#include "utils/dictionary.hpp"
#include "utils/MapLoader.hpp"
#include "utils/to_s.hpp"
#include "utils/Logger.hpp"
#include "script/lua_utility.hpp"
#include <boost/foreach.hpp>
#include <algorithm>

using namespace psc;
using namespace presenter;
using namespace game;
using namespace easing;
using namespace accessor;
using utils::to_s;
using namespace std::tr1::placeholders;

Demo::Demo()
    :c1p_("char/char1_new"), c2p_("char/char2_new"), sconf_("stage/jungle"), num_of_cpu_(1),
     ai_level_(2), some_ui_inited_(false), L_(0)
{
}

Demo::~Demo()
{
    lua_close(L_);

    std::cout << "Demoplayer Game destructing ..." << std::endl;
    std::cout << " player0 use count: " << player0_.use_count() << std::endl;
    std::cout << " player1 use count: " << player1_.use_count() << std::endl;
}

pDemo Demo::init()
{
    scene_    = view::Scene::create("game");
    ui_scene_ = view::Scene::create("ui");
    //scene_->setTo2DView().enableGlobalHittingEvent(); //2011.03.28 weapon temporary removal
    scene_->setTo2DView();
    ui_scene_->setTo2DView();
    ctrl::EventDispatcher::i().get_timer_dispatcher("ui");

    gameplay_ = Conf::i().config_of("gameplay/multi");
    PlayerAbility::ability_modify( gameplay_.M("ability_constants") );
    uiconf_   = Conf::i().config_of("ui/demo_layout");
    // setup stage & ui & player's view objects:
    stage_ = presenter::Stage::create( sconf_.size() ? sconf_ : "stage/jungle" );

    L_ = luaL_newstate();
    luaL_openlibs(L_);
    script::Lua::run_script(L_, Conf::i().script_path("ui/demo/demo.lua").c_str());
    script::Lua::call(L_, "init", static_cast<void*>(this));
    script::Lua::call(L_, "mainmenu");

    /*** Ok, we handover the control to script from here on. ***/

    ctrl::EventDispatcher::i().get_timer_dispatcher("global")->subscribe(
        bind(loading_complete_, 100), 100);

    audio::Sound::i().playBGM_AB("day_a.ogg", "day_b.ogg");

    return shared_from_this();
}

void Demo::init_(int const& num_of_cpu, std::string const& c1p, std::string const& c2p, std::string const& scene_name, bool const& inplace)
{
    num_of_cpu_ = num_of_cpu;
    c1p_ = c1p;
    c2p_ = c2p;
    sconf_ = scene_name;
    music_state_ = false;
    music_state_old_ = false;

    passive_conf0_ = Conf::i().config_of(c1p).M("passive_mod");
    passive_conf1_ = Conf::i().config_of(c2p).M("passive_mod");

    //stop timer for now because the initial loading gonna be some time.
    ctrl::EventDispatcher::i().get_timer_dispatcher("game")->stop();
    ctrl::EventDispatcher::i().get_timer_dispatcher("ui")->stop();
    scene_->allowPicking(false);

    //load scene again first
    if( !inplace ) {
        stage_->releaseResource();
        stage_.reset();
    }
    stage_ = presenter::Stage::create( sconf_.size() ? sconf_ : "stage/jungle" );

    data::pViewSetting s0, s1;

    s0 = data::ViewSetting::create( uiconf_.M("mapview0") );
    s1 = data::ViewSetting::create( uiconf_.M("mapview1") );

    std::string ai_temp[4] =
        {"ai/easy.lua", "ai/normal.lua", "ai/hard.lua", "ai/insane.lua"};

    ///THIS IS IMPORTANT, ALL PLAYERS MUST BE DEFINED FIRST.
    ctrl::Input* input0 = ctrl::InputMgr::i().getInputByIndex(0);
    ctrl::Input* input1 = ctrl::InputMgr::i().getInputByIndex(1);
    if( num_of_cpu_ == 0 ) {
        player0_ = ctrl::Player::create(input0, 0);
        player1_ = ctrl::Player::create(input1, 1);
    }
    else if( num_of_cpu_ == 1 ) {
        input1->setControlledByAI(true);
        player0_ = ctrl::Player::create(input0, 0);
        player1_ = ctrl::AIPlayer::create(input1, 1, ai_temp[ai_level_]);
    }
    else {
        input0->setControlledByAI(true);
        input1->setControlledByAI(true);
        //std::random_shuffle(ai_temp, ai_temp + 4);
        player0_ = ctrl::AIPlayer::create(input0, 0, ai_temp[2]);
        player1_ = ctrl::AIPlayer::create(input1, 1, ai_temp[2]);
    }
    player0_->push_ally(0).push_enemy(1);
    player1_->push_ally(1).push_enemy(0);

    // setup player settings
    player0_->set_config( !passive_conf0_.empty() ? passive_conf0_.M("weapon") : gameplay_.M("player1").M("weapon") );
    player1_->set_config( !passive_conf1_.empty() ? passive_conf1_.M("weapon") : gameplay_.M("player2").M("weapon") );

    // setup map0
    data::pMapSetting set0 = data::MapSetting::create( gameplay_.M("player1") );

    // setup map1
    data::pMapSetting set1 = data::MapSetting::create( gameplay_.M("player2") );

    // update map settings with player passive modification:
    set0->apply_player_passive_mods( passive_conf0_ );
    set1->apply_player_passive_mods( passive_conf1_ );

    // some interacting settings have to be resolved first:
    set0->damage_factor( set0->damage_factor() * set1->negate_damage_factor() );
    set1->damage_factor( set1->damage_factor() * set0->negate_damage_factor() );

    map0_ = presenter::Map::create(set0);
    //map0_ = utils::MapLoader::load(0); //temp: this is for exciting demo.
    map0_->set_view_master( presenter::cube::ViewSpriteMaster::create(scene_, s0, player0_) );

    map1_ = presenter::Map::create(set1);
    //map1_ = utils::MapLoader::load(1); //temp: this is for exciting demo.
    map1_->set_view_master( presenter::cube::ViewSpriteMaster::create(scene_, s1, player1_) );

    // setup garbage land
    map0_->push_garbage_land(map1_);
    map1_->push_garbage_land(map0_);
    map0_->lose_event(bind(&Demo::end, this, ref(map0_)));
    map1_->lose_event(bind(&Demo::end, this, ref(map1_)));
    map0_->stop_dropping(); //make them stop dropping from the very beginning.
    map1_->stop_dropping();

    ///NEW: MAKE PLAYER KNOWS ABOUT MAP
    std::vector< presenter::wpMap > map_list;
    map_list.push_back(map0_);
    map_list.push_back(map1_);
    player0_->setMapList( map_list );
    player1_->setMapList( map_list );

    setup_ui();

    min_ = 0, sec_ = 0 ,last_garbage_1p_ = 0, last_garbage_2p_ = 0;

    using std::tr1::bind;

    //start timer here.
    ctrl::EventDispatcher::i().get_timer_dispatcher("game")->set_speed(3.0);
    ctrl::EventDispatcher::i().get_timer_dispatcher("ui")->set_speed(3.0);
    ctrl::EventDispatcher::i().get_timer_dispatcher("input")->set_speed(3.0);
    ctrl::EventDispatcher::i().get_timer_dispatcher("global")->set_speed(3.0);
    ctrl::EventDispatcher::i().get_timer_dispatcher("game")->start();
    ctrl::EventDispatcher::i().get_timer_dispatcher("ui")->start();

//    ctrl::EventDispatcher::i().get_timer_dispatcher("global")->subscribe(
//        bind(&Demo::game_start, this), 4000);

    //start music

    audio::Sound::i().stopAll(); //stop old
    stage_->playBGM();

    //ready_go(4);
    starting_effect(inplace);
}

void Demo::init_vs_ppl(std::string const& c1p, std::string const& c2p, std::string const& scene_name)
{
    init_(0, c1p, c2p, scene_name);
}

void Demo::init_vs_cpu(std::string const& c1p, std::string const& c2p, std::string const& scene_name)
{
    init_(1, c1p, c2p, scene_name);
}

void Demo::init_cpudemo(std::string const& c1p, std::string const& c2p, std::string const& scene_name)
{
    init_(2, c1p, c2p, scene_name);
}

void Demo::ask_for_tutorial()
{
    //call Lua
}

view::pScene Demo::get_ui_scene()
{
    return ui_scene_;
}

void Demo::quit()
{
    App::i().quit();
}

void Demo::leaving_effect()
{
    heatgauge1_->set<Visible>(false);
    heatgauge2_->set<Visible>(false);
    hide_upper_layer_ui();
    scene_->tween<ISine, Pos2D>(vec2( Conf::i().SCREEN_W(), - Conf::i().SCREEN_H()/2 ), 1000u);
    script::Lua::call(L_, "slide_in");

    audio::Sound::i().playBGM_AB("day_a.ogg", "day_b.ogg");

    ctrl::EventDispatcher::i().get_timer_dispatcher("game")->subscribe(
        bind(&Demo::cleanup, this), shared_from_this(), 1000); //1000 ms
}

void Demo::starting_effect(bool const& inplace)
{
    if( inplace ) {
        ready_go(4);
    }
    else {
        std::tr1::function<void()> cb = bind(&Demo::ready_go, this, 3);
        scene_->tween<OSine, Pos2D>(
            vec2( - Conf::i().SCREEN_W() * 2, - Conf::i().SCREEN_H()/2 ),
            vec2( - Conf::i().SCREEN_W() / 2, - Conf::i().SCREEN_H()/2 ),
            1000u, 0, cb);
    }
    script::Lua::call(L_, "slide_out", inplace);
}

//This is currently a mockup, of course we can't use normal fonts as countdown text. image needed.
void Demo::ready_go(int step)
{
    if ( step < 0 ) {
        ready_go_text_->tween<Linear, Alpha>(0, 500u);
        return;
    }
    else if ( step == 0 ) {
        audio::Sound::i().playBuffer("go.wav");
        ready_go_text_->changeText("go!");
        ready_go_text_->set<Scale>(vec3(1.5,1.5,1.5));
        ready_go_text_->tween<OElastic, Scale>(vec3(5,5,5), 900u, 0);

        game_start();
    }
    else if ( step <= 3 ) {
        audio::Sound::i().playBuffer("count.wav");
        ready_go_text_->showNumber(step);
        ready_go_text_->set<Scale>(vec3(1.5,1.5,1.5));
        ready_go_text_->set<Visible>(true);
        ready_go_text_->tween<OElastic, Scale>(vec3(5,5,5), 900u, 0);
    }
    ctrl::EventDispatcher::i().get_timer_dispatcher("global")->subscribe(
        std::tr1::bind(&Demo::ready_go, this, step-1), shared_from_this(), 1000);
}

void Demo::game_start()
{
    //note: bad area
    timer_ui_   = pDummy(new int);
    //note: end of bad area

    using std::tr1::bind;
    ctrl::EventDispatcher::i().get_timer_dispatcher("game")->subscribe(
        bind(&Demo::update_ui_by_second, this), timer_ui_, 1000, -1);

    BOOST_FOREACH(ctrl::Input const* input, ctrl::InputMgr::i().getInputs()) {
        ctrl::EventDispatcher::i().subscribe_btn_event(
            bind(&Demo::pause, this, input), shared_from_this(), &input->pause(), ctrl::BTN_PRESS);
    }

    if( num_of_cpu_ == 2 ) {
        //blocker_->set<Visible>(true);
        pause_note_text_->set<Visible>(true);
    }

    ctrl::EventDispatcher::i().get_timer_dispatcher("game")->start();
    scene_->allowPicking(true);

    player0_->subscribe_player_specific_interactions();
    player1_->subscribe_player_specific_interactions();

    map0_->start_dropping();
    map1_->start_dropping();

    if( num_of_cpu_ > 0 )
        player1_->startThinking();
    if( num_of_cpu_ > 1 )
        player0_->startThinking();
}

void Demo::setup_ui()
{
    utils::map_any const& base = uiconf_.M("base");
    ui_layout_ = view::Menu::create( base.S("layout_tex"), scene_, base.I("w"), base.I("h") );
    ui_layout_->set<Alpha>(192);

    utils::map_any const& misc = uiconf_.M("misc");
    BOOST_FOREACH(utils::pair_any const& it, misc) {
        std::string    const& key  = boost::any_cast<std::string const>(it.first);
        utils::map_any const& attr = boost::any_cast<utils::map_any const>(it.second);
        ui_layout_->
            addSpriteText(key, attr.S("text"), attr.S("font"), 0, attr.I("fsize"), attr.I("center") )
           .getSpriteText(key).set<Pos2D>( vec2(attr.I("x"), attr.I("y")) );
    }

    //2012.05 attack effect changed. I still need their position but don't show them
    ui_layout_->getSpriteText("gar1p").set<Visible>(false);
    ui_layout_->getSpriteText("gar2p").set<Visible>(false);

    vec2 center_pos1( uiconf_.I("character_center_x1"), uiconf_.I("character_center_y") );
    vec2 center_pos2( uiconf_.I("character_center_x2"), uiconf_.I("character_center_y") );
    pview1_ = presenter::PlayerView::create( c1p_, scene_, center_pos1 );
    pview2_ = presenter::PlayerView::create( c2p_, scene_, center_pos2 );
    pview2_->flipPosition();
    pview1_->setMap( map0_ );
    pview2_->setMap( map1_ );
    pview1_->setInput( ctrl::InputMgr::i().getInputByIndex(0) ); //temp: for pview to know input for rumbling wiimote
    pview2_->setInput( ctrl::InputMgr::i().getInputByIndex(1) ); //temp: for pview to know input for rumbling wiimote

    utils::map_any const& gauge_conf = uiconf_.M("heatgauge");
    vec2 gauge1_pos( gauge_conf.I("x_1p"), gauge_conf.I("y") );
    vec2 gauge2_pos( gauge_conf.I("x_2p"), gauge_conf.I("y") );

//2012.05 new heatgauge
//    heatgauge1_ = view::Sprite::create("heatgauge1", scene_, gauge_conf.I("w"), gauge_conf.I("h"), false);
//    heatgauge2_ = view::Sprite::create("heatgauge2", scene_, gauge_conf.I("w"), gauge_conf.I("h"), false);
//    heatgauge1_->set<Pos2D>( gauge1_pos ).set<ColorDiffuseVec3>( vec3(0,255,0) ).set<Alpha>(128)
//                .set<Rotation>(vec3(0, 0, gauge_conf.I("rotation")));
//    heatgauge2_->set<Pos2D>( gauge2_pos ).set<ColorDiffuseVec3>( vec3(0,255,0) ).set<Alpha>(128)
//                .set<Rotation>(vec3(0, 0, gauge_conf.I("rotation")));
    heatgauge1_ = view::Sprite::create("heat/0", ui_scene_, 96, 96, true);
    heatgauge2_ = view::Sprite::create("heat/0", ui_scene_, 96, 96, true);
    heatgauge1_->set<ColorDiffuseVec3>( vec3(0,255,0) ).set<Alpha>(192);
    heatgauge2_->set<ColorDiffuseVec3>( vec3(0,255,0) ).set<Alpha>(192);

    ui_layout_->setPickable(false);

    gauge1_flag_ = gauge2_flag_ = false; // ?????

    // other UI related texts here

    if( !some_ui_inited_ ) {
        //blocker_  = view::Sprite::create("blocker", ui_scene_, Conf::i().SCREEN_W() ,350, true);
        blocker_  = view::Sprite::create("blocker", ui_scene_, Conf::i().SCREEN_W() ,368, true);
        blocker_->set<Pos2D>( vec2(Conf::i().SCREEN_W() /2, Conf::i().SCREEN_H() /2) );
        blocker_->setDepth(-50).set<Alpha>(100).set<GradientDiffuse>(0).setPickable(false);

        end_text_ = view::SpriteText::create("play again?", ui_scene_, "kimberley", 30, true);
        end_text_->setPickable(false);
        end_text2_= view::SpriteText::create("\nyes: left click\nleave: right click", ui_scene_, "kimberley", 30, true);
        end_text2_->setPickable(false);
        win_t_    = view::Sprite::create("win", ui_scene_, 384, 192, true);
        lose_t_   = view::Sprite::create("lose", ui_scene_, 384, 192, true);

        pause_text_ = view::SpriteText::create("back to menu?", ui_scene_, "kimberley", 30, true);
        pause_text_->set<Pos2D>( vec2(Conf::i().SCREEN_W() /2, Conf::i().SCREEN_H() /2 + 60) );
        pause_text_->setDepth(-450).setPickable(false);

        pause_text2_ = view::SpriteText::create("\nyes: left click\nno: right click", ui_scene_, "kimberley", 30, true);
        pause_text2_->set<Pos2D>( vec2(Conf::i().SCREEN_W() /2, Conf::i().SCREEN_H() /2 + 100) );
        pause_text2_->setDepth(-450).setPickable(false);

        pause_t_ = view::Sprite::create("pause", ui_scene_, 384, 192, true);
        pause_t_->set<Pos2D>( vec2(Conf::i().SCREEN_W()/2, Conf::i().SCREEN_H()/2 - 50) );
        pause_t_->setDepth(-450).setPickable(false);

        ready_go_text_ = view::SpriteText::create("3", ui_scene_, "kimberley", 30, true);
        ready_go_text_->set<Pos2D>( vec2(Conf::i().SCREEN_W() /2, Conf::i().SCREEN_H() /2 + 20) ); //hacky
        ready_go_text_->setPickable(false);

        pause_note_text_ = view::SpriteText::create("press middle button to pause", ui_scene_, "kimberley", 30, true);
        pause_note_text_->set<Pos2D>( vec2(Conf::i().SCREEN_W() /2, Conf::i().SCREEN_H() - 30 ) );
        pause_note_text_->tween<SineCirc, Alpha>(0, 3000u, -1);
        pause_note_text_->setDepth(-100).setPickable(false);

        some_ui_inited_ = true;
    }

    // temp: hack for cutin effect
    ppl1_special_img_ = view::Sprite::create(c1p_.substr(5, 9)+"/cutin", ui_scene_, 425, 368, true);
    ppl1_special_img_->set<Pos2D>( vec2(Conf::i().SCREEN_W() /2, Conf::i().SCREEN_H() /2) ).setDepth(-100).setPickable(false);
    ppl1_special_img_->set<Visible>(false);
    ppl1_special_img_->textureFlipH();

    hide_upper_layer_ui();
}

void Demo::hide_upper_layer_ui()
{
    if( some_ui_inited_ ) {
        blocker_->set<Visible>(false);
        blocker_->set<Visible>(false);
        end_text_->set<Visible>(false);
        end_text2_->set<Visible>(false);
        pause_text_->set<Visible>(false);
        pause_text2_->set<Visible>(false);
        ready_go_text_->set<Visible>(false);
        pause_note_text_->set<Visible>(false);
        win_t_->set<Visible>(false);
        lose_t_->set<Visible>(false);
        pause_t_->set<Visible>(false);
    }
}

//2012.05 fix
void Demo::update_heatgauge(ctrl::pPlayer player, view::pSprite gauge, bool& out_flag)
{
    gauge->set<Pos2D>( player->input()->getCursor()->get<Pos2D>() );
    //gauge->set<Scale>( vec3(player->heat(), 1, 1) );
    gauge->setTexture( "heat/"+utils::to_s( static_cast<int>(player->heat() * 12.0) ) );

    if( !player->is_overheat() ) {
        out_flag = false;
        if( player->heat() < 0.5 ) {
            gauge->set<Green>(255);
            gauge->set<Red>( player->heat()*2*255 );
        }
//        else {
//            gauge->set<Green>( 255 - (player->heat()-0.5)*2*255 );
//            gauge->set<Red>(255);
//        }
    }
    else if( !out_flag ) {
        out_flag = true;
        gauge->tween<SineCirc, ColorDiffuseVec3>(vec3(255,255,255), player->overheat_downtime()/4, 3);
    }
}

void Demo::update_ui(){
    int new_garbage_1p_ = map0_->garbage_left() + map1_->current_sum_of_attack();
    int new_garbage_2p_ = map1_->garbage_left() + map0_->current_sum_of_attack();
// 2012.05 substituted by attack effect
//    ui_layout_->getSpriteText("gar1p").showNumber(new_garbage_1p_, 0);
//    ui_layout_->getSpriteText("gar2p").showNumber(new_garbage_2p_, 0);
    ui_layout_->getSpriteText("scr1p").showNumber(map0_->score(), 5);
    ui_layout_->getSpriteText("scr2p").showNumber(map1_->score(), 5);

    //2011.03.25 weapon temporarily removed

    //ui_layout_->getSpriteText("wep1p1").showNumber(player0_->weapon(0)->ammo(), 2);
    //ui_layout_->getSpriteText("wep1p2").showNumber(player0_->weapon(1)->ammo(), 2);
    //ui_layout_->getSpriteText("wep1p3").showNumber(player0_->weapon(2)->ammo(), 2);
    //ui_layout_->getSpriteText("wep2p1").showNumber(player1_->weapon(0)->ammo(), 2);
    //ui_layout_->getSpriteText("wep2p2").showNumber(player1_->weapon(1)->ammo(), 2);
    //ui_layout_->getSpriteText("wep2p3").showNumber(player1_->weapon(2)->ammo(), 2);

    //for( int i = 0; i <= 2; ++i ) { //note: not flexible, only for test.
    //    if( i == player0_->wepid() )
    //        ui_layout_->getSpriteText("wep1p"+to_s(i+1)).set<Scale>(vec3(2,2,2));
    //    else ui_layout_->getSpriteText("wep1p"+to_s(i+1)).set<Scale>(vec3(1,1,1));
    //
    //    if( i == player1_->wepid() )
    //        ui_layout_->getSpriteText("wep2p"+to_s(i+1)).set<Scale>(vec3(2,2,2));
    //    else ui_layout_->getSpriteText("wep2p"+to_s(i+1)).set<Scale>(vec3(1,1,1));
    //}

    if( pview1_->getState() == presenter::PlayerView::HIT &&
        last_garbage_1p_ > new_garbage_1p_ ) stage_->hitGroup(1);
    if( pview2_->getState() == presenter::PlayerView::HIT &&
        last_garbage_2p_ > new_garbage_2p_ ) stage_->hitGroup(2);

    update_heatgauge(player0_, heatgauge1_, gauge1_flag_);
    update_heatgauge(player1_, heatgauge2_, gauge2_flag_);

    last_garbage_1p_ = new_garbage_1p_;
    last_garbage_2p_ = new_garbage_2p_;
}

void Demo::update_ui_by_second(){
    ++sec_;
    if( sec_ > 59 ) ++min_, sec_ = 0;
    std::string sec = to_s(sec_); if( sec.size() < 2 ) sec = "0" + sec;
    std::string min = to_s(min_); if( min.size() < 2 ) min = "0" + min;
    ui_layout_->getSpriteText("time").changeText( min + ":" + sec );
}

void Demo::game_stop()
{
    timer_ui_.reset();
    btn_pause_.reset();
    ctrl::EventDispatcher::i().clear_btn_event();
    ctrl::EventDispatcher::i().clear_obj_event( scene_ );
    audio::Sound::i().stopAll();
    map0_->stop_dropping();
    map1_->stop_dropping();

    ctrl::EventDispatcher::i().get_timer_dispatcher("game")->set_speed(1.0);
    ctrl::EventDispatcher::i().get_timer_dispatcher("ui")->set_speed(1.0);
    ctrl::EventDispatcher::i().get_timer_dispatcher("input")->set_speed(1.0);
    ctrl::EventDispatcher::i().get_timer_dispatcher("global")->set_speed(1.0);

    ctrl::InputMgr::i().getInputByIndex(0)->setControlledByAI(false);
    ctrl::InputMgr::i().getInputByIndex(1)->setControlledByAI(false);
    player0_->stopAllActions();
    player1_->stopAllActions();
}

void Demo::cleanup()
{
    map0_.reset();
    map1_.reset();
    player0_.reset();
    player1_.reset();
}

//note: temp code
void Demo::end(pMap lose_map)
{
    game_stop();

    if( pause_note_text_) pause_note_text_->set<Visible>(false);
    blocker_->tween<Linear, Alpha>(0, 100, 500u).set<Visible>(true);

    win_t_->set<Visible>(true);
    lose_t_->set<Visible>(true);

    vec2 pos1 = vec2(Conf::i().SCREEN_W() /4,   Conf::i().SCREEN_H() /2);
    vec2 pos2 = vec2(Conf::i().SCREEN_W() /4*3, Conf::i().SCREEN_H() /2);
    if( lose_map == map0_ ) {
        lose_t_->set<Pos2D>( pos1 );
        win_t_->set<Pos2D>( pos2 );
        if( num_of_cpu_ == 1 )
            audio::Sound::i().playBuffer("3/3c/lose.wav");
        else
            audio::Sound::i().playBuffer("3/3c/win.wav");
    }
    else {
        lose_t_->set<Pos2D>( pos2 );
        win_t_->set<Pos2D>( pos1 );
        audio::Sound::i().playBuffer("3/3c/win.wav");
    }
    vec3 v0(0,0,0), v1(1,1,1);
    win_t_->setDepth(-450).tween<OElastic, Scale>(v0, v1, 1000u, 0);
    lose_t_->setDepth(-450).tween<OElastic, Scale>(v0, v1, 1000u, 0);

    end_text_->set<Visible>(true);
    end_text2_->set<Visible>(true);
    end_text_->set<Pos2D> ( vec2(Conf::i().SCREEN_W() /2, Conf::i().SCREEN_H() /2 + 50) );
    end_text2_->set<Pos2D>( vec2(Conf::i().SCREEN_W() /2, Conf::i().SCREEN_H() /2 + 100) );
    end_text_-> set<Alpha>(0).setDepth(-450).tween<Linear, Alpha>(0, 255, 500u, 0, 0, 1000);
    end_text2_->set<Alpha>(0).setDepth(-450).tween<Linear, Alpha>(0, 255, 500u, 0, 0, 1000);

    ctrl::EventDispatcher::i().get_timer_dispatcher("game")->subscribe(
        bind(&Demo::setup_end_button, this), 1000);
}

void Demo::setup_end_button()
{
    std::tr1::function<void(int, int)> clicka = bind(&Demo::reinit, this);
    std::tr1::function<void(int, int)> clickb = bind(&Demo::end_sequence1, this);
    btn_reinit_ = pDummy(new int);
    BOOST_FOREACH(ctrl::Input const* input, ctrl::InputMgr::i().getInputs()) {
        ctrl::EventDispatcher::i().subscribe_btn_event(
            clicka, btn_reinit_, &input->trig1(), ctrl::BTN_PRESS);
        ctrl::EventDispatcher::i().subscribe_btn_event(
            clickb, btn_reinit_, &input->trig2(), ctrl::BTN_PRESS);
    }
}

void Demo::end_sequence1()
{
    audio::Sound::i().playBuffer("4/4c.wav");
    btn_reinit_.reset();

//2012.05 memo: because we are staying in this master presenter, and not going anywhere.
    //stage_->releaseResource(); //release when player isn't going to replay
    //App::i().launchMainMenu();
    std::cout << "game_demo end completed." << std::endl;

    script::Lua::call(L_, "mainmenu");
    leaving_effect();
}

void Demo::pause_quit()
{
    ctrl::EventDispatcher::i().get_timer_dispatcher("game")->start();
    //audio::Sound::i().pauseAll(false);
    btn_pause_.reset(); //reset button event subscribed by this handle.

    ctrl::EventDispatcher::i().get_timer_dispatcher("game")->subscribe(
        bind(&Demo::game_stop, this), shared_from_this(), 1); //1 ms

    ctrl::EventDispatcher::i().get_timer_dispatcher("game")->subscribe(
        bind(&Demo::end_sequence1, this), shared_from_this(), 1); //1 ms
}

void Demo::reinit()
{
    using std::tr1::bind;
    audio::Sound::i().playBuffer("4/4b.wav");
    btn_reinit_.reset();

    init_(num_of_cpu_, c1p_, c2p_, sconf_, true);
//2012.05 memo: because we are staying in this master presenter, and not going anywhere.
//    ctrl::EventDispatcher::i().get_timer_dispatcher("global")->subscribe(
//        bind(&App::launchDemo, &App::i()), 500);
    std::cout << "game_demo re-initialized." << std::endl;
}

static void do_nothing(int, int){};

void Demo::pause(ctrl::Input const* controller)
{
    if( btn_pause_ ) return; //it's already paused, don't do anything if this is called again.

    if( pause_note_text_) pause_note_text_->set<Visible>(false);

    pause_text_->set<Visible>(true);
    pause_text2_->set<Visible>(true);
    pause_t_->set<Visible>(true);
    blocker_->set<Alpha>(100).set<Visible>(true);

    ctrl::EventDispatcher::i().get_timer_dispatcher("game")->stop();
    audio::Sound::i().pauseAll(true);
    scene_->allowPicking(false);

    std::tr1::function<void(int, int)> clicka = bind(&Demo::pause_quit, this);
    std::tr1::function<void(int, int)> clickb = bind(&Demo::resume, this, controller);

    btn_pause_ = pDummy(new int);

    BOOST_FOREACH(ctrl::Input const* input, ctrl::InputMgr::i().getInputs()) {
        ctrl::EventDispatcher::i().subscribe_btn_event(
            do_nothing, shared_from_this(), &input->trig1(), ctrl::BTN_PRESS); //assign null
        ctrl::EventDispatcher::i().subscribe_btn_event(
            do_nothing, shared_from_this(), &input->trig2(), ctrl::BTN_PRESS); //assign null
        ctrl::EventDispatcher::i().subscribe_btn_event(
            do_nothing, shared_from_this(), &input->pause(), ctrl::BTN_PRESS); //assign null
    }

    ctrl::EventDispatcher::i().subscribe_btn_event(
        clicka, btn_pause_, &controller->trig1(), ctrl::BTN_PRESS);
    ctrl::EventDispatcher::i().subscribe_btn_event(
        clickb, btn_pause_, &controller->trig2(), ctrl::BTN_PRESS);
    ctrl::EventDispatcher::i().subscribe_btn_event(
        clickb, btn_pause_, &controller->pause(), ctrl::BTN_PRESS);
}

void Demo::resume(ctrl::Input const* controller)
{
    if( !btn_pause_ ) return; //if it's not paused at all, don't do anything

    pause_text_->set<Visible>(false);
    pause_text2_->set<Visible>(false);
    pause_t_->set<Visible>(false);
    blocker_->set<Visible>(false);

    ctrl::EventDispatcher::i().get_timer_dispatcher("game")->start();
    audio::Sound::i().pauseAll(false);
    scene_->allowPicking(true);

    btn_pause_.reset(); //reset button event subscribed by this handle.

    BOOST_FOREACH(ctrl::Input const* input, ctrl::InputMgr::i().getInputs()) {
        ctrl::EventDispatcher::i().subscribe_btn_event(
            bind(&Demo::pause, this, input), shared_from_this(), &input->pause(), ctrl::BTN_PRESS);
        input->player()->subscribe_player_specific_interactions();
    }
}

void Demo::timed_pause(std::time_t const& t)
{
    ctrl::EventDispatcher::i().get_timer_dispatcher("game")->stop();
    audio::Sound::i().pauseAll(true);
    btn_pause_ = pDummy(new int);
    scene_->allowPicking(false);

    //these probably should not belong here
    blocker_->set<Alpha>(100).set<Visible>(true);
    ppl1_special_img_->set<Visible>(true);

    data::AnimatorParam<OSine, Pos2D>  mov1;
    data::AnimatorParam<Linear, Pos2D> mov2;
    data::AnimatorParam<ISine, Pos2D>  mov3;

    mov1.start( vec2(-300, Conf::i().SCREEN_H() / 2) ).end( vec2( 300, Conf::i().SCREEN_H() / 2) ).duration(200);
    mov2.start( vec2( 300, Conf::i().SCREEN_H() / 2) ).end( vec2( 900, Conf::i().SCREEN_H() / 2) ).duration(550);
    mov3.start( vec2( 900, Conf::i().SCREEN_H() / 2) ).end( vec2(1600, Conf::i().SCREEN_H() / 2) ).duration(200);

    ppl1_special_img_->queue(mov1).queue(mov2).tween(mov3);
    //---

    BOOST_FOREACH(ctrl::Input const* input, ctrl::InputMgr::i().getInputs()) {
        ctrl::EventDispatcher::i().subscribe_btn_event(
            do_nothing, shared_from_this(), &input->trig1(), ctrl::BTN_PRESS); //assign null
        ctrl::EventDispatcher::i().subscribe_btn_event(
            do_nothing, shared_from_this(), &input->trig2(), ctrl::BTN_PRESS); //assign null
        ctrl::EventDispatcher::i().subscribe_btn_event(
            do_nothing, shared_from_this(), &input->pause(), ctrl::BTN_PRESS); //assign null
    }

    ctrl::EventDispatcher::i().get_timer_dispatcher("global")->subscribe(
        bind(&Demo::resume2, this), shared_from_this(), t);
}

void Demo::resume2()
{
    if( !btn_pause_ ) return; //if it's not paused at all, don't do anything

    blocker_->set<Visible>(false);

    ctrl::EventDispatcher::i().get_timer_dispatcher("game")->start();
    audio::Sound::i().pauseAll(false);
    scene_->allowPicking(true);

    btn_pause_.reset(); //reset button event subscribed by this handle.

    BOOST_FOREACH(ctrl::Input const* input, ctrl::InputMgr::i().getInputs()) {
        ctrl::EventDispatcher::i().subscribe_btn_event(
            bind(&Demo::pause, this, input), shared_from_this(), &input->pause(), ctrl::BTN_PRESS);
        input->player()->subscribe_player_specific_interactions();
    }

    ctrl::EventDispatcher::i().get_timer_dispatcher("game")->subscribe(
        bind(&Demo::ppl1_special_attacked, this, false), shared_from_this(), 1000);
}

void Demo::music_state(bool f) {
    music_state_ = f;
}

//temp: hack
bool predicate_column_full_and_has_enough_garbage(pMap const& m0, pMap const& m1)
{
    int map_width = m0->map_setting()->width();              // this should be the same for both map
    int cube_max  = m0->map_setting()->height() * map_width; // this should be the same for both map
    return ( ( m0->column_full_num() + m1->column_full_num() > 4 ) &&
             ( ( m0->column_full_num() > 2 && m0->garbage_count() > map_width ) ||
               ( m1->column_full_num() > 2 && m1->garbage_count() > map_width ) ) )
            //column full above. but you should at the same time have at least *some* garbage to be emergent
               ||
            //enough garbage below. means YOU ARE GOING DOWN... many garbages on the way!!!
           ( m0->grounded_cube_count() + m0->garbage_left() > cube_max ||
             m1->grounded_cube_count() + m1->garbage_left() > cube_max );
}

void Demo::cycle()
{
    clock_t t0 = 0x0fffffff, t1 = 0, t2 = 0, t3 = 0, t4 = 0, t5 = 0, t6 = 0, t7 = 0;
    if( player0_ ) { //it's just some condition that the game is initialized, because we firstly initialized player0_
        t0 = clock();
        pview1_->cycle();
        pview2_->cycle();
        update_ui();
        t1 = clock();
        map0_->cycle();
        map1_->cycle();
        t2 = clock();

        // temp: hack, just for test
        if( predicate_column_full_and_has_enough_garbage(map0_, map1_) ) {
            if( timer_music_state_ ) {
                printf("Demo: nope.. we are very dangerous again.\n");
                timer_music_state_.reset();
            }
            music_state(true);
        }
        else {
            if( music_state_ ) {
                if( map0_->warning_level() + map1_->warning_level() == 0 ) {
                    if( !timer_music_state_ ) {
                        printf("Demo: countdown 6.5 secs to music_state = false\n");
                        timer_music_state_ = pDummy(new int);
                        ctrl::EventDispatcher::i().get_timer_dispatcher("game")->subscribe(
                            std::tr1::bind(&Demo::music_state, this, false), timer_music_state_, 6500);
                        //this means you have to be in no-danger situation for 3 seconds to leave emergency music
                    }
                } else {
                    if( timer_music_state_ ) {
                        printf("Demo: nope.. it's still pretty dangerous.\n");
                        timer_music_state_.reset();
                    }
                }
            }
        }

        // temp: hack, just for test
        if( music_state_ == true && music_state_old_ == false ) {
            printf("Demo: true -> music_state\n");
            stage_->playFastBGM(400);
        }
        if( music_state_ == false && music_state_old_ == true ) {
            printf("Demo: false -> music_state\n");
            stage_->playBGM(500);
        }
        t3 = clock();

        if( !btn_reinit_ && !btn_pause_ ) { //2011.04.09 quick fix: if these indicator is alive, stop AI's possible inputs
            player0_->cycle();
            player1_->cycle();
        }

        t4 = clock();
//        // temp: hack, just for test (cut-in)
//        if( !ppl1_special_attacked_ && map1_->garbage_left() > 15 ) {
//            timed_pause(1000);
//            ppl1_special_attacked_ = true;
//        }
    }


    stage_->cycle();
    t5 = clock();
    scene_->redraw();
    t6 = clock();
    ui_scene_->redraw();

    t7 = clock();
    if( t7 - t0 > 14 ) {
        //printf(" -- Demo::profiler: %ld %ld %ld %ld %ld\n", t1-t0, t2-t1, t3-t2, t4-t3, t5-t4);
        printf(" -- Demo: ui1(%ld) maps(%ld) music(%ld) player(%ld) stage(%ld) scene(%ld) ui2(%ld)\n", t1-t0, t2-t1, t3-t2, t4-t3, t5-t4, t6-t5, t7-t6);
    }

    // temp: hack, just for test
    music_state_old_ = music_state_;
}

