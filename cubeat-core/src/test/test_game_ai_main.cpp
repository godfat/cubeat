

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
#include "ctrl/AIPlayer.hpp"
#include "Weapon.hpp"
#include "utils/Random.hpp"
#include "utils/dictionary.hpp"
#include "utils/MapLoader.hpp"
#include "utils/to_s.hpp"

#include <boost/tr1/functional.hpp> // for bind
#include <boost/thread/thread.hpp>
#include <boost/foreach.hpp>

using namespace psc;
using namespace easing;
using namespace accessor;
using utils::to_s;

class TestGame{

public:
    TestGame(){
        scene_ = psc::view::Scene::create("game");
        //scene_->setTo2DView().enableGlobalHittingEvent();     //important

        data::pViewSetting s0, s1;

        s0 = data::ViewSetting::create(64);   //must use config
        s0->x_offset(159).y_offset(684);
        s1 = data::ViewSetting::create(64);   //must use config
        s1->x_offset(740).y_offset(684);

        ctrl::AIPlayer::AISetting ai_temp[3] =
        {ctrl::AIPlayer::Easy(), ctrl::AIPlayer::Normal(), ctrl::AIPlayer::Hard()};

        ///THIS IS IMPORTANT, ALL PLAYERS MUST BE DEFINED FIRST.
        ctrl::Input* input0 = ctrl::InputMgr::i().getInputByIndex(0);
        ctrl::Input* input1 = ctrl::InputMgr::i().getInputByIndex(1);

        input0->setControlledByAI(true);
        input1->setControlledByAI(true);
        std::random_shuffle(ai_temp, ai_temp + 3);
        player0_ = ctrl::AIPlayer::create(input0, 0, ai_temp[0]);
        player1_ = ctrl::AIPlayer::create(input1, 1, ai_temp[1]);

        player0_->push_ally(0).push_enemy(1);
        player1_->push_ally(1).push_enemy(0);

        // setup player settings
        //player0_->set_config(gameplay_.M("player1").M("weapon"));
        //player1_->set_config(gameplay_.M("player2").M("weapon"));

        // setup map0
        data::pMapSetting set0 = data::MapSetting::create();
        //map0_ = presenter::Map::create(set0, utils::MapLoader::load_cube_colors("config/puzzle.zzml"));
        map0_ = presenter::Map::create(set0);
        map0_->set_view_master( presenter::cube::ViewSpriteMaster::create(scene_, s0, player0_) );

        // setup map1
        data::pMapSetting set1 = data::MapSetting::create();
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

        //start music
        //stage_->playBGM();

        ctrl::EventDispatcher::i().get_timer_dispatcher("game")->subscribe(
            std::tr1::bind(&TestGame::update_ui_by_second, this), 1000, -1);

        if( !player0_->startThinking() )
            std::cout << " cpu player 0 AI failed the initialization." << std::endl;

        if( !player1_->startThinking() )
            std::cout << " cpu player 1 AI failed the initialization." << std::endl;
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
        pview1_ = presenter::PlayerView::create( c1p.size() ? c1p : "char/char1.zzml", scene_, center_pos );
        pview2_ = presenter::PlayerView::create( c2p.size() ? c2p : "char/char2.zzml", scene_, center_pos );
        pview2_->flipPosition();
        pview1_->setMap( map0_ );
        pview2_->setMap( map1_ );

        utils::map_any const& gauge_conf = uiconf_.M("heatgauge");
        vec2 gauge1_pos( gauge_conf.I("x_1p"), gauge_conf.I("y") );
        vec2 gauge2_pos( gauge_conf.I("x_2p"), gauge_conf.I("y") );
        heatgauge1_ = view::Sprite::create("heatgauge1", scene_, gauge_conf.I("w"), gauge_conf.I("h"), false);
        heatgauge2_ = view::Sprite::create("heatgauge2", scene_, gauge_conf.I("w"), gauge_conf.I("h"), false);
        heatgauge1_->set<Pos2D>( gauge1_pos ).set<ColorDiffuseVec3>( vec3(0,255,0) ).set<Alpha>(128)
                    .set<Rotation>(vec3(0, 0, gauge_conf.I("rotation")));
        heatgauge2_->set<Pos2D>( gauge2_pos ).set<ColorDiffuseVec3>( vec3(0,255,0) ).set<Alpha>(128)
                    .set<Rotation>(vec3(0, 0, gauge_conf.I("rotation")));

        gauge1_flag_ = gauge2_flag_ = false;
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
        player0_->cycle();
        player1_->cycle();
    }

    void update_ui(){
        int new_garbage_1p_ = map0_->garbage_left() + map1_->current_sum_of_attack();
        int new_garbage_2p_ = map1_->garbage_left() + map0_->current_sum_of_attack();
        ui_layout_->getSpriteText("gar1p").showNumber(new_garbage_1p_);
        ui_layout_->getSpriteText("gar2p").showNumber(new_garbage_2p_);
        ui_layout_->getSpriteText("scr1p").showNumber(map0_->score(), 5);
        ui_layout_->getSpriteText("scr2p").showNumber(map1_->score(), 5);
        //ui_layout_->getSpriteText("wep1p1").showNumber(player0_->weapon(0)->ammo(), 2);
        //ui_layout_->getSpriteText("wep1p2").showNumber(player0_->weapon(1)->ammo(), 2);
        //ui_layout_->getSpriteText("wep1p3").showNumber(player0_->weapon(2)->ammo(), 2);
        //ui_layout_->getSpriteText("wep2p1").showNumber(player1_->weapon(0)->ammo(), 2);
        //ui_layout_->getSpriteText("wep2p2").showNumber(player1_->weapon(1)->ammo(), 2);
        //ui_layout_->getSpriteText("wep2p3").showNumber(player1_->weapon(2)->ammo(), 2);

        if( pview1_->getState() == presenter::PlayerView::HIT &&
            last_garbage_1p_ > new_garbage_1p_ ) stage_->hitGroup(1);
        if( pview2_->getState() == presenter::PlayerView::HIT &&
            last_garbage_2p_ > new_garbage_2p_ ) stage_->hitGroup(2);

        update_heatgauge(player0_, heatgauge1_, gauge1_flag_);
        update_heatgauge(player1_, heatgauge2_, gauge2_flag_);

        last_garbage_1p_ = new_garbage_1p_;
        last_garbage_2p_ = new_garbage_2p_;
    }

    void update_heatgauge(ctrl::pPlayer player, view::pSprite gauge, bool& out_flag) {
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

    void update_ui_by_second(){
        ++sec_;
        if( sec_ > 59 ) ++min_, sec_ = 0;
        std::string sec = to_s(sec_); if( sec.size() < 2 ) sec = "0" + sec;
        std::string min = to_s(min_); if( min.size() < 2 ) min = "0" + min;
        ui_layout_->getSpriteText("time").changeText( min + ":" + sec );
    }

private:
    view::pScene scene_;
    presenter::pStage stage_;
    presenter::pMap map0_;
    presenter::pMap map1_;
    ctrl::pPlayer player0_;
    ctrl::pPlayer player1_;

    utils::map_any uiconf_;
    view::pMenu ui_layout_;

    presenter::pPlayerView pview1_;
    presenter::pPlayerView pview2_;
    view::pSprite heatgauge1_;
    view::pSprite heatgauge2_;

    int min_, sec_;
    bool gauge1_flag_, gauge2_flag_;
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
    return psc::App::i().init().run(std::tr1::bind(&TestGame::cycle, &tester));
}
