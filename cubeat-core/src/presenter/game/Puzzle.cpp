
#include "presenter/game/Puzzle.hpp"
#include "view/Scene.hpp"
#include "view/AnimatedSprite.hpp"
#include "view/SpriteText.hpp"
#include "view/Menu.hpp"
#include "Accessors.hpp"
#include "EasingEquations.hpp"

#include "presenter/Stage.hpp"
#include "presenter/PlayerView.hpp"
#include "presenter/Map.hpp"
#include "presenter/cube/ViewSprite.hpp"

#include "EventDispatcher.hpp"
#include "Input.hpp"
#include "Player.hpp"
#include "Weapon.hpp"
#include "audio/Sound.hpp"
#include "Conf.hpp"
#include "App.hpp"

#include "utils/Random.hpp"
#include "utils/dictionary.hpp"
#include "utils/MapLoader.hpp"
#include "utils/to_s.hpp"
#include "utils/Logger.hpp"
#include <boost/foreach.hpp>

using namespace psc;
using namespace presenter;
using namespace game;
using namespace easing;
using namespace accessor;
using utils::to_s;
using namespace std::tr1::placeholders;

Puzzle::Puzzle()
{
}

Puzzle::~Puzzle()
{
    std::cout << "Puzzle Game destructing ..." << std::endl;
    std::cout << " player0 use count: " << player0_.use_count() << std::endl;
}

pPuzzle Puzzle::init(std::string const& c1p, std::string const& sc, int puzzle_level)
{
    using std::tr1::bind;
    //App::i().setLoading(1);
    scene_ = psc::view::Scene::create("random Puzzle game");
    //scene_->setTo2DView().enableGlobalHittingEvent(); //2011.03.28 weapon temporary removal
    scene_->setTo2DView();

    c1p_ = c1p; sconf_ = sc; puzzle_level_ = puzzle_level;

    data::pViewSetting s0, s1;

    s0 = data::ViewSetting::create(64);   //must use config
    s0->x_offset(159).y_offset(684);
    s1 = data::ViewSetting::create(64);   //must use config
    s1->x_offset(740).y_offset(684);

    ///THIS IS IMPORTANT, ALL PLAYERS MUST BE DEFINED FIRST.
    ctrl::Input* input = ctrl::InputMgr::i().getInputByIndex(0);
    player0_ = ctrl::Player::create(input, 0, false);
    player0_->push_ally(0).player_hit_event(bind(&Puzzle::puzzle_started, this));

    // setup map0
    map0_ = utils::MapLoader::generate( puzzle_level );
    map0_->set_view_master( presenter::cube::ViewSpriteMaster::create(scene_, s0, player0_) );

    // setup map1
    data::pMapSetting set1 = data::MapSetting::create();
    set1->starting_line(0).dropping_creatable(false);
    map1_ = presenter::Map::create(set1);
    map1_->set_view_master( presenter::cube::ViewSpriteMaster::create(scene_, s1) );

    ///NEW: MAKE PLAYER KNOWS ABOUT MAP
    std::vector< presenter::wpMap > map_list;
    map_list.push_back(map0_);
    player0_->setMapList( map_list );

    // setup garbage land
    map0_->push_garbage_land(map1_);
    map1_->push_garbage_land(map0_);
    //map0_->set_endgame(bind(&Puzzle::end, this, _1)); no endgame right now.. testing

    // setup stage & ui & player's view objects:
    stage_ = presenter::Stage::create( sc.size() ? sc : "stage/jungle" );
    setup_ui_by_config( c1p, std::string(), "ui/in_game_2p_layout" );

    min_ = 0, sec_ = 0 ,last_garbage_1p_ = 0, last_garbage_2p_ = 0;
    win_ = false, puzzle_started_ = false, end_ = false;

    //start music
    stage_->playBGM();

    //note: bad area
    timer_ui_   = pDummy(new int);
    //note: end of bad area

    ctrl::EventDispatcher::i().subscribe_timer(
        bind(&Puzzle::update_ui_by_second, this), timer_ui_, 1000, -1);
    ctrl::EventDispatcher::i().subscribe_timer(
        bind(&App::setLoading, &App::i(), 100), 100); //stupid and must?

    ctrl::EventDispatcher::i().subscribe_btn_event(
        bind(&Puzzle::pause, this), shared_from_this(), &input->pause(), ctrl::BTN_PRESS);

    //temp: puzzle description on the right.
    desc_text_ = view::SpriteText::create("puzzle:\n\nclear all cubes\nin 1 shot", scene_, "Star Jedi", 30, true);
    desc_text_->set<Pos2D>( vec2(s1->x_offset() + s1->cube_size()*3, s1->y_offset()/3) );
    desc_text_->setDepth(-20).setPickable(false);

    return shared_from_this();
}

void Puzzle::setup_ui_by_config( std::string const& c1p, std::string const& c2p, std::string const& path )
{
    uiconf_ = Conf::i().config_of(path);
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

    vec2 center_pos( uiconf_.I("character_center_x"), uiconf_.I("character_center_y") );
    pview1_ = presenter::PlayerView::create( c1p.size() ? c1p : "config/char/char1.zzml", scene_, center_pos );
    pview1_->setMap( map0_ );

    //2011.04.05 make stage number equal to puzzle level.
    ui_layout_->getSpriteText("stage").changeText( "level" + to_s(puzzle_level_ - 2) ); //first puzzle have 3 chains.

    utils::map_any const& gauge_conf = uiconf_.M("heatgauge");
    vec2 gauge1_pos( gauge_conf.I("x_1p"), gauge_conf.I("y") );
    heatgauge1_ = view::Sprite::create("heatgauge1", scene_, gauge_conf.I("w"), gauge_conf.I("h"), false);
    heatgauge1_->set<Pos2D>( gauge1_pos ).set<ColorDiffuseVec3>( vec3(0,255,0) ).set<Alpha>(128)
                .set<Rotation>(vec3(0, 0, gauge_conf.I("rotation")));

    gauge1_flag_ = false;
}

void Puzzle::update_heatgauge(ctrl::pPlayer player, view::pSprite gauge, bool& out_flag)
{
    gauge->set<Scale>( vec3(player->heat(), 1, 1) );

    if( !player->is_overheat() ) {
        out_flag = false;
        if( player->heat() < 0.5 ) {
            gauge->set<Green>(255);
            gauge->set<Red>( player->heat()*2*255 );
        }
        else {
            gauge->set<Green>( 255 - (player->heat()-0.5)*2*255 );
            gauge->set<Red>(255);
        }
    }
    else if( !out_flag ) {
        out_flag = true;
        gauge->tween<SineCirc, ColorDiffuseVec3>(vec3(255,255,255), player->overheat_downtime()/4, 3);
    }
}

void Puzzle::update_ui(){
    int new_garbage_1p_ = map0_->garbage_left() + map1_->current_sum_of_attack();
    int new_garbage_2p_ = map1_->garbage_left() + map0_->current_sum_of_attack();
    ui_layout_->getSpriteText("gar1p").showNumber(new_garbage_1p_, 0);
    ui_layout_->getSpriteText("gar2p").showNumber(new_garbage_2p_, 0);
    ui_layout_->getSpriteText("scr1p").showNumber(map0_->score(), 5);
    ui_layout_->getSpriteText("scr2p").showNumber(map1_->score(), 5);
    //ui_layout_->getSpriteText("wep1p1").showNumber(player0_->weapon(0)->ammo(), 2);
    //ui_layout_->getSpriteText("wep1p2").showNumber(player0_->weapon(1)->ammo(), 2);
    //ui_layout_->getSpriteText("wep1p3").showNumber(player0_->weapon(2)->ammo(), 2);

    last_garbage_1p_ = new_garbage_1p_;
    last_garbage_2p_ = new_garbage_2p_;

    update_heatgauge(player0_, heatgauge1_, gauge1_flag_);
}

void Puzzle::update_ui_by_second(){
    ++sec_;
    if( sec_ > 59 ) ++min_, sec_ = 0;
    std::string sec = to_s(sec_); if( sec.size() < 2 ) sec = "0" + sec;
    std::string min = to_s(min_); if( min.size() < 2 ) min = "0" + min;
    ui_layout_->getSpriteText("time").changeText( min + ":" + sec );
}

void Puzzle::cleanup()
{
    timer_ui_.reset();
    btn_pause_.reset();
    ctrl::EventDispatcher::i().clear_btn_event();
    ctrl::EventDispatcher::i().clear_obj_event( scene_ );
    audio::Sound::i().stopAll();
    map0_->stop_dropping();
    map1_->stop_dropping();

    player0_->stopAllActions();
}

//note: temp code
void Puzzle::end(pMap lose_map)
{
    cleanup();
    end_ = true;

    if( !blocker_ ) {
        blocker_ = view::Sprite::create("blocker", scene_, Conf::i().SCREEN_W() ,350, true);
        blocker_->set<Pos2D>( vec2(Conf::i().SCREEN_W() /2, Conf::i().SCREEN_H() /2) );
        blocker_->setDepth(-40).set<GradientDiffuse>(0).setPickable(false);
    }
    blocker_->tween<Linear, Alpha>(0, 100, 500u).set<Visible>(true);

    audio::Sound::i().playBuffer( win_ ? "3/3c/win.wav" : "3/3c/lose.wav" );

    win_t_  = view::Sprite::create( win_ ? "win" : "lose" , scene_, 384, 192, true);
    //lose_t_ = view::Sprite::create("lose", scene_, 384, 192, true);

    vec2 pos = vec2(Conf::i().SCREEN_W() / 2, Conf::i().SCREEN_H() / 2 - 50);
    win_t_->set<Pos2D>( pos );

    vec3 v0(0,0,0), v1(1,1,1);
    win_t_->setDepth(-450).tween<OElastic, Scale>(v0, v1, 1000u, 0);
    //lose_t_->setDepth(-450).tween<OElastic, Scale>(v0, v1, 1000u, 0);

    end_text_ = view::SpriteText::create( win_ ? "to next level?" : "try again at same level?", scene_, "Star Jedi", 30, true);
    end_text2_= view::SpriteText::create("\nyes: left click\nleave: right click", scene_, "Star Jedi", 30, true);
    end_text_->set<Pos2D> ( vec2(Conf::i().SCREEN_W() / 2, Conf::i().SCREEN_H() / 2 + 50) );
    end_text2_->set<Pos2D>( vec2(Conf::i().SCREEN_W() / 2, Conf::i().SCREEN_H() / 2 + 100) );
    end_text_-> set<Alpha>(0).setDepth(-450).tween<Linear, Alpha>(0, 255, 500u, 0, 0, 1000);
    end_text2_->set<Alpha>(0).setDepth(-450).tween<Linear, Alpha>(0, 255, 500u, 0, 0, 1000);
    using std::tr1::bind;
    ctrl::EventDispatcher::i().subscribe_timer(bind(&Puzzle::setup_end_button, this), 1000);
}

void Puzzle::setup_end_button()
{
    using std::tr1::bind;
    std::tr1::function<void(int, int)> clicka = bind(&Puzzle::reinit, this);
    std::tr1::function<void(int, int)> clickb = bind(&Puzzle::end_sequence1, this);

    btn_reinit_ = pDummy(new int);

    ctrl::Input const* input = ctrl::InputMgr::i().getInputByIndex(0);
    ctrl::EventDispatcher::i().subscribe_btn_event(
        clicka, btn_reinit_, &input->trig1(), ctrl::BTN_PRESS);
    ctrl::EventDispatcher::i().subscribe_btn_event(
        clickb, btn_reinit_, &input->trig2(), ctrl::BTN_PRESS);
}

void Puzzle::end_sequence1()
{
    audio::Sound::i().playBuffer("4/4c.wav");
    btn_reinit_.reset();
    stage_->releaseResource(); //release when player isn't going to replay
    App::i().launchMainMenu();
    std::cout << "game_puzzle end call finished." << std::endl;
}

void Puzzle::pause_quit()
{
    App::i().resume();
    audio::Sound::i().pauseAll(false);
    btn_pause_.reset(); //reset button event subscribed by this handle.
    ctrl::EventDispatcher::i().subscribe_timer(
        bind(&Puzzle::cleanup, this), shared_from_this(), 1); //1 ms
        //because we call this here.. it's gonna cleanup a lot of things.
        //it's better we delay this call.
    ctrl::EventDispatcher::i().subscribe_timer(
        bind(&Puzzle::end_sequence1, this), shared_from_this(), 100); //100 ms
}

void Puzzle::reinit()
{
    using std::tr1::bind;
    int new_puzzle_lv = win_ ? puzzle_level_+1 : puzzle_level_;
    //if( new_puzzle_lv > 8 ) new_puzzle_lv = 8;
    if( new_puzzle_lv > 19 ) new_puzzle_lv = 19;
    else if( new_puzzle_lv < 3 ) new_puzzle_lv = 3;
    audio::Sound::i().playBuffer("4/4b.wav");
    btn_reinit_.reset();
    ctrl::EventDispatcher::i().subscribe_timer(
        bind(&App::launchPuzzle, &App::i(), c1p_, sconf_, new_puzzle_lv), 500);

    std::cout << "game_puzzle end call finished." << std::endl;
}

void Puzzle::pause()
{
    if( btn_pause_ ) return; //it's already paused, don't do anything if this is called again.

    if( !pause_text_ || !pause_text2_ ) {
        pause_text_ = view::SpriteText::create("back to menu?", scene_, "Star Jedi", 30, true);
        pause_text_->set<Pos2D>( vec2(Conf::i().SCREEN_W() /2, Conf::i().SCREEN_H() /2 + 60) );
        pause_text_->setDepth(-450).setPickable(false);
        pause_text2_ = view::SpriteText::create("\nyes: left click\nno: right click", scene_, "Star Jedi", 30, true);
        pause_text2_->set<Pos2D>( vec2(Conf::i().SCREEN_W() /2, Conf::i().SCREEN_H() /2 + 100) );
        pause_text2_->setDepth(-450).setPickable(false);
    }
    pause_text_->set<Visible>(true);
    pause_text2_->set<Visible>(true);

    if( !pause_t_ ) {
        pause_t_ = view::Sprite::create("pause", scene_, 384, 192, true);
        pause_t_->set<Pos2D>( vec2(Conf::i().SCREEN_W()/2, Conf::i().SCREEN_H()/2 - 50) );
        pause_t_->setDepth(-450).setPickable(false);
    }
    pause_t_->set<Visible>(true);

    if( !blocker_ ) {
        blocker_ = view::Sprite::create("blocker", scene_, Conf::i().SCREEN_W() ,350, true);
        blocker_->set<Pos2D>( vec2(Conf::i().SCREEN_W() /2, Conf::i().SCREEN_H() /2) );
        blocker_->setDepth(-40).set<GradientDiffuse>(0).setPickable(false);
    }
    blocker_->set<Alpha>(100).set<Visible>(true);

    App::i().pause();
    audio::Sound::i().pauseAll(true);
    scene_->allowPicking(false);

    ctrl::Input const* controller = ctrl::InputMgr::i().getInputByIndex(0);

    std::tr1::function<void(int, int)> clicka = bind(&Puzzle::pause_quit, this);
    std::tr1::function<void(int, int)> clickb = bind(&Puzzle::resume, this);

    btn_pause_ = pDummy(new int);

    ctrl::EventDispatcher::i().subscribe_btn_event(
        clicka, btn_pause_, &controller->trig1(), ctrl::BTN_PRESS);
    ctrl::EventDispatcher::i().subscribe_btn_event(
        clickb, btn_pause_, &controller->trig2(), ctrl::BTN_PRESS);
    ctrl::EventDispatcher::i().subscribe_btn_event(
        clickb, btn_pause_, &controller->pause(), ctrl::BTN_PRESS);
}

void Puzzle::resume()
{
    if( !btn_pause_ ) return; //if it's not paused at all, don't do anything

    pause_text_->set<Visible>(false);
    pause_text2_->set<Visible>(false);
    pause_t_->set<Visible>(false);
    blocker_->set<Visible>(false);

    App::i().resume();
    audio::Sound::i().pauseAll(false);
    scene_->allowPicking(true);

    btn_pause_.reset(); //reset button event subscribed by this handle.

    ctrl::Input const* input = ctrl::InputMgr::i().getInputByIndex(0);
    ctrl::EventDispatcher::i().subscribe_btn_event(
        bind(&Puzzle::pause, this), shared_from_this(), &input->pause(), ctrl::BTN_PRESS);

    input->player()->subscribe_player_specific_interactions(true);
}

void Puzzle::puzzle_started() //This is a callback for obj_event, so you cannot clear obj_event inside this.
{
    puzzle_started_ = true;
    ctrl::EventDispatcher::i().subscribe_timer(
        bind(&ctrl::EventDispatcher::clear_obj_event, &ctrl::EventDispatcher::i(), scene_), 1);
}

void Puzzle::cycle()
{
    pview1_->cycle();
    update_ui();
    stage_->cycle();
    scene_->redraw();
    map0_->redraw().cycle();
    map1_->redraw().cycle();

    //note: bad way........ but have no time.
    if( !end_ ) {
        if( puzzle_started_ ) {
            if( map0_->all_empty() ) {
                win_ = true;
                end(map0_);
            }
            else if( map0_->all_waiting() ) {
                win_ = false;
                end(map0_);
            }
        }
    }
}
