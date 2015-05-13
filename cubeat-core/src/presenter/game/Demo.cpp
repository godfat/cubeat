
#include "presenter/game/Demo.hpp"

#include "data/ViewSetting.hpp"

#include "view/Scene.hpp"
#include "view/AnimatedSprite.hpp"
#include "view/SpriteText.hpp"
#include "view/Menu.hpp"
#include "Accessors.hpp"
#include "EasingEquations.hpp"

#include "presenter/Stage2.hpp"
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
    :c1p_("char/char1_new"), c2p_("char/char2_new"), sconf_("stage/jungle1"), game_mode_(GM_PVC),
     submode_(0), ai_level_(3), ai_logging_times_(0), ai_logging_rounds_(0), some_ui_inited_(false),
     is_countdown_(false), L_(0)
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

    // setup stage & ui & player's view objects:
//    stage_ = presenter::Stage2::create( sconf_.size() ? sconf_ : "stage/jungle" );

    L_ = luaL_newstate();
    luaL_openlibs(L_);
    script::Lua::run_script(L_, Conf::i().script_path("demo/demo.lua").c_str());
    script::Lua::call(L_, "init", static_cast<void*>(this));

    /*** Ok, we handover the control to script from here on. ***/

    ctrl::EventDispatcher::i().get_timer_dispatcher("global")->subscribe(
        bind(loading_complete_, 100), 100);

    audio::Sound::i().playBGM_AB("title_a.ogg", "title_b.ogg");

    load_stats_and_achievements_into_memory();

    return shared_from_this();
}

void Demo::init_(int const& game_mode, std::string const& c1p, std::string const& c2p, std::string const& scene_name, bool const& inplace, int const& submode)
{
    btn_reinit_.reset();

    game_state_ = GS_INIT;
    game_mode_ = game_mode;
    submode_ = submode;
    c1p_ = c1p;
    c2p_ = c2p;
    sconf_ = scene_name;
    music_state_ = false;
    music_state_old_ = false;
    uiconf_ = Conf::i().config_of("ui/demo_layout");

    // remember the random seed.
    int seed = 0;
//    if( App::i().getReplay().read_file("tmp/replay") ) {
//        seed = App::i().getReplay().seed();
//    } else {
        seed = std::time(0)^std::clock();
        App::i().getReplay().seed(seed);
//    }
    App::i().getReplay().set_timer_dispatcher( ctrl::EventDispatcher::i().get_timer_dispatcher("game") );
    utils::Random3::i().seed(seed);

// WTF MEMO 2012.9 failed to adjust for balance
    passive_conf0_ = Conf::i().config_of(c1p).M("passive_mod");
    passive_conf1_ = Conf::i().config_of(c2p).M("passive_mod");

    //stop timer for now because the initial loading gonna be some time.
    ctrl::EventDispatcher::i().get_timer_dispatcher("game")->set_time(0).stop();
    ctrl::EventDispatcher::i().get_timer_dispatcher("ui")->stop();

    double speed = gameplay_.F("speed");
    ctrl::EventDispatcher::i().get_timer_dispatcher("game")->set_speed(speed);
    ctrl::EventDispatcher::i().get_timer_dispatcher("ui")->set_speed(speed);
    ctrl::EventDispatcher::i().get_timer_dispatcher("input")->set_speed(speed);
    ctrl::EventDispatcher::i().get_timer_dispatcher("global")->set_speed(speed);

    scene_->allowPicking(false);

    //load scene again first
    /// NOTE update 2014.08: Now the life time of stage actually covers from talk_start->game->talk_end,
    ///                      and not just in-game. So it's no longer OK to load stage here when submode_ indicates
    ///                      it's not normal PVP/PVC game.
    if( submode_ == 0 ) {
        if( !inplace && stage_ ) {
            stage_->releaseResource();
            stage_.reset();
        }
        stage_ = presenter::Stage2::create( sconf_.size() ? sconf_ : "stage/jungle" );
    }

    data::pViewSetting s0, s1;

    s0 = data::ViewSetting::create( uiconf_.M("mapview0") );
    s1 = data::ViewSetting::create( uiconf_.M("mapview1") );

    ///THIS IS IMPORTANT, ALL PLAYERS MUST BE DEFINED FIRST.
    ctrl::Input* input0 = ctrl::InputMgr::i().getInputByIndex(0);
    ctrl::Input* input1 = ctrl::InputMgr::i().getInputByIndex(1);

    if( game_mode_ == GM_PVC || game_mode_ == GM_TUT ) {
        input1->setControlledByAI(true);
        player0_ = ctrl::Player::create(input0, 0);
        player1_ = ctrl::AIPlayer::create(input1, 1, game_mode_ == GM_TUT ? -1 : ai_level_);
    }
    else if( game_mode_ == GM_CVC || game_mode_ == GM_LOG ) {
        input0->setControlledByAI(true);
        input1->setControlledByAI(true);
        player0_ = ctrl::AIPlayer::create(input0, 0, ai_level_);
        player1_ = ctrl::AIPlayer::create(input1, 1, ai_level_);
        if( game_mode_ == GM_LOG ) { // AI LOGGING's timer speed will overwrite the gameplay_ one
            double speed = Conf::i().config_of("ai_logging_config").F("speed");
            ctrl::EventDispatcher::i().get_timer_dispatcher("game")->set_speed(speed);
            ctrl::EventDispatcher::i().get_timer_dispatcher("ui")->set_speed(speed);
            ctrl::EventDispatcher::i().get_timer_dispatcher("input")->set_speed(speed);
            ctrl::EventDispatcher::i().get_timer_dispatcher("global")->set_speed(speed);
        }
    } else { // Make PVP a default
        player0_ = ctrl::Player::create(input0, 0);
        player1_ = ctrl::Player::create(input1, 1);
    }
    player0_->push_ally(0).push_enemy(1);
    player1_->push_ally(1).push_enemy(0);

    // setup player settings
    player0_->set_config( !passive_conf0_.empty() ? passive_conf0_.M("weapon") : gameplay_.M("player1").M("weapon") );
    player1_->set_config( !passive_conf1_.empty() ? passive_conf1_.M("weapon") : gameplay_.M("player2").M("weapon") );

    // setup map settings
    data::pMapSetting set0 = data::MapSetting::create( gameplay_.M("player1") );
    data::pMapSetting set1 = data::MapSetting::create( gameplay_.M("player2") );

    // update map settings with player passive modification:
// WTF MEMO 2012.9 failed to adjust for balance
// WTF MEMO 2014.3 try to use it again:
    set0->apply_player_passive_mods( passive_conf0_ );
    set1->apply_player_passive_mods( passive_conf1_ );

    // some interacting settings have to be resolved first:
// WTF MEMO 2012.9 failed to adjust for balance
//    set0->damage_factor( set0->damage_factor() * set1->negate_damage_factor() );
//    set1->damage_factor( set1->damage_factor() * set0->negate_damage_factor() );

    if( game_mode_ == GM_PVC && ai_level_ == 0 ) {
        set0->cube_dying_duration(900).sink_speed(80).sink_speed_limit(240);
        set1->cube_dying_duration(900).sink_speed(80).sink_speed_limit(240);
    }

    if( game_mode_ == GM_TUT ) {
        map0_ = utils::MapLoader::load( "tutorial" );
        map1_ = utils::MapLoader::load( "tutorial" );
    } else {
        /// WTF Wait, what does player do in creating Maps? using MapLoader didn't seem to break anything.
        if( gameplay_.exist("shortcut") ) map0_ = utils::MapLoader::load( gameplay_.S("shortcut") );
        else map0_ = presenter::Map::create(set0, player0_);

        if( gameplay_.exist("shortcut2") ) map1_ = utils::MapLoader::load( gameplay_.S("shortcut2") );
        else map1_ = presenter::Map::create(set1, player1_);
    }

    map0_->set_view_master( presenter::cube::ViewSpriteMaster::create(scene_, s0, player0_) );
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

    /// Demo hacking for presentation purpose:
    if( gameplay_.exist("score1") ) map0_->score( gameplay_.I("score1") );
    if( gameplay_.exist("attack2") ) map0_->set_garbage_amount(gameplay_.I("attack2"));
    if( gameplay_.exist("attack1") ) map0_->new_garbage_event()(std::vector< std::tr1::tuple<int, int, int> >(), gameplay_.I("attack1"));
    if( gameplay_.exist("score2") ) map1_->score( gameplay_.I("score2") );
    if( gameplay_.exist("attack1") ) map1_->set_garbage_amount(gameplay_.I("attack1"));
    if( gameplay_.exist("attack2") ) map1_->new_garbage_event()(std::vector< std::tr1::tuple<int, int, int> >(), gameplay_.I("attack2"));
    if( gameplay_.exist("time") ) {
        int faketime = gameplay_.I("time");
        min_ = faketime / 60; sec_ = faketime % 60;
        std::string sec = to_s(sec_); if( sec.size() < 2 ) sec = "0" + sec;
        std::string min = to_s(min_); if( min.size() < 2 ) min = "0" + min;
        ui_layout_->getSpriteText("time").changeText( min + ":" + sec );
    }
    /// ////////////////////////////

    if( game_mode_ == GM_TUT ) {
        submode_ = 50; /// WTF TEMP
        script::Lua::call(L_, "init_override", inplace, submode_);
    }

    ctrl::EventDispatcher::i().get_timer_dispatcher("game")->start();
    ctrl::EventDispatcher::i().get_timer_dispatcher("ui")->start();

    //start music
    audio::Sound::i().stopAll(); //stop old

    if( stage_ ) {
        stage_->playBGM();
    } else {
        printf("Demo (error): Stage isn't initialized, can't playBGM().\n");
    }

    //ready_go(4);
    starting_effect(inplace);
}

void Demo::init_single(int const& submode, int const& level, std::string const& c1p, std::string const& scene_name, bool const& inplace)
{
    btn_reinit_.reset();

    game_state_ = GS_INIT;
    game_mode_ = GM_SINGLE; // assign this value to Puzzle Demo for now
    submode_   = submode;
    c1p_ = c1p;
    sconf_ = scene_name;
    music_state_ = false;
    music_state_old_ = false;
    mode_level_ = level;
    uiconf_ = Conf::i().config_of("ui/puzzle_layout");

    //stop timer for now because the initial loading gonna be some time.
    ctrl::EventDispatcher::i().get_timer_dispatcher("game")->set_time(0).stop();
    ctrl::EventDispatcher::i().get_timer_dispatcher("ui")->stop();
    scene_->allowPicking(false);

    //load scene again first
    if( !inplace ) {
        if( stage_ ) {
            stage_->releaseResource();
            stage_.reset();
        }
        stage_ = presenter::Stage2::create( sconf_.size() ? sconf_ : "stage/jungle" );
    }

    data::pViewSetting s0;
    s0 = data::ViewSetting::create( uiconf_.M("mapview0") );

    ///THIS IS IMPORTANT, ALL PLAYERS MUST BE DEFINED FIRST.
    ctrl::Input* input = ctrl::InputMgr::i().getInputByIndex(0);
    player0_ = ctrl::Player::create(input, 0);
    player0_->push_ally(0);

    // setup map
    if( submode_ == 0 ) {
        map0_ = utils::MapLoader::generate( mode_level_ );
    } else {
        data::pMapSetting set0 = data::MapSetting::create( gameplay_.M("player1") );
        map0_ = presenter::Map::create(set0, player0_);
        map0_->lose_event(bind(&Demo::end, this, ref(map0_)));
    }
    map0_->set_view_master( presenter::cube::ViewSpriteMaster::create(scene_, s0, player0_) );
    map0_->stop_dropping(); //make them stop dropping from the very beginning.

    ///NEW: MAKE PLAYER KNOWS ABOUT MAP
    std::vector< presenter::wpMap > map_list;
    map_list.push_back(map0_);
    player0_->setMapList( map_list );

    setup_ui();

    min_ = 0, sec_ = 0 ,last_garbage_1p_ = 0, last_garbage_2p_ = 0;

    puzzle_started_ = false;

    script::Lua::call(L_, "init_override", inplace, submode_);

    //start timer here.
    ctrl::EventDispatcher::i().get_timer_dispatcher("game")->start();
    ctrl::EventDispatcher::i().get_timer_dispatcher("ui")->start();

    //start music
    if( !inplace ) {
        audio::Sound::i().stopAll(); //stop old
        stage_->playBGM();
    }

    starting_effect(inplace);
}

void Demo::init_vs_ppl(std::string const& c1p, std::string const& c2p, std::string const& scene_name)
{
    init_(GM_PVP, c1p, c2p, scene_name);
}

void Demo::init_vs_cpu(std::string const& c1p, std::string const& c2p, std::string const& scene_name, int const& ai_level)
{
    ai_level_ = ai_level; // TEMP WTF MEMO
    init_(GM_PVC, c1p, c2p, scene_name);
}

void Demo::init_story(std::string const& c1p, std::string const& c2p, std::string const& scene_name, int const& ai_level)
{
    ai_level_ = ai_level; // WTF TEMP MEMO AGAIN
    init_(GM_PVC, c1p, c2p, scene_name, false, 99);
}

void Demo::init_tutorial(std::string const& c1p, std::string const& c2p, std::string const& scene_name)
{
    init_(GM_TUT, c1p, c2p, scene_name);
}

void Demo::init_cpudemo(std::string const& c1p, std::string const& c2p, std::string const& scene_name)
{
    init_(GM_CVC, c1p, c2p, scene_name);
}

void Demo::init_ai_logging(std::string const& c1p, std::string const& c2p, std::string const& scene_name)
{
    //ai_logging_times_ = 20;
    ai_logging_conf_   = Conf::i().config_of("ai_logging_config");
    run_next_log();
    //init_(3, c1p, c2p, scene_name);
}

void Demo::init_map_starting_line(int const& map_id, int const& n) {
    if( map_id == 1 && map1_ ) {
        map1_->purge_all();
        map1_->map_setting()->starting_line(n);
        ctrl::EventDispatcher::i().get_timer_dispatcher("global")->subscribe(
            bind(&Map::init_cubes, map1_.get()), shared_from_this(), 2000);
        //map1_->init_cubes();
    } else {
        map0_->purge_all();
        map0_->map_setting()->starting_line(n);
        ctrl::EventDispatcher::i().get_timer_dispatcher("global")->subscribe(
            bind(&Map::init_cubes, map0_.get()), shared_from_this(), 2000);
        //map0_->init_cubes();
    }
}

void Demo::init_map_with_config(int const& map_id, std::string const& filename)
{
    data::pMapSetting new_setting = utils::MapLoader::load_map_setting(filename);
    if( map_id == 1 && map1_ ) {
        map1_-> purge_all();
        map1_-> map_setting()->color_amounts( new_setting->color_amounts() ).
                chain_amounts( new_setting->chain_amounts() ).
                starting_line( new_setting->starting_line() ).
                chaining_duration( new_setting->chaining_duration() ).
                cube_dying_duration( new_setting->cube_dying_duration() ).
                cube_sinking_duration( new_setting->cube_sinking_duration() ).
                cube_dropping_duration( new_setting->cube_dropping_duration() ).
                sink_speed( new_setting->sink_speed() ).
                sink_speed_add( new_setting->sink_speed_add() ).
                sink_speed_limit( new_setting->sink_speed_limit() ).
                drop_speed( new_setting->drop_speed() ).
                warning_gap( new_setting->warning_gap() ).
                max_warning_count( new_setting->max_warning_count() ).
                damage_factor( new_setting->damage_factor() ).
                negate_damage_factor( new_setting->negate_damage_factor() ).
                drop_normal_chance( new_setting->drop_normal_chance() ).
                width( new_setting->width() ).
                height( new_setting->height() ).
                dropping_creatable( new_setting->dropping_creatable() ).
                garbage_dumpable( new_setting->garbage_dumpable() ).
                use_broken_as_garbage( new_setting->use_broken_as_garbage() ).
                preset( new_setting->preset() ).
                preset_data( new_setting->preset_data() ).
                tutorial( new_setting->tutorial() );

        ctrl::EventDispatcher::i().get_timer_dispatcher("global")->subscribe(
            bind(&Map::init_cubes, map1_.get(), utils::MapLoader::load_cube_colors(filename)), shared_from_this(), 1);
        //map1_->init_cubes( utils::MapLoader::load_cube_colors(filename) );

    } else {
        map0_->purge_all();
        map0_-> map_setting()->color_amounts( new_setting->color_amounts() ).
                chain_amounts( new_setting->chain_amounts() ).
                starting_line( new_setting->starting_line() ).
                chaining_duration( new_setting->chaining_duration() ).
                cube_dying_duration( new_setting->cube_dying_duration() ).
                cube_sinking_duration( new_setting->cube_sinking_duration() ).
                cube_dropping_duration( new_setting->cube_dropping_duration() ).
                sink_speed( new_setting->sink_speed() ).
                sink_speed_add( new_setting->sink_speed_add() ).
                sink_speed_limit( new_setting->sink_speed_limit() ).
                drop_speed( new_setting->drop_speed() ).
                warning_gap( new_setting->warning_gap() ).
                max_warning_count( new_setting->max_warning_count() ).
                damage_factor( new_setting->damage_factor() ).
                negate_damage_factor( new_setting->negate_damage_factor() ).
                drop_normal_chance( new_setting->drop_normal_chance() ).
                width( new_setting->width() ).
                height( new_setting->height() ).
                dropping_creatable( new_setting->dropping_creatable() ).
                garbage_dumpable( new_setting->garbage_dumpable() ).
                use_broken_as_garbage( new_setting->use_broken_as_garbage() ).
                preset( new_setting->preset() ).
                preset_data( new_setting->preset_data() ).
                tutorial( new_setting->tutorial() );

        ctrl::EventDispatcher::i().get_timer_dispatcher("global")->subscribe(
            bind(&Map::init_cubes, map0_.get(), utils::MapLoader::load_cube_colors(filename)), shared_from_this(), 1);
        //map0_->init_cubes( utils::MapLoader::load_cube_colors(filename) );
    }
}

void Demo::reset_map_record(int const& map_id) {
    if( map_id == 1 && map1_ ) {
        map1_->reset_record();
    } else {
        map0_->reset_record();
    }
}

void Demo::hide_map_warning(int const& map_id) {
    if( map_id == 1 && map1_ ) {
        map1_->alert_bar_update_event()(0);
    } else {
        map0_->alert_bar_update_event()(0);
    }
}

/// WTF MEMO Here... Another problem. Without map1, I can't call this method at all.
/// Likely cause is the animation events... event though I've null-checked map1_,
/// inside of set_garbage_amount() method, there are other problems....
void Demo::set_map_garbage_amount(int const& map_id, int const& n) {
    if( map_id == 1 && map1_ ) {
        map1_->set_garbage_amount(n);
        map0_->new_garbage_event()(std::vector< std::tr1::tuple<int, int, int> >(), n);
    } else {
        map0_->set_garbage_amount(n);
        if( map1_ )
            map1_->new_garbage_event()(std::vector< std::tr1::tuple<int, int, int> >(), n);
    }
}

void Demo::set_map_dropping(int const& map_id, bool const& flag) {
    if( map_id == 1 && map1_ ) {
        flag ? map1_->start_dropping() : map1_->stop_dropping();
    } else {
        flag ? map0_->start_dropping() : map0_->stop_dropping();
    }
}

void Demo::set_only_one_shot_for_puzzle() {
    player0_->player_hit_event(bind(&Demo::remove_all_game_scene_obj_event, this));
}

void Demo::set_stage_name(std::string const& str) {
    ui_layout_->getSpriteText("stage").changeText(str);
}

void Demo::set_countdown(bool const& flag) {
    is_countdown_ = flag;
}

void Demo::set_time(int const& time) {
    min_ = time / 60;
    sec_ = time % 60;
    update_ui_time();
}

void Demo::set_player_overheat_ui(int const& id, bool const& flag) {
    if( id == 0 && player0_ ) {
        overheat1_->set<Visible>(flag);
    }
    else if( id == 1 && player1_ ) {
        overheat2_->set<Visible>(flag);
    }
}

void Demo::play_sound(std::string const& file) {
    audio::Sound::i().playBuffer(file);
}

void Demo::hide_character_animations() {
    if( pview1_ ) pview1_->getView()->set<Visible>(false);
    if( pview2_ ) pview2_->getView()->set<Visible>(false);
}

int  Demo::get_time() const {
    return min_*60 + sec_;
}

int  Demo::get_map_score(int const& map_id) const {
    return map_id == 1 ?
        map1_->score() :
        map0_->score();
}

int  Demo::get_map_highest_chain(int const& map_id) const {
    return map_id == 1 ?
        map1_->highest_chain() :
        map0_->highest_chain();
}

int  Demo::get_map_garbage_left(int const& map_id) const {
    return map_id == 1 ?
        map1_->garbage_left() :
        map0_->garbage_left();
}

int  Demo::get_map_warning_level(int const& map_id) const {
    return map_id == 1 ?
        map1_->warning_level() :
        map0_->warning_level();
}

int  Demo::get_map_broken_num(int const& map_id) const {
    return map_id == 1 ?
        map1_->garbage_count() :
        map0_->garbage_count();
}

int  Demo::get_map_max_warning_count(int const& map_id) const {
    return map_id == 1 ?
        map1_->model()->ms()->max_warning_count() :
        map0_->model()->ms()->max_warning_count();
}

int const* Demo::get_map_cubes_cleared_data(int const& map_id) const {
    return map_id == 1 ?
        map1_->cubes_cleared_data() :
        map0_->cubes_cleared_data();
}

int const* Demo::get_map_cubes_matched_data(int const& map_id) const {
    return map_id == 1 ?
        map1_->cubes_matched_data() :
        map0_->cubes_matched_data();
}

ctrl::AIPlayer* Demo::get_ai_player() const {
    return player1_ && game_mode_ != GM_PVP ?
           static_cast<ctrl::AIPlayer*>(player1_.get()) :
           0;
}

bool Demo::is_map_all_waiting(int const& map_id) const {
    return map_id == 1 ?
        map1_->all_waiting() :
        map0_->all_waiting();
}

bool Demo::is_map_empty(int const& map_id) const {
    return map_id == 1 ?
        map1_->all_empty() :
        map0_->all_empty();
}

bool Demo::is_puzzle_started() const {
    return puzzle_started_;
}

void Demo::run_next_log()
{
    if( ai_logging_conf_.V("sessions").size() > 0 ) {
        ai_logging_times_ = ai_logging_conf_.I("times");       // how many times is a single recording?

        std::string c1p = ai_logging_conf_.V("sessions").V(0).S(0);
        std::string c2p = ai_logging_conf_.V("sessions").V(0).S(1);
        std::string sc  = ai_logging_conf_.V("sessions").V(0).S(2);
        ai_logging_conf_.V("sessions").erase( ai_logging_conf_.V("sessions").begin() ); //pop head

        utils::map_any log = Conf::i().config_of("tmp/ai_log");
        std::string    record_name = c1p.substr(5, c1p.size()-5) + c2p.substr(5, c2p.size()-5);

        if( !log.exist(record_name) ) {
            log[record_name] = utils::vector_any();
        }
        int index = log.V(record_name).size();
        log.V(record_name).push_back( utils::map_any() );
        log.V(record_name).M(index)["player0"] = 0;
        log.V(record_name).M(index)["player1"] = 0;
        Conf::i().save_config(log, "tmp/ai_log");

        init_(3, c1p, c2p, sc, false);
    }
}

void Demo::endgame(int map_num)
{
    if( game_state_ != GS_STARTED ) return;
    if( map_num == 0 ) {
        end(map0_);
    }
    else if( map_num == 1 ) {
        map1_->warning_level( map1_->model()->ms()->max_warning_count() * 1000 );
        end(map1_);
    }
}

void Demo::load_stage(int stage_id)
{
    if( stage_ ) {
        stage_->releaseResource();
        stage_.reset();
    }
    std::string scene_name_base = "stage/jungle";
    sconf_ = scene_name_base + utils::to_s(stage_id);
    stage_ = presenter::Stage2::create( sconf_ );

    audio::Sound::i().stopAll();
    audio::Sound::i().playBGM_AB("day_a.ogg", "day_b.ogg");
}

view::pScene Demo::get_ui_scene()
{
    return ui_scene_;
}

view::pScene Demo::get_game_scene()
{
    return scene_;
}

void Demo::quit()
{
    App::i().quit();
}

void Demo::leaving_effect(bool const& ending_theme)
{
    heatgauge1_->set<Visible>(false);
    if( game_mode_ != GM_SINGLE ) heatgauge2_->set<Visible>(false);
    hide_upper_layer_ui();
    scene_->tween<ISine, Pos2D>(vec2( Conf::i().SCREEN_W(), - Conf::i().SCREEN_H()/2 ), 1000u);
    script::Lua::call(L_, "slide_in");

    audio::Sound::i().stopAll();

    if( ending_theme ) {
        audio::Sound::i().playBGM("ending.ogg");
    } else {
        audio::Sound::i().playBGM_AB("day_a.ogg", "day_b.ogg");
    }

    ctrl::EventDispatcher::i().get_timer_dispatcher("game")->subscribe(
        bind(&Demo::cleanup, this), shared_from_this(), 1000); //1000 ms
}

void Demo::starting_effect(bool const& inplace)
{
    int count_step = !gameplay_.exist("dont_show_ready") && game_mode_ != GM_TUT ? 1 : 0;
    if( inplace ) {
        ready_go(count_step + 1);
    }
    else {
        std::tr1::function<void()> cb = bind(&Demo::ready_go, this, count_step);
        scene_->tween<OSine, Pos2D>(
            vec2( - Conf::i().SCREEN_W() * 2, - Conf::i().SCREEN_H()/2 ),
            vec2( - Conf::i().SCREEN_W() / 2, - Conf::i().SCREEN_H()/2 ),
            950u, 0, cb);
    }

    script::Lua::call(L_, "slide_out", inplace);
}

//This is currently a mockup, of course we can't use normal fonts as countdown text. image needed.
void Demo::ready_go(int step)
{
    if ( step < 0 ) {

        /// WTF Hack for demo
        if( !gameplay_.exist("dont_show_ready") && game_mode_ != GM_TUT ) {

            ready_go_text_->tween<Linear, Alpha>(0, 500u);

            blocker_->set<Visible>(false);
            blocker_->set<Alpha>(144);
        }

        return;
    }
    else if ( step == 0 ) {

        /// WTF Hack for demo
        if( !gameplay_.exist("dont_show_ready") && game_mode_ != GM_TUT ) {
            audio::Sound::i().playBuffer("go.wav");
            ready_go_text_->changeText("go!");
            ready_go_text_->set<Scale>(vec3(1,1,1));
            ready_go_text_->tween<OElastic, Scale>(vec3(2,2,2), 900u, 0);

            blocker_->tween<Linear, Alpha>(144, 0, 1000u);
        }

        game_start();
    }
    else if ( step <= 1 ) {
        /// WTF Hack for demo
        if( !gameplay_.exist("dont_show_ready") && game_mode_ != GM_TUT ) {
            audio::Sound::i().playBuffer("count.wav");
            //ready_go_text_->showNumber(step);

            ready_go_text_->changeText("ready?");
            ready_go_text_->set<Scale>(vec3(0.8,0.8,0.8));
            ready_go_text_->set<Visible>(true);
            ready_go_text_->tween<OElastic, Scale>(vec3(1.6,1.6,1.6), 900u, 0);
            blocker_->set<Visible>(true);
        }
    }
    ctrl::EventDispatcher::i().get_timer_dispatcher("game")->subscribe(
        std::tr1::bind(&Demo::ready_go, this, step-1), shared_from_this(), 1000);
}

void Demo::game_start()
{
    //note: bad area
    timer_ui_   = pDummy(new int);
    //note: end of bad area

    using std::tr1::bind;
    ctrl::EventDispatcher::i().get_timer_dispatcher("game")->subscribe(
        bind(&Demo::counting_by_second, this), timer_ui_, 1000, -1);

    BOOST_FOREACH(ctrl::Input const* input, ctrl::InputMgr::i().getInputs()) {
        ctrl::EventDispatcher::i().subscribe_btn_event(
            bind(&Demo::pause, this, input), shared_from_this(), &input->pause(), ctrl::BTN_PRESS);
    }

    if( game_mode_ == GM_CVC || game_mode_ == GM_LOG ) {
        blocker_->set<Visible>(true);
        blocker_->set<Pos2D>(vec2(Conf::i().SCREEN_W()/2, Conf::i().SCREEN_H() + 130));
        pause_note_text_->set<Visible>(true);
    }

    // Have to reset timer here again, because the "ready_go" part and scene_ slide effect can't do when timer is stopped.
    ctrl::EventDispatcher::i().get_timer_dispatcher("game")->set_time(0);
    scene_->allowPicking(true);

    /// MEMO: replay recording starts here
    if( game_mode_ != GM_SINGLE ) { /// WTF: Do we want to setup replay for single player modes ???
        /// OK, probably we want... but let's just skip it here for the moment
        App::i().getReplay().toggle_recording_andor_replaying(true, shared_from_this());
    }

    player0_->subscribe_player_specific_interactions();
    player0_->start_heat_timer();
    if( game_mode_ == GM_SINGLE && submode_ != 0 ) {
        map0_->start_dropping();
    }
    else if( game_mode_ == GM_TUT ) {
        player1_->subscribe_player_specific_interactions();
        player1_->start_heat_timer();
    }
    else if( game_mode_ != GM_SINGLE ) {
        map0_->start_dropping();
        player1_->subscribe_player_specific_interactions();
        player1_->start_heat_timer();
        map1_->start_dropping();
    }

    if( App::i().getReplay().is_replaying() ) {
        ctrl::InputMgr::i().getInputByIndex(0)->setControlledByAI(true);
        ctrl::InputMgr::i().getInputByIndex(1)->setControlledByAI(true);
    } else {
        if( ( game_mode_ == GM_PVC || game_mode_ == GM_CVC || game_mode_ == GM_LOG ) )
            player1_->startThinking();
        if( game_mode_ == GM_CVC || game_mode_ == GM_LOG )
            player0_->startThinking();
    }

    update_stats_and_achievements_startgame();

    game_state_ = GS_STARTED;
}

void Demo::setup_ui()
{
    utils::map_any const& base = uiconf_.M("base");
    ui_layout_ = view::Menu::create( "ui_orig", scene_, 0, 0 );

    BOOST_FOREACH(utils::pair_any const& it, base) {
        std::string    const& key  = boost::any_cast<std::string const>(it.first);
        utils::map_any const& attr = boost::any_cast<utils::map_any const>(it.second);
        ui_layout_->
            addSprite(key, 0, attr.I("w"), attr.I("h"), false, attr.S("img"))
           .getSprite(key).set<Pos2D>( vec2(attr.I("x"), attr.I("y")) );
    }

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
    pview1_ = presenter::PlayerView::create( c1p_, scene_, center_pos1 );
    pview1_->setMap( map0_ );
    pview1_->setInput( ctrl::InputMgr::i().getInputByIndex(0) ); //temp: for pview to know input for rumbling wiimote
//    heatgauge1_ = view::Sprite::create("heat/0", ui_scene_, 96, 96, true);
//    heatgauge1_->set<ColorDiffuseVec3>( vec3(0,255,0) ).set<Alpha>(255);
    heatgauge1_ = view::Sprite::create("heat/gauge", ui_scene_, 96, 96, true);
    heatgauge1_->setPickable(false);
    for( int i = 0; i < 24; ++i ) {
        heatunit1_[i] = view::Sprite::create("heat/unit0", heatgauge1_, 96, 96, true);
        heatunit1_[i]->setDepth(-5).setPickable(false).set<Rotation>(vec3(0,0,-i * 15)).set<ColorDiffuseVec3>( vec3(0,255,0) ).set<Alpha>(255);
    }
    overheat1_ = view::Sprite::create("ui/overheat4", ui_scene_, 338, 66, true);
    overheat1_->setDepth(-60).setPickable(false);
    overheat1_->set<Visible>(false);
    overheat1_->tween<SineCirc, Alpha>(0, 1000u, -1);
    player0_->player_overheat_event( bind(&Demo::set_player_overheat_ui, this, _1, _2) );

    if( game_mode_ == GM_SINGLE ) {    //2011.04.05 make stage number equal to puzzle level.
        ui_layout_->getSpriteText("stage").changeText( "level" + to_s(mode_level_ - 1) ); //first puzzle have 3 chains.
    }

    if( game_mode_ != GM_SINGLE ) { // puzzle demo WTF temp
        vec2 center_pos2( uiconf_.I("character_center_x2"), uiconf_.I("character_center_y") );
        pview2_ = presenter::PlayerView::create( c2p_, scene_, center_pos2 );
        pview2_->flipPosition();
        pview2_->setMap( map1_ );
        pview2_->setInput( ctrl::InputMgr::i().getInputByIndex(1) ); //temp: for pview to know input for rumbling wiimote
//        heatgauge2_ = view::Sprite::create("heat/0", ui_scene_, 96, 96, true);
//        heatgauge2_->set<ColorDiffuseVec3>( vec3(0,255,0) ).set<Alpha>(255);
        heatgauge2_ = view::Sprite::create("heat/gauge", ui_scene_, 96, 96, true);
        heatgauge2_->setPickable(false);
        for( int i = 0; i < 24; ++i ) {
            heatunit2_[i] = view::Sprite::create("heat/unit0", heatgauge2_, 96, 96, true);
            heatunit2_[i]->setDepth(-5).setPickable(false).set<Rotation>(vec3(0,0,-i * 15)).set<ColorDiffuseVec3>( vec3(0,255,0) ).set<Alpha>(255);;
        }
        overheat2_ = view::Sprite::create("ui/overheat4", ui_scene_, 338, 66, true);
        overheat2_->setDepth(-60).setPickable(false);
        overheat2_->set<Visible>(false);
        overheat2_->tween<SineCirc, Alpha>(0, 1000u, -1);
        player1_->player_overheat_event( bind(&Demo::set_player_overheat_ui, this, _1, _2) );

        if( game_mode_ == GM_PVC && submode_ == 99 && c1p_ == c2p_ ) {
            pview2_->setColor(vec3(0,0,0));
        }
    }

//2012.05 new heatgauge
//    utils::map_any const& gauge_conf = uiconf_.M("heatgauge");
//    vec2 gauge1_pos( gauge_conf.I("x_1p"), gauge_conf.I("y") );
//    vec2 gauge2_pos( gauge_conf.I("x_2p"), gauge_conf.I("y") );

//    heatgauge1_ = view::Sprite::create("heatgauge1", scene_, gauge_conf.I("w"), gauge_conf.I("h"), false);
//    heatgauge2_ = view::Sprite::create("heatgauge2", scene_, gauge_conf.I("w"), gauge_conf.I("h"), false);
//    heatgauge1_->set<Pos2D>( gauge1_pos ).set<ColorDiffuseVec3>( vec3(0,255,0) ).set<Alpha>(128)
//                .set<Rotation>(vec3(0, 0, gauge_conf.I("rotation")));
//    heatgauge2_->set<Pos2D>( gauge2_pos ).set<ColorDiffuseVec3>( vec3(0,255,0) ).set<Alpha>(128)
//                .set<Rotation>(vec3(0, 0, gauge_conf.I("rotation")));

    ui_layout_->setPickable(false);
    gauge1_flag_ = gauge2_flag_ = false; // for heatgauge blinking only??

    // other UI related texts here

    if( !some_ui_inited_ ) {
        //blocker_  = view::Sprite::create("blocker", ui_scene_, Conf::i().SCREEN_W() ,350, true);
        blocker_  = view::Sprite::create("blocker", ui_scene_, Conf::i().SCREEN_W() ,368, true);
        blocker_->set<Pos2D>( vec2(Conf::i().SCREEN_W() /2, Conf::i().SCREEN_H() /2) );
        blocker_->setDepth(-10).set<Alpha>(144).set<GradientDiffuse>(0).setPickable(false);

        end_text_ = view::SpriteText::create("play again?", ui_scene_, "kimberley", 30, true);
        //end_text_->setPickable(false);
        end_text2_= view::SpriteText::create("\nyes: left click\nleave: right click", ui_scene_, "kimberley", 30, true);
        //end_text2_->setPickable(false);
        win_t_    = view::Sprite::create("win", ui_scene_, 384, 192, true);
        win_t_->setDepth(-450).setPickable(false);
        lose_t_   = view::Sprite::create("lose", ui_scene_, 384, 192, true);
        lose_t_->setDepth(-450).setPickable(false);

        pause_text_ = view::SpriteText::create("Resume", ui_scene_, "kimberley", 30, true);
        pause_text_->set<Pos2D>( vec2(Conf::i().SCREEN_W() /2 - 100, Conf::i().SCREEN_H() /2 + 70) );
        pause_text_->setDepth(-450).set<Scale>(vec3(1.3, 1.3, 1.3));//.setPickable(false);

        pause_text2_ = view::SpriteText::create("Leave", ui_scene_, "kimberley", 30, true);
        pause_text2_->set<Pos2D>( vec2(Conf::i().SCREEN_W() /2 + 100, Conf::i().SCREEN_H() /2 + 70) );
        pause_text2_->setDepth(-450).set<Scale>(vec3(1.3, 1.3, 1.3));//.setPickable(false);

        pause_t_ = view::Sprite::create("pause", ui_scene_, 384, 192, true);
        pause_t_->set<Pos2D>( vec2(Conf::i().SCREEN_W()/2, Conf::i().SCREEN_H()/2 - 50) );
        pause_t_->setDepth(-450).setPickable(false);

        ready_go_text_ = view::SpriteText::create("3", ui_scene_, "GN-KillGothic", 72, true);
        ready_go_text_->set<Pos2D>( vec2(Conf::i().SCREEN_W() /2, Conf::i().SCREEN_H() /2 + 20) ); //hacky
        ready_go_text_->setDepth(-450).setPickable(false);

        pause_note_text_ = view::SpriteText::create("press middle button to pause", ui_scene_, "kimberley", 30, true);
        pause_note_text_->set<Pos2D>( vec2(Conf::i().SCREEN_W() /2, Conf::i().SCREEN_H() - 30 ) );
        pause_note_text_->tween<SineCirc, Alpha>(0, 3000u, -1);
        pause_note_text_->setDepth(-100).setPickable(false);

        some_ui_inited_ = true;
    }

    hide_upper_layer_ui();
}

void Demo::hide_upper_layer_ui()
{
    if( some_ui_inited_ ) {
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
    char_big1_.reset();
    char_big2_.reset();
}

//2012.05 fix
void Demo::update_heatgauge(ctrl::pPlayer player, view::pSprite gauge, bool& out_flag)
{
    gauge->set<Pos2D>( player->input()->getCursor()->get<Pos2D>() );
    //gauge->set<Scale>( vec3(player->heat(), 1, 1) );
//    gauge->setTexture( "heat/"+utils::to_s( static_cast<int>(player->heat() * 12.0) ) );

    /// HACK
    view::pSprite *heatunit;
    if( gauge == heatgauge1_ ) heatunit = heatunit1_;
    else heatunit = heatunit2_;

    int v = static_cast<int>(player->heat() * 24.0);

    // 0~23 or 0~24 ??
    for( int i = 0; i < 24; ++i ) {
        if( i < 12 ) {
            heatunit[i]->set<Green>( 255 /*205 + i / 24.0 * 50 * 2*/ );
            heatunit[i]->set<Red>( i / 24.0 * 255 * 2 );
        } else {
            int color_green = 255 - (i / 24.0 - 0.5) * 255 * 3.5;
            heatunit[i]->set<Green>( color_green >= 0 ? color_green : 0 );
            heatunit[i]->set<Red>(255);
        }
        if( i < v ) {
            heatunit[i]->setTexture("heat/unit1");
        } else {
            heatunit[i]->setTexture("heat/unit0");
        }
    }

    if( !player->is_overheat() ) {
        out_flag = false;
//        if( player->heat() < 0.5 ) {
//            for( int i = 0; i < 24; ++i ) {
//                heatunit[i]->set<Green>(255);
//                heatunit[i]->set<Red>( player->heat()*2*255 );
//            }
//        }
//        else {
//            for( int i = 0; i < 24; ++i ) {
//                heatunit[i]->set<Green>( 255 - (player->heat()-0.5)*255*1.5);
//                heatunit[i]->set<Red>(255);
//            }
//        }
    }
    else if( !out_flag ) {
        out_flag = true;

        audio::Sound::i().playBuffer("overheat.wav");

        for( int i = 0; i < 24; ++i )
            heatunit[i]->tween<SineCirc, ColorDiffuseVec3>(vec3(255,255,255), player->overheat_downtime()/4, 3);
    }
}

void Demo::update_ui(){
    ui_layout_->getSpriteText("scr1p").showNumber(map0_->score(), 5);
    update_heatgauge(player0_, heatgauge1_, gauge1_flag_);

    vec2 pos = player0_->input()->getCursor()->get<Pos2D>();
    pos.Y -= 60; // about half of the height of overheat texture + heatgauge's half height
    overheat1_->set<Pos2D>(pos);

    update_ui_time();

    if( game_mode_ != GM_SINGLE ) { // puzzle demo WTF temp

        int new_garbage_1p_ = map0_->garbage_left() + map1_->current_sum_of_attack();
        int new_garbage_2p_ = map1_->garbage_left() + map0_->current_sum_of_attack();
    // 2012.05 substituted by attack effect
    //    ui_layout_->getSpriteText("gar1p").showNumber(new_garbage_1p_, 0);
    //    ui_layout_->getSpriteText("gar2p").showNumber(new_garbage_2p_, 0);

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

// 2012 remove: no interaction to the background scene
//        if( pview1_->getState() == presenter::PlayerView::HIT &&
//            last_garbage_1p_ > new_garbage_1p_ ) stage_->hitGroup(1);
//        if( pview2_->getState() == presenter::PlayerView::HIT &&
//            last_garbage_2p_ > new_garbage_2p_ ) stage_->hitGroup(2);

        update_heatgauge(player1_, heatgauge2_, gauge2_flag_);

        vec2 pos = player1_->input()->getCursor()->get<Pos2D>();
        pos.Y -= 60; // about half of the height of overheat texture + heatgauge's half height
        overheat2_->set<Pos2D>(pos);

        last_garbage_1p_ = new_garbage_1p_;
        last_garbage_2p_ = new_garbage_2p_;
    }
}

void Demo::counting_by_second(){
    if( is_countdown_ ) {
        --sec_;
        if( sec_ < 0 ) --min_, sec_ = 59;
    } else {
        ++sec_;
        if( sec_ > 59 ) ++min_, sec_ = 0;
    }
}

void Demo::update_ui_time() {
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

    map0_->stop_dropping();
    player0_->stopAllActions();
    ctrl::InputMgr::i().getInputByIndex(0)->setControlledByAI(false);

    if( game_mode_ != GM_SINGLE ) { // puzzle demo WTF temp

        audio::Sound::i().stopAll(); // don't stop music when puzzle

        map1_->stop_dropping();
        player1_->stopAllActions();
        ctrl::InputMgr::i().getInputByIndex(1)->setControlledByAI(false);
    }

    /// MEMO: replay recording stops here
    App::i().getReplay().toggle_recording_andor_replaying(false, shared_from_this());

    ctrl::EventDispatcher::i().get_timer_dispatcher("game")->set_speed(1.0);
    ctrl::EventDispatcher::i().get_timer_dispatcher("ui")->set_speed(1.0);
    ctrl::EventDispatcher::i().get_timer_dispatcher("input")->set_speed(1.0);
    ctrl::EventDispatcher::i().get_timer_dispatcher("global")->set_speed(1.0);
}

void Demo::cleanup()
{
    map0_.reset();
    player0_.reset();
    pview1_.reset();

    if( game_mode_ != GM_SINGLE ) { // puzzle demo WTF temp
        map1_.reset();
        player1_.reset();
        pview2_.reset();
    }

    /// NOTE: This conditional corresponds to the same one in init_(), please take notice there.
    if( submode_ == 0 ) {
        if( stage_ ) {
            stage_->releaseResource();
            stage_.reset();
        }
    }
}

bool Demo::ai_logging(pMap lose_map)
{
    if( game_mode_ != GM_LOG )
        return false;

    ai_logging_times_ -= 1;

    utils::map_any log = Conf::i().config_of("tmp/ai_log");
    std::string record_name = c1p_.substr(5, c1p_.size()-5) + c2p_.substr(5, c2p_.size()-5);

    int index = log.V(record_name).size()-1; //back
    int win_times_0 = log.V(record_name).M(index).I("player0");
    int win_times_1 = log.V(record_name).M(index).I("player1");
    if( lose_map == map0_ ) {
        std::cout << "Player 1 win.\n";
        win_times_1 += 1;
    }
    else {
        std::cout << "Player 0 win.\n";
        win_times_0 += 1;
    }

    log.V(record_name).M(index)["player0"] = win_times_0;
    log.V(record_name).M(index)["player1"] = win_times_1;

    Conf::i().save_config(log, "tmp/ai_log");

    if( ai_logging_times_ > 0 ) {
        ctrl::EventDispatcher::i().get_timer_dispatcher("global")->subscribe(
            bind(&Demo::reinit, this), shared_from_this(), 100);
        return true;
    } else {
        if( ai_logging_conf_.V("sessions").size() > 0 ) {
            run_next_log();
            return true;
        }
        else {
            return false;
        }
    }
}

void Demo::end_phase2(pMap lose_map)
{
    printf("submode: %d\n", submode_);
    script::Lua::call(L_, "ending", game_mode_, submode_);
}


//note: temp code
void Demo::end(pMap lose_map)
{
    game_stop();
    game_state_ = GS_ENDED;

    // ai_logging special_case:
    // if I call reinit directly, won't the call-stack explode here?

    if ( ai_logging(lose_map) ) {
        return;
    }

    if( game_mode_ != GM_SINGLE )
        lose_map->ending_effect();

    update_stats_and_achievements_endgame(lose_map);

    ctrl::EventDispatcher::i().get_timer_dispatcher("game")->subscribe(
        std::tr1::bind(&Demo::end_phase2, this, lose_map), shared_from_this(), 800);
}

void button_get_focus(view::pSpriteText& sp)
{
    sp->set<Red>(0);
}

void button_lose_focus(view::pSpriteText& sp)
{
    sp->set<Red>(255);
}

// 2015.02: Functionality already subsitituded by Lua. Keeping only for reference
//void Demo::setup_end_button()
//{
//    using namespace std::tr1::placeholders;
//    std::tr1::function<void(/*int, int*/view::pSprite&)> clicka = bind(&Demo::reinit, this);
//    std::tr1::function<void(/*int, int*/view::pSprite&)> clickb = bind(&Demo::end_sequence1, this);
//    btn_reinit_ = pDummy(new int);
//
//    /// DUNNO WHY, but these additions will crash the game when quitting.
//
//    ctrl::Input const* input1 = ctrl::InputMgr::i().getInputByIndex(0);
//    ctrl::Input const* input2 = ctrl::InputMgr::i().getInputByIndex(1);
//    end_text_->onPress(&input1->trig1()) = clicka;
//    end_text_->onPress(&input2->trig1()) = clicka;
//    end_text2_->onPress(&input1->trig1()) = clickb;
//    end_text2_->onPress(&input2->trig1()) = clickb;
//    end_text_->onEnterFocus(input1) = bind(&button_get_focus, ref(end_text_));
//    end_text_->onEnterFocus(input2) = bind(&button_get_focus, ref(end_text_));
//    end_text2_->onEnterFocus(input1) = bind(&button_get_focus, ref(end_text2_));
//    end_text2_->onEnterFocus(input2) = bind(&button_get_focus, ref(end_text2_));
//    end_text_->onLeaveFocus(input1) = bind(&button_lose_focus, ref(end_text_));
//    end_text_->onLeaveFocus(input2) = bind(&button_lose_focus, ref(end_text_));
//    end_text2_->onLeaveFocus(input1) = bind(&button_lose_focus, ref(end_text2_));
//    end_text2_->onLeaveFocus(input2) = bind(&button_lose_focus, ref(end_text2_));
//    if( game_mode_ != GM_SINGLE ) { // puzzle demo WTF temp
//        BOOST_FOREACH(ctrl::Input const* input, ctrl::InputMgr::i().getInputs()) {
//            ctrl::EventDispatcher::i().subscribe_btn_event(
//                clicka, btn_reinit_, &input->trig1(), ctrl::BTN_PRESS);
//            ctrl::EventDispatcher::i().subscribe_btn_event(
//                clickb, btn_reinit_, &input->trig2(), ctrl::BTN_PRESS);
//        }
//    }
//    else {
//        ctrl::Input const* input = ctrl::InputMgr::i().getInputByIndex(0);
//        ctrl::EventDispatcher::i().subscribe_btn_event(
//            clicka, btn_reinit_, &input->trig1(), ctrl::BTN_PRESS);
//        ctrl::EventDispatcher::i().subscribe_btn_event(
//            clickb, btn_reinit_, &input->trig2(), ctrl::BTN_PRESS);
//    }
//}

void Demo::end_sequence1(bool const& ending_theme)
{
    audio::Sound::i().playBuffer("4/4c.wav");
    btn_reinit_.reset();

//2012.05 memo: because we are staying in this master presenter, and not going anywhere.
    //stage_->releaseResource(); //release when player isn't going to retry
    //App::i().launchMainMenu();
    std::cout << "game_demo end completed." << std::endl;

    script::Lua::call(L_, "cleanup", submode_);
    leaving_effect(ending_theme);
}

void Demo::pause_quit()
{
    // Pause & Quit should be the equivalent of game_stop() & end(),
    // there are some stats to cleanup...
    game_state_ = GS_ENDED;

    ctrl::EventDispatcher::i().get_timer_dispatcher("game")->start();
    //audio::Sound::i().pauseAll(false);
    btn_pause_.reset(); //reset button event subscribed by this handle.

    ctrl::EventDispatcher::i().get_timer_dispatcher("game")->subscribe(
        bind(&Demo::game_stop, this), shared_from_this(), 1); //1 ms

    ctrl::EventDispatcher::i().get_timer_dispatcher("game")->subscribe(
        bind(&Demo::end_sequence1, this, false), shared_from_this(), 1); //1 ms
}

void Demo::reinit()
{
    using std::tr1::bind;
    audio::Sound::i().playBuffer("4/4b.wav");

    if( game_mode_ != GM_SINGLE ) { // puzzle demo WTF temp
        init_(game_mode_, c1p_, c2p_, sconf_, true);
    }
//2012.05 memo: because we are staying in this master presenter, and not going anywhere.
//    ctrl::EventDispatcher::i().get_timer_dispatcher("global")->subscribe(
//        bind(&App::launchDemo, &App::i()), 500);
    std::cout << "game_demo re-initialized." << std::endl;
}

static void do_nothing(int, int){};

void Demo::pause(ctrl::Input const* controller)
{
    if( btn_pause_ ) return; //it's already paused, don't do anything if this is called again.

    if( pause_note_text_ ) pause_note_text_->set<Visible>(false);

    if( game_mode_ != GM_SINGLE ) {
        ctrl::InputMgr::i().getInputByIndex(0)->setControlledByAI(false);
        ctrl::InputMgr::i().getInputByIndex(1)->setControlledByAI(false);
        player0_->stopAllActions();
        player1_->stopAllActions();
    }

    pause_text_->set<Visible>(true);
    pause_text2_->set<Visible>(true);
    pause_t_->set<Visible>(true);
    blocker_->set<Alpha>(144).set<Visible>(true);
    blocker_->set<Pos2D>(vec2(Conf::i().SCREEN_W()/2, Conf::i().SCREEN_H()/2));

    ctrl::EventDispatcher::i().get_timer_dispatcher("game")->stop();
    audio::Sound::i().pauseAll(true);
    scene_->allowPicking(false);

    std::tr1::function<void(/*int, int*/view::pSprite&)> clicka = bind(&Demo::pause_quit, this);
    std::tr1::function<void(/*int, int*/view::pSprite&)> clickb = bind(&Demo::resume, this, controller);

    btn_pause_ = pDummy(new int);

    if( game_mode_ != GM_SINGLE ) { // puzzle demo WTF temp
        BOOST_FOREACH(ctrl::Input const* input, ctrl::InputMgr::i().getInputs()) {
            ctrl::EventDispatcher::i().subscribe_btn_event(
                do_nothing, shared_from_this(), &input->trig1(), ctrl::BTN_PRESS); //assign null
            ctrl::EventDispatcher::i().subscribe_btn_event(
                do_nothing, shared_from_this(), &input->trig2(), ctrl::BTN_PRESS); //assign null
            ctrl::EventDispatcher::i().subscribe_btn_event(
                do_nothing, shared_from_this(), &input->pause(), ctrl::BTN_PRESS); //assign null
        }
    }

    pause_text_->onPress(&controller->trig1()) = clickb;
    pause_text2_->onPress(&controller->trig1()) = clicka;
    pause_text_->onEnterFocus(controller) = bind(&button_get_focus, ref(pause_text_));
    pause_text2_->onEnterFocus(controller) = bind(&button_get_focus, ref(pause_text2_));
    pause_text_->onLeaveFocus(controller) = bind(&button_lose_focus, ref(pause_text_));
    pause_text2_->onLeaveFocus(controller) = bind(&button_lose_focus, ref(pause_text2_));

//    ctrl::EventDispatcher::i().subscribe_btn_event(
//        clicka, btn_pause_, &controller->trig1(), ctrl::BTN_PRESS);
//    ctrl::EventDispatcher::i().subscribe_btn_event(
//        clickb, btn_pause_, &controller->trig2(), ctrl::BTN_PRESS);
//    ctrl::EventDispatcher::i().subscribe_btn_event(
//        clickb, btn_pause_, &controller->pause(), ctrl::BTN_PRESS);
}

void Demo::eventual_pause()
{
    if( btn_pause_ ) return; //it's already paused, don't do anything if this is called again.

    if( pause_note_text_ ) pause_note_text_->set<Visible>(false);

    ctrl::EventDispatcher::i().get_timer_dispatcher("game")->stop();
//    audio::Sound::i().pauseAll(true);
    scene_->allowPicking(false);

    // you should not be able to overlapping pause state, so we still use this as indicator
    btn_pause_ = pDummy(new int);

    if( game_mode_ != GM_SINGLE ) { // puzzle demo WTF temp
        BOOST_FOREACH(ctrl::Input const* input, ctrl::InputMgr::i().getInputs()) {
            ctrl::EventDispatcher::i().subscribe_btn_event(
                do_nothing, shared_from_this(), &input->trig1(), ctrl::BTN_PRESS); //assign null
            ctrl::EventDispatcher::i().subscribe_btn_event(
                do_nothing, shared_from_this(), &input->trig2(), ctrl::BTN_PRESS); //assign null
            ctrl::EventDispatcher::i().subscribe_btn_event(
                do_nothing, shared_from_this(), &input->pause(), ctrl::BTN_PRESS); //assign null
        }
    }
}

void Demo::resume(ctrl::Input const* controller)
{
    if( !btn_pause_ ) return; //if it's not paused at all, don't do anything

    if( App::i().getReplay().is_replaying() && game_mode_ != GM_SINGLE ) {
        ctrl::InputMgr::i().getInputByIndex(0)->setControlledByAI(true);
        ctrl::InputMgr::i().getInputByIndex(1)->setControlledByAI(true);
        player0_->stopAllActions();
        player1_->stopAllActions();
    }
    else if( game_mode_ == GM_PVC || game_mode_ == GM_TUT ) {
        ctrl::InputMgr::i().getInputByIndex(1)->setControlledByAI(true);
        player1_->startThinking();
    }
    else if( game_mode_ == GM_CVC || game_mode_ == GM_LOG ) {
        ctrl::InputMgr::i().getInputByIndex(0)->setControlledByAI(true);
        ctrl::InputMgr::i().getInputByIndex(1)->setControlledByAI(true);
        player0_->startThinking();
        player1_->startThinking();
    }

    pause_text_->set<Visible>(false);
    pause_text2_->set<Visible>(false);
    pause_t_->set<Visible>(false);
    blocker_->set<Visible>(false);

    ctrl::EventDispatcher::i().get_timer_dispatcher("game")->start();
    audio::Sound::i().pauseAll(false);
    scene_->allowPicking(true);

    btn_pause_.reset(); //reset button event subscribed by this handle.

    if( game_mode_ != GM_SINGLE ) { // puzzle demo WTF temp
        BOOST_FOREACH(ctrl::Input const* input, ctrl::InputMgr::i().getInputs()) {
            ctrl::EventDispatcher::i().subscribe_btn_event(
                bind(&Demo::pause, this, input), shared_from_this(), &input->pause(), ctrl::BTN_PRESS);
            input->player()->subscribe_player_specific_interactions();
        }
    }
    else {
        ctrl::Input const* input = ctrl::InputMgr::i().getInputByIndex(0);
        ctrl::EventDispatcher::i().subscribe_btn_event(
            bind(&Demo::pause, this, input), shared_from_this(), &input->pause(), ctrl::BTN_PRESS);
        input->player()->subscribe_player_specific_interactions(false);
    }
}

void Demo::eventual_resume()
{
    if( !btn_pause_ ) return; //if it's not paused at all, don't do anything

    ctrl::EventDispatcher::i().get_timer_dispatcher("game")->start();
//    audio::Sound::i().pauseAll(false);
    scene_->allowPicking(true);

    btn_pause_.reset(); //reset button event subscribed by this handle.

    if( game_mode_ != GM_SINGLE ) { // puzzle demo WTF temp
        BOOST_FOREACH(ctrl::Input const* input, ctrl::InputMgr::i().getInputs()) {
            ctrl::EventDispatcher::i().subscribe_btn_event(
                bind(&Demo::pause, this, input), shared_from_this(), &input->pause(), ctrl::BTN_PRESS);
            input->player()->subscribe_player_specific_interactions();
        }
    }
    else {
        ctrl::Input const* input = ctrl::InputMgr::i().getInputByIndex(0);
        ctrl::EventDispatcher::i().subscribe_btn_event(
            bind(&Demo::pause, this, input), shared_from_this(), &input->pause(), ctrl::BTN_PRESS);
        input->player()->subscribe_player_specific_interactions(false);
    }
}

void Demo::music_state(bool f) {
    music_state_ = f;
}

void Demo::remove_all_game_scene_obj_event() //This is a callback for obj_event, so you cannot clear obj_event inside this.
{
    puzzle_started_ = true;
    ctrl::EventDispatcher::i().get_timer_dispatcher("game")->subscribe(
        bind(&ctrl::EventDispatcher::clear_obj_event, &ctrl::EventDispatcher::i(), scene_), 1);
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

    /// NEXT BIG PROBLEM:
    /// GM_SINGLE is flooded here......... how to fix?


    clock_t t0 = 0x0fffffff, t1 = 0, t2 = 0, t3 = 0, t4 = 0, t5 = 0, t6 = 0, t7 = 0;
    if( player0_ ) { //it's just some condition that the game is initialized, because we firstly initialized player0_
        t0 = clock();
        pview1_->cycle();
        if( game_mode_ != GM_SINGLE ) pview2_->cycle();
        update_ui();
        t1 = clock();
        map0_->cycle();
        if( game_mode_ != GM_SINGLE ) map1_->cycle();
        t2 = clock();

        // temp: hack, just for test
        if( game_mode_ != GM_SINGLE && game_state_ == GS_STARTED ) {
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
                                std::tr1::bind(&Demo::music_state, this, false), timer_music_state_, 7000);
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
                stage_->playFastBGM(200);
            }
            if( music_state_ == false && music_state_old_ == true ) {
                printf("Demo: false -> music_state\n");
                stage_->playBGM(500);
            }
        }
        t3 = clock();

        if( !btn_reinit_ && !btn_pause_ ) { //2011.04.09 quick fix: if these indicator is alive, stop AI's possible inputs
            player0_->cycle();
            if( game_mode_ != GM_SINGLE ) player1_->cycle();
        }

        if( (game_mode_ == GM_SINGLE || game_mode_ == GM_TUT ) && game_state_ == GS_STARTED && !btn_pause_ ) {
            script::Lua::call(L_, "check_ending_condition_by_frame", submode_);
        }

        if( game_state_== GS_STARTED && !btn_pause_ ) {
            update_stats_and_achievements_byframe();
        }

        t4 = clock();
    }
    if( stage_ ) {
        stage_->cycle();
    }
    t5 = clock();
    scene_->redraw();
    t6 = clock();
    ui_scene_->redraw();

    // temp: hack, just for test
    music_state_old_ = music_state_;

    t7 = clock();
    if( t7 - t0 > 14 ) {
        //printf(" -- Demo::profiler: %ld %ld %ld %ld %ld\n", t1-t0, t2-t1, t3-t2, t4-t3, t5-t4);
        printf(" -- Demo: ui1(%ld) maps(%ld) music(%ld) player(%ld) stage(%ld) scene(%ld) ui2(%ld)\n", t1-t0, t2-t1, t3-t2, t4-t3, t5-t4, t6-t5, t7-t6);
    }

}





// --- Stats and Achievements tracking (end game tracking)

void Demo::update_stats_and_achievements_endgame(pMap lose_map)
{
    printf(" \n\n ATTACK MADE PER SESSION 1p: %d\n", map0_->attack_made_per_session());
    printf(" ATTACK MADE PER SESSION 2p: %d\n\n\n", map1_->attack_made_per_session());

    int cubes_exploded_per_session_1p = 0;
    int cubes_exploded_per_session_2p = 0;
    for( int i = 0; i < data::COLOR_MAX; ++i ) {
        cubes_exploded_per_session_1p += map0_->cubes_cleared_data()[i];
        cubes_exploded_per_session_2p += map1_->cubes_cleared_data()[i];
    }
    int effective_clicks_1p = cubes_exploded_per_session_1p + map0_->cubes_restored();
    int effective_clicks_2p = cubes_exploded_per_session_2p + map1_->cubes_restored();

    printf(" 1p effective clicks: %d\n", effective_clicks_1p);
    printf(" 2p effective clicks: %d\n", effective_clicks_2p);

    printf(" 1p ATTACK / CLICK ratio: %lf \n", static_cast<double>(map0_->attack_made_per_session()) / effective_clicks_1p);
    printf(" 2p ATTACK / CLICK ratio: %lf \n", static_cast<double>(map1_->attack_made_per_session()) / effective_clicks_2p);

    double time_in_seconds = static_cast<double>(ctrl::EventDispatcher::i().get_timer_dispatcher("game")->get_time()) / 60000;

    printf(" 1p ATTACK / Min ratio: %lf \n", static_cast<double>(map0_->attack_made_per_session()) / time_in_seconds);
    printf(" 2p ATTACK / Min ratio: %lf \n", static_cast<double>(map1_->attack_made_per_session()) / time_in_seconds);

    /// SUPER BIG FUCKING PROBLEM! NOTE WTF
    // Because when the game ends, AI returns input control back to player! is_controlled_by_AI doesn't work here!!!!

    if( ( game_mode_ == GM_PVP || game_mode_ == GM_PVC ) && map0_ != lose_map ) {
        // note: there should be win / lose counts for different levels of AI,
        //       but it's still not possible to just use that stat to determine this achievement
        //       And not very sure if haste_count() / haste_accumulated_time() are useful or interesting stat to keep
        if( player0_->haste_count() == 0 && map0_ != lose_map && statistics_.I("achieve_win_veryhard_no_haste") == 0 ) {
            statistics_["achieve_win_veryhard_no_haste"] = 1;
            script::Lua::call(L_, "save_record_and_achievement", "achieve_win_veryhard_no_haste", true);
        }

        // note: should there be a stat_shortest_time ?
        //       shortest time vs easy / normal / hard / very hard... etc?
        if( ctrl::EventDispatcher::i().get_timer_dispatcher("game")->get_time() < 45000 &&
            ai_level_ > 0 && statistics_.I("achieve_win_lightning_fast") == 0 ) {
            statistics_["achieve_win_lightning_fast"] = 1;
            script::Lua::call(L_, "save_record_and_achievement", "achieve_win_lightning_fast", true);
        }

        // note: should there be a record of "overkill" amount ?
        //       overkill vs easy / normal / hard / very hard... etc?
        printf(" \n\n\n WHAT THE FUCK? %d   AI: %d  record: %d \n\n\n\n", map1_->garbage_left(), ai_level_, statistics_.I("achieve_win_overkill1"));
        if( map1_->garbage_left() >= 12 && ai_level_ > 0 && statistics_.I("achieve_win_overkill1") == 0 ) { // note this is map1_
            statistics_["achieve_win_overkill1"] = 1;
            script::Lua::call(L_, "save_record_and_achievement", "achieve_win_overkill1", true);
        }

        // note: same as above
        if( map1_->garbage_left() >= 24 && ai_level_ > 2 && statistics_.I("achieve_win_overkill2") == 0 ) { // note this is map1_
            statistics_["achieve_win_overkill2"] = 1;
            script::Lua::call(L_, "save_record_and_achievement", "achieve_win_overkill2", true);
        }

        // note: Feels like non-sensical to have a "how many times you have entered warning" record.
        //       So this should just be a reference to determine if player get this achievement or not?
        if( map0_->alert_triggered_count() == 0 && ai_level_ > 0 && statistics_.I("achieve_win_safety_first") == 0 ) {
            statistics_["achieve_win_safety_first"] = 1;
            script::Lua::call(L_, "save_record_and_achievement", "achieve_win_safety_first", true);
        }
    }

    if( !player0_->is_controlled_by_AI() && map0_ == lose_map ) { // there currently is only one achievement condition here
    }
}

void Demo::update_stats_and_achievements_byframe()
{
    if( !player0_->is_controlled_by_AI() ) { // this restriction should be for all achievements, right? computer earning achieves for you?

        if( map0_->highest_chain() > statistics_.I("stat_highest_chain") ) {
            statistics_["stat_highest_chain"] = map0_->highest_chain();
            script::Lua::call(L_, "save_record_and_achievement", "stat_highest_chain", map0_->highest_chain());
        }

        if( map0_->highest_single_color_match() > statistics_.I("stat_highest_single_color_match") ) {
            statistics_["stat_highest_single_color_match"] = map0_->highest_single_color_match();
            script::Lua::call(L_, "save_record_and_achievement", "stat_highest_single_color_match", map0_->highest_single_color_match());
        }

        if( map0_->highest_color_count() > statistics_.I("stat_highest_color_count") ) {
            statistics_["stat_highest_color_count"] = map0_->highest_color_count();
            script::Lua::call(L_, "save_record_and_achievement", "stat_highest_color_count", map0_->highest_color_count());
        }

        // There should really be a stat_highest_garbage_left_opponent
        if( map1_->garbage_left() >= 60 && statistics_.I("achieve_garbage_left_60") == 0 ) { // note this is map1_
            statistics_["achieve_garbage_left_60"] = 1;
            script::Lua::call(L_, "save_record_and_achievement", "achieve_garbage_left_60", true);
        }

        // There should really be a stat_shoot_opponent_count
        if( player0_->jama_shoot_count() > 0 && statistics_.I("achieve_shoot_opponent") == 0 ) {
            statistics_["achieve_shoot_opponent"] = 1;
            script::Lua::call(L_, "save_record_and_achievement", "achieve_shoot_opponent", true);
        }

        // There should really be a stat_overheat_count
        if( player0_->overheat_count() > 0 && statistics_.I("achieve_overheat") == 0 ) {
            statistics_["achieve_overheat"] = 1;
            script::Lua::call(L_, "save_record_and_achievement", "achieve_overheat", true);
        }
    }
}

void Demo::update_stats_and_achievements_startgame()
{
    if( game_mode_ == GM_PVP ) {
        // There should be a stat_player_vs_player count
        // but that alone can't determine this achievement
        // that said, for local multiplayer, until controller support is in, two mice should be the only legitimate way to do it
        // so it seemed no sense to make the distinction.

        /// NOTE: We should simply ignore the 2P keyboard setup (it was there mainly for debug purpose when no 2nd mouse was available)

        if ( ctrl::InputMgr::i().manyMouseCount() > 1 && statistics_.I("achieve_two_mice_pvp") == 0 ) {
            statistics_["achieve_two_mice_pvp"] = 1;
            script::Lua::call(L_, "save_record_and_achievement", "achieve_two_mice_pvp", true);
        }
    }
}

void Demo::load_stats_and_achievements_into_memory()
{
    if( script::Lua::call_R<bool>(L_, "record_exist", "stat_highest_chain") ) {
        statistics_["stat_highest_chain"] = script::Lua::call_R<int>(L_, "get_record", "stat_highest_chain");
    } else {
        statistics_["stat_highest_chain"] = 0;
    }

    if( script::Lua::call_R<bool>(L_, "record_exist", "stat_highest_single_color_match") ) {
        statistics_["stat_highest_single_color_match"] = script::Lua::call_R<int>(L_, "get_record", "stat_highest_single_color_match");
    } else {
        statistics_["stat_highest_single_color_match"] = 0;
    }

    if( script::Lua::call_R<bool>(L_, "record_exist", "stat_highest_color_count") ) {
        statistics_["stat_highest_color_count"] = script::Lua::call_R<int>(L_, "get_record", "stat_highest_color_count");
    } else {
        statistics_["stat_highest_color_count"] = 0;
    }

    if( script::Lua::call_R<bool>(L_, "record_exist", "achieve_garbage_left_60") ) {
        statistics_["achieve_garbage_left_60"] = static_cast<int>( script::Lua::call_R<bool>(L_, "get_record", "achieve_garbage_left_60") );
    } else {
        statistics_["achieve_garbage_left_60"] = 0;
    }

    if( script::Lua::call_R<bool>(L_, "record_exist", "achieve_two_mice_pvp") ) {
        statistics_["achieve_two_mice_pvp"] = static_cast<int>( script::Lua::call_R<bool>(L_, "get_record", "achieve_two_mice_pvp") );
    } else {
        statistics_["achieve_two_mice_pvp"] = 0;
    }

    if( script::Lua::call_R<bool>(L_, "record_exist", "achieve_shoot_opponent") ) {
        statistics_["achieve_shoot_opponent"] = static_cast<int>( script::Lua::call_R<bool>(L_, "get_record", "achieve_shoot_opponent") );
    } else {
        statistics_["achieve_shoot_opponent"] = 0;
    }

    if( script::Lua::call_R<bool>(L_, "record_exist", "achieve_win_veryhard_no_haste") ) {
        statistics_["achieve_win_veryhard_no_haste"] = static_cast<int>( script::Lua::call_R<bool>(L_, "get_record", "achieve_win_veryhard_no_haste") );
    } else {
        statistics_["achieve_win_veryhard_no_haste"] = 0;
    }

    if( script::Lua::call_R<bool>(L_, "record_exist", "achieve_win_lightning_fast") ) {
        statistics_["achieve_win_lightning_fast"] = static_cast<int>( script::Lua::call_R<bool>(L_, "get_record", "achieve_win_lightning_fast") );
    } else {
        statistics_["achieve_win_lightning_fast"] = 0;
    }

    if( script::Lua::call_R<bool>(L_, "record_exist", "achieve_overheat") ) {
        statistics_["achieve_overheat"] = static_cast<int>( script::Lua::call_R<bool>(L_, "get_record", "achieve_overheat") );
    } else {
        statistics_["achieve_overheat"] = 0;
    }

    if( script::Lua::call_R<bool>(L_, "record_exist", "achieve_win_overkill1") ) {
        statistics_["achieve_win_overkill1"] = static_cast<int>( script::Lua::call_R<bool>(L_, "get_record", "achieve_win_overkill1") );
    } else {
        statistics_["achieve_win_overkill1"] = 0;
    }

    if( script::Lua::call_R<bool>(L_, "record_exist", "achieve_win_overkill2") ) {
        statistics_["achieve_win_overkill2"] = static_cast<int>( script::Lua::call_R<bool>(L_, "get_record", "achieve_win_overkill2") );
    } else {
        statistics_["achieve_win_overkill2"] = 0;
    }

    if( script::Lua::call_R<bool>(L_, "record_exist", "achieve_win_safety_first") ) {
        statistics_["achieve_win_safety_first"] = static_cast<int>( script::Lua::call_R<bool>(L_, "get_record", "achieve_win_safety_first") );
    } else {
        statistics_["achieve_win_safety_first"] = 0;
    }
}

