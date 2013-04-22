
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
    TestGGPO(): temp_corrected_frame_(125), frame_(0), real_new_frame_(0), rollbacked_(false), ggpo_(0) {
        scene_ = view::Scene::create("game");

        ctrl::EventDispatcher::i().get_timer_dispatcher("game")->stop();

        scene_->setTo2DView();
        utils::Random2::i().init(12345);
        utils::Random2::i().set_frame(0);

        ggpo_setup();

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
//        player1_ = ctrl::AIPlayer::create(input1, 1, "ai/normal.lua");

        /// WTF
        player0_->push_ally(0);

//        player0_->push_ally(0).push_enemy(1);
//        player1_->push_ally(1).push_enemy(0);

        // setup map0
        data::pMapSetting set0 = data::MapSetting::create( gameplay_.M("player1") );

        /// Debug: use a very basic setup to determine rollback problem:
//        set0->dropping_creatable(false);
//        utils::vector_2d<int> test_data(6, 11);
//        test_data[0][0] = 0;
//        test_data[1][0] = 1;
//        test_data[2][0] = 1;
//        test_data[1][1] = 2;
//        test_data[2][1] = 2;
//        test_data[0][2] = 1;
//        map0_ = presenter::Map::create(set0, test_data, player0_);

        map0_ = presenter::Map::create(set0, player0_);
        map0_->set_view_master( presenter::cube::ViewSpriteMaster::create(scene_, s0, player0_) );

        // setup map1
//        data::pMapSetting set1 = data::MapSetting::create( gameplay_.M("player2") );
//        map1_ = presenter::Map::create(set1, player1_);
//        map1_->set_view_master( presenter::cube::ViewSpriteMaster::create(scene_, s1, player1_) );

        // setup garbage land
//        map0_->push_garbage_land(map1_);
//        map1_->push_garbage_land(map0_);

        ///NEW: MAKE PLAYER KNOWS ABOUT MAP
        std::vector< presenter::wpMap > map_list;
        map_list.push_back(map0_);
//        map_list.push_back(map1_);
        player0_->setMapList( map_list );
//        player1_->setMapList( map_list );

        // MAYBE: we have to dispatch here because we have to make sure there are timers to rollback
        //        if we have to rollback to right after the init state (where we have already setup
        //        newly created timers, but not yet started to poll inputs just yet.
        ctrl::EventDispatcher::i().dispatch();

        // backup right after the initialization!
        utils::Random2::i().end_frame();
        utils::pools_backup(0);

        ctrl::EventDispatcher::i().get_timer_dispatcher("game")->start();
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

    void precondition() {
        ++frame_;
        if( frame_ > real_new_frame_ ) {
            real_new_frame_ = frame_;
        }

        if( frame_ == 8 + temp_corrected_frame_ ) {
            //ctrl::EventDispatcher::i().get_timer_dispatcher("game")->stop();
            printf("rollbacked! From %d to %d\n", frame_, temp_corrected_frame_);

            /// UPDATE NOTE:
            /// All un-synchronized part of TimerDispatcher when I was inspecting with system("pause") seemed resolved.
            /// And always remember the metadata of std containers, such as begin()/end()/size() etc
            /// reside in the enveloping class "physically".

            /// Current solution note: timers_ and newly_created_timers_ have to be Restorable
            /// (since newly_created_timers_ goes across frames), but frame_counter_ & game-time related
            /// variables should be kept from Restorable with a barrier, because those are "INPUT" data,
            /// not game states. Review this if in doubt => game_state[f+1] = update(game_state[f], input{key, time, random})

            ctrl::EventDispatcher::i().get_timer_dispatcher("game")->remove_all_newly_created_timers();

            utils::pools_restore(temp_corrected_frame_);

            // Put this here?
            /// NOTE: Can't put this here? causing crash when new TimerCallbacks get pushed into..
            map0_->cleanup_dead_cubes();
//            map1_->cleanup_dead_cubes();

            //ctrl::EventDispatcher::i().get_timer_dispatcher("game")->start();
            frame_ = temp_corrected_frame_+1;
            ++temp_corrected_frame_;
        }

        // can't be sure EventDispatcher won't use Random2... so that's it.
        utils::Random2::i().set_frame(frame_);
        ctrl::EventDispatcher::i().get_timer_dispatcher("game")->set_frame(frame_);
    }

    void update_state(int f) {
        printf("Frame: %d\n", f);

        std::vector<double> chances;
        chances.push_back(40);
        chances.push_back(40);
        chances.push_back(20);

        printf("%d %d %d %d %d\n", utils::r_random(50),
                                   utils::r_random(100),
                                   utils::r_random(150),
                                   utils::distributed_random(chances),
                                   utils::r_random(25));

        map0_->cycle();
//        map1_->cycle();
        player0_->cycle();
//        player1_->cycle();

        // YOU SHOULD NOT CALL THIS DURING ROLLBACK
        // Have to find a condition here for testing randoms during a rollback...
        if( f == real_new_frame_ ) { // If .. this is true, then it should be a whole new frame and not in rollback
            utils::Random2::i().end_frame();
        }

        // Have to set it here for the next cycle, I guess...
        utils::pools_backup(f);
    }

    // note to self: It is imperative to understand random number generator (RNG) and TIME (or DELTA TIME)
    // as an "INPUT", which means, if we say we find inconsistency at frame N's input, and thus we have to
    // rollback to frame M, because we find that frame M is the last frame with consistent inputs from both sides,
    // we should only rollback the RNG to frame M+1, so that the finished-and-recoreded state of frame M
    // (based on the calculations from the last correct input), combined with the newly found corrected input
    // of frame M+1, will produce the correct outcome state M+1, and so on.

    // A VERY STUPID ASSERTION ERROR:
    // The premises of this rollback method, is that you CAN'T, and you WON'T rollback to the same point
    // in the past TWICE or MORE. The history of time is ever-progressing, so every backup is only useful
    // ONCE. My object pool adoption made this assertion in the very beginning, once a restoration
    // is made then the memory blocks of the backup is immediately freed.

    // But stupidly I test it with a code that constantly rolling-back to frame 3,
    // Of course that will never work...

    void cycle(){
        update_state( frame_ );

        scene_->redraw();
    }

    void cleanup() { //might not be used in test
        ggpo_close_session(ggpo_);
#if defined(__WIN32__) || defined(WIN32)
        WSACleanup();
#endif
    }

private:
    utils::map_any gameplay_;
    view::pScene  scene_;
    view::pSprite sprite_;
    pMap map0_, map1_;
    ctrl::pPlayer player0_, player1_;

    int temp_corrected_frame_;

    int frame_, real_new_frame_;
    bool rollbacked_;

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
    return psc::App::i().run(std::tr1::bind(&TestGGPO::cycle, &tester),
                             std::tr1::bind(&TestGGPO::precondition, &tester));
}
