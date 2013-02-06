
#include "data/MapSetting.hpp"
#include "data/ViewSetting.hpp"
#include "view/Scene.hpp"
#include "view/Sprite.hpp"
#include "EventDispatcher.hpp"
#include "ctrl/TimerDispatcher.hpp"
#include "Input.hpp"
#include "Player.hpp"
#include "ctrl/AIPlayer.hpp"
#include "presenter/Map.hpp"
#include "presenter/cube/ViewSprite.hpp"
#include "presenter/cube/ViewSpriteMaster.hpp"
#include "utils/dictionary.hpp"
#include "utils/Random.hpp"
#include "utils/ObjectPool.hpp"
#include "Conf.hpp"

#include "ggponet.h"

#if defined(__WIN32__) || defined(WIN32)
#include "windows.h"
#endif

using namespace psc;
using namespace presenter;

bool test_ggpo_begin_game(char* name) { // do nothing?
    return true;
}

bool test_ggpo_advance_frame(int flags) {
    return true;
}

bool test_ggpo_load_state(unsigned char* buf, int len) {
    return true;
}

bool test_ggpo_save_state(unsigned char** buf, int *len, int* checksum, int frame) {
    return true;
}

void test_ggpo_free_buffer(void* buf) {
}

bool test_ggpo_net_log(char* filename, unsigned char* buffer, int len) {
    return true;
}

bool test_ggpo_net_event(GGPOEvent* e) {
    return true;
}

static time_t realtime()
{
    return IrrDevice::i().d()->getTimer()->getRealTime();
}

class TestGGPO{
public:
    TestGGPO(): frame_(0), rollbacked_(false), test_data1_with_timer_(0), test_data2_with_timer_(1.0), ggpo_(0) {
        scene_ = view::Scene::create("game");
        scene_->setTo2DView();
        utils::Random2::i().init(12345);
        my_timer_ = ctrl::TimerDispatcher::create("test_ggpo", true);
        ctrl::EventDispatcher::i().add_timer_dispatcher(my_timer_);

        using std::tr1::bind;
        using std::tr1::ref;
        my_timer_->subscribe(bind(&TestGGPO::test_data1, this), 20, 10);
        my_timer_->subscribe(bind(&TestGGPO::test_data2, this, ref(test_data2_with_timer_)), 20, 10);

        ggpo_setup();


        utils::Random2::i().set_frame(1);

        data::pViewSetting s0, s1;

        gameplay_ = Conf::i().config_of("gameplay/multi");

        s0 = data::ViewSetting::create(64);   //must use config
        s0->x_offset(159).y_offset(684);
        s1 = data::ViewSetting::create(64);   //must use config
        s1->x_offset(740).y_offset(684);

        ///THIS IS IMPORTANT, ALL PLAYERS MUST BE DEFINED FIRST.
        ctrl::Input* input0 = ctrl::InputMgr::i().getInputByIndex(0);
        ctrl::Input* input1 = ctrl::InputMgr::i().getInputByIndex(1);
        player0_ = ctrl::Player::create(input0, 0);
        player1_ = ctrl::AIPlayer::create(input1, 1, "ai/normal.lua");
        player0_->push_ally(0).push_enemy(1);
        player1_->push_ally(1).push_enemy(0);

        // setup map0
        data::pMapSetting set0 = data::MapSetting::create( gameplay_.M("player1") );
        map0_ = presenter::Map::create(set0, player0_);
        map0_->set_view_master( presenter::cube::ViewSpriteMaster::create(scene_, s0, player0_) );

        // setup map1
        data::pMapSetting set1 = data::MapSetting::create( gameplay_.M("player2") );
        map1_ = presenter::Map::create(set1, player1_);
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
    }

    void test_data1() {
        ++test_data1_with_timer_;
        printf("Test Data1: %d\n", test_data1_with_timer_);
    }

    void test_data2(double& a) {
        a *= 2;
        printf("Test Data2: %lf (%lf)\n", a, test_data2_with_timer_);
    }

    ~TestGGPO() {
        cleanup();
    }

    void ggpo_setup() {
#if defined(__WIN32__) || defined(WIN32)
        WSADATA wd = { 0 };
        WSAStartup(MAKEWORD(2, 2), &wd);
#else
        printf("WTF: Networking implementations are not available for this platform yet.\n");
#endif
        ggpo_callbacks_.begin_game      = test_ggpo_begin_game;
        ggpo_callbacks_.advance_frame   = test_ggpo_advance_frame;
        ggpo_callbacks_.load_game_state = test_ggpo_load_state;
        ggpo_callbacks_.save_game_state = test_ggpo_save_state;
        ggpo_callbacks_.free_buffer     = test_ggpo_free_buffer;
        ggpo_callbacks_.on_event        = test_ggpo_net_event;

        //necessary?
        ggpo_callbacks_.log_game_state  = test_ggpo_net_log;

        GGPOErrorCode result = ggpo_start_session(&ggpo_, &ggpo_callbacks_, "psc::TestGGPO", 2, sizeof(int), 7000);
        printf("GGPO Error: %d\n", result);

        ggpo_set_disconnect_timeout(ggpo_, 3000);
        ggpo_set_disconnect_notify_start(ggpo_, 3000);

        GGPOPlayer p1, p2;
        GGPOPlayerHandle h1, h2;

        p1.size = p2.size = sizeof(GGPOPlayer);
        p1.type = GGPO_PLAYERTYPE_LOCAL;            // local player
        p2.type = GGPO_PLAYERTYPE_REMOTE;           // remote player
        p1.player_num = 1;
        p2.player_num = 2;
        strcpy(p2.u.remote.ip_address, "127.0.0.1");// ip addess of the player
        p2.u.remote.port = 7001;                    // port of that player
        printf("%s:%d\n", p2.u.remote.ip_address, p2.u.remote.port);

        result = ggpo_add_player(ggpo_, &p1,  &h1);
        printf("GGPO Error: %d\n", result);
        ggpo_set_frame_delay(ggpo_, h1, 2);         // only set frame delay for self

        result = ggpo_add_player(ggpo_, &p2,  &h2);
        printf("GGPO Error: %d\n", result);

        printf("GGPO: connecting...\n");
    }

    void update_state(int f) {
        printf("Frame: %d\n", f);
        utils::Random2::i().set_frame(f);

        std::vector<double> chances;
        chances.push_back(40);
        chances.push_back(40);
        chances.push_back(20);

        printf("%d %d %d %d %d\n", utils::r_random(50),
                                   utils::r_random(100),
                                   utils::r_random(150),
                                   utils::distributed_random(chances),
                                   utils::r_random(25));

        utils::pools_backup(f);
    }

    void cleanup() { //might not be used in test
        ggpo_close_session(ggpo_);
#if defined(__WIN32__) || defined(WIN32)
        WSACleanup();
#endif
    }

    void cycle(){
        ++frame_;
        if( frame_ == 3 ) {
            printf("rollbacked!!\n");
            utils::pools_restore(1);
            for( int i = 1; i < 3; ++i ) {
                update_state(i);
            }
        }
        if( frame_ == 5 ) {
            printf("rollbacked!! twice\n");
            utils::pools_restore(2);
            for( int i = 2; i < 5; ++i ) {
                update_state(i);
            }
        }
        update_state( frame_ );

        scene_->redraw();

        map0_->cycle();
        map1_->cycle();
        player0_->cycle();
        player1_->cycle();

        utils::Random2::i().end_frame();
    }

private:
    utils::map_any gameplay_;
    view::pScene  scene_;
    view::pSprite sprite_;
    pMap map0_, map1_;
    ctrl::pPlayer player0_, player1_;

    int frame_;
    bool rollbacked_;
    int    test_data1_with_timer_;
    double test_data2_with_timer_;

    ctrl::pTimerDispatcher my_timer_;

    //networking and ggpo related
    GGPOSession* ggpo_;
    GGPOSessionCallbacks ggpo_callbacks_;
};

#include "App.hpp"
#include <cstdlib> // for srand
#include <ctime> // for time, clock

#include <boost/tr1/functional.hpp> // for bind

int main(){
    std::srand(std::time(0)^std::clock()); //  init srand for global rand...
    psc::Conf::i().init("");
    psc::App::i().init();
    TestGGPO tester;
    return psc::App::i().run(std::tr1::bind(&TestGGPO::cycle, &tester));
}
