
#include "view/Scene.hpp"
#include "view/SpriteText.hpp"
#include "view/Menu.hpp"
#include "Accessors.hpp"

#include "presenter/Stage.hpp"
#include "presenter/PlayerView.hpp"
#include "presenter/Map.hpp"
#include "presenter/cube/ViewSpriteMaster.hpp"

#include "EventDispatcher.hpp"
#include "ctrl/TimerDispatcher.hpp"
#include "Input.hpp"
#include "Player.hpp"
#include "Weapon.hpp"
#include "utils/Random.hpp"
#include "utils/dictionary.hpp"
#include "utils/MapLoader.hpp"
#include "utils/to_s.hpp"
#include <boost/foreach.hpp>

using namespace psc;
using namespace easing;
using namespace accessor;
using utils::to_s;
using std::tr1::bind;

class TestGame{
    typedef std::tr1::shared_ptr<int> pDummy;
public:
    TestGame(){
        scene_ = psc::view::Scene::create("game");
        scene_->setTo2DView()/*.enableGlobalHittingEvent()*/;   //2011.03.28 weapon temporary removal

        ctrl::EventDispatcher::i().get_timer_dispatcher("game")->stop();
        scene_->allowPicking(false);

        gameplay_ = Conf::i().config_of("gameplay/multi");

        data::pViewSetting s0, s1;

        s0 = data::ViewSetting::create(64);   //must use config
        s0->x_offset(159).y_offset(684);
        s1 = data::ViewSetting::create(64);   //must use config
        s1->x_offset(740).y_offset(684);

        ///THIS IS IMPORTANT, ALL PLAYERS MUST BE DEFINED FIRST.
        player0_ = ctrl::Player::create(ctrl::InputMgr::i().getInputByIndex(0), 0);
        player1_ = ctrl::Player::create(ctrl::InputMgr::i().getInputByIndex(1), 1);
        player0_->debug_reset_all_weapon();
        player1_->debug_reset_all_weapon();
        player0_->push_ally(0).push_enemy(1);
        player1_->push_ally(1).push_enemy(0);

        // setup player settings
        player0_->set_config(gameplay_.M("player1").M("weapon"));
        player1_->set_config(gameplay_.M("player2").M("weapon"));

        // setup map0
        data::pMapSetting set0 = data::MapSetting::create( gameplay_.M("player1") );
        //map0_ = presenter::Map::create(set0, utils::MapLoader::load_cube_colors("config/puzzle.zzml"));
        map0_ = presenter::Map::create(set0);
        map0_->set_view_master( presenter::cube::ViewSpriteMaster::create(scene_, s0, player0_) );

        // setup map1
        data::pMapSetting set1 = data::MapSetting::create( gameplay_.M("player2") );
        map1_ = presenter::Map::create(set1);
        map1_->set_view_master( presenter::cube::ViewSpriteMaster::create(scene_, s1, player1_) );

        // setup garbage land
        map0_->push_garbage_land(map1_);
        map1_->push_garbage_land(map0_);

        ///NEW: MAKE PLAYER KNOWS ABOUT MAP
        std::vector< presenter::wpMap > map_list;
        map_list.push_back(map0_);
        map_list.push_back(map1_);
        player0_->setMapList( map_list );
        player1_->setMapList( map_list );

        // setup stage & ui & player's view objects:
        utils::map_any stage = Conf::i().config_of("test_stage");
        stage_ = presenter::Stage::create( stage.S("test_stage") );
        setup_ui_by_config( "char/char1.zzml",
                            "char/char2.zzml",
                            "ui/in_game_2p_layout.zzml" );

        min_ = 0, sec_ = 0 ,last_garbage_1p_ = 0, last_garbage_2p_ = 0;

        ready_go_text_ = view::SpriteText::create("3", scene_, "kimberley", 30, true);
        ready_go_text_->set<Pos2D>( vec2(Conf::i().SCREEN_W() /2, Conf::i().SCREEN_H() /2) );
        ready_go_text_->setPickable(false);

        ctrl::EventDispatcher::i().get_timer_dispatcher("global")->subscribe(
            bind(&TestGame::game_start, this), 3000);

        ready_go(3);
    }

    void ready_go(int step) {
        if ( step < 0 ) {
            ready_go_text_->tween<Linear, Alpha>(0, 500u);
            return;
        }
        else if ( step == 0 ) {
            ready_go_text_->changeText("go!");
            ready_go_text_->set<Scale>(vec3(1.5,1.5,1.5));
            //need sound fx here
        }
        else {
            ready_go_text_->showNumber(step);
            ready_go_text_->set<Scale>(vec3(5,5,5));
            //need sound fx here
        }
        ready_go_text_->tween<OElastic, Scale>(vec3(5,5,5), 900u, 0);
        ctrl::EventDispatcher::i().get_timer_dispatcher("global")->subscribe(
            std::tr1::bind(&TestGame::ready_go, this, step-1), 1000);
    }


    void game_start() {
        //start music
        stage_->playBGM();

        //note: bad area
        //timer_item_ = pDummy(new int);                  //2011.03.25 item temporarily removed
        timer_ui_   = pDummy(new int);
        //note: end of bad area

        using std::tr1::bind;
        ctrl::EventDispatcher::i().get_timer_dispatcher("game")->subscribe(
            bind(&TestGame::update_ui_by_second, this), timer_ui_, 1000, -1);
        //2011.03.25 item temporarily removed
        //ctrl::EventDispatcher::i().get_timer_dispatcher("game")->subscribe(
        //    bind(&Multi::item_creation, this), timer_item_, 15000);

        ctrl::EventDispatcher::i().get_timer_dispatcher("game")->start();
        scene_->allowPicking(true);

        player0_->subscribe_player_specific_interactions();
        player1_->subscribe_player_specific_interactions();
    }


    void setup_ui_by_config( std::string const& c1p, std::string const& c2p, std::string const& path ) {
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
        pview2_ = presenter::PlayerView::create( c2p.size() ? c2p : "config/char/char2.zzml", scene_, center_pos );
        pview2_->flipPosition();
        pview1_->setMap( map0_ );
        pview2_->setMap( map1_ );
    }

    void cycle(){
        pview1_->cycle();
        pview2_->cycle();
        update_ui();
        stage_->cycle();
        //IrrDevice::i().d()->getVideoDriver()->clearZBuffer();
        scene_->redraw();
        map0_->cycle();
        map1_->cycle();
    }

    void update_ui(){
        int new_garbage_1p_ = map0_->garbage_left() + map1_->current_sum_of_attack();
        int new_garbage_2p_ = map1_->garbage_left() + map0_->current_sum_of_attack();
        ui_layout_->getSpriteText("gar1p").showNumber(new_garbage_1p_);
        ui_layout_->getSpriteText("gar2p").showNumber(new_garbage_2p_);
        ui_layout_->getSpriteText("scr1p").showNumber(map0_->score(), 5);
        ui_layout_->getSpriteText("scr2p").showNumber(map1_->score(), 5);
//        ui_layout_->getSpriteText("wep1p1").showNumber(player0_->weapon(0)->ammo(), 2);
//        ui_layout_->getSpriteText("wep1p2").showNumber(player0_->weapon(1)->ammo(), 2);
//        ui_layout_->getSpriteText("wep1p3").showNumber(player0_->weapon(2)->ammo(), 2);
//        ui_layout_->getSpriteText("wep2p1").showNumber(player1_->weapon(0)->ammo(), 2);
//        ui_layout_->getSpriteText("wep2p2").showNumber(player1_->weapon(1)->ammo(), 2);
//        ui_layout_->getSpriteText("wep2p3").showNumber(player1_->weapon(2)->ammo(), 2);

        if( pview1_->getState() == presenter::PlayerView::HIT &&
            last_garbage_1p_ > new_garbage_1p_ ) stage_->hitGroup(1);
        if( pview2_->getState() == presenter::PlayerView::HIT &&
            last_garbage_2p_ > new_garbage_2p_ ) stage_->hitGroup(2);

        last_garbage_1p_ = new_garbage_1p_;
        last_garbage_2p_ = new_garbage_2p_;
    }

    void update_ui_by_second(){
        ++sec_;
        if( sec_ > 59 ) ++min_, sec_ = 0;
        std::string sec = to_s(sec_); if( sec.size() < 2 ) sec = "0" + sec;
        std::string min = to_s(min_); if( min.size() < 2 ) min = "0" + min;
        ui_layout_->getSpriteText("time").changeText( min + ":" + sec );
    }

private:
    utils::map_any gameplay_;

    view::pScene scene_;
    presenter::pStage stage_;
    presenter::pMap map0_;
    presenter::pMap map1_;
    ctrl::pPlayer player0_;
    ctrl::pPlayer player1_;

    utils::map_any uiconf_;
    view::pMenu ui_layout_;

    view::pSpriteText ready_go_text_;
    pDummy timer_ui_;

    presenter::pPlayerView pview1_;
    presenter::pPlayerView pview2_;

    int min_, sec_;
    int last_garbage_1p_, last_garbage_2p_; //used for temporary state comparison
};

#include "App.hpp"
#include <cstdlib> // for srand
#include <ctime> // for time, clock

#include <boost/tr1/functional.hpp> // for bind

int main(){
    std::srand(std::time(0)^std::clock()); //  init srand for global rand...
    psc::Conf::i().init("");
    psc::App::i().init();
    TestGame tester;
    return psc::App::i().run(std::tr1::bind(&TestGame::cycle, &tester));
}
