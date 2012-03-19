
#include "Conf.hpp"
#include "view/Scene.hpp"
#include "presenter/Stage.hpp"
#include "presenter/PlayerView.hpp"
#include "presenter/Map.hpp"
#include "presenter/cube/ViewSprite.hpp"
#include "presenter/game/Puzzle.hpp"

#include "EventDispatcher.hpp"
#include "Player.hpp"
#include "Accessors.hpp"
#include "utils/Random.hpp"
#include "utils/dictionary.hpp"
#include "utils/MapLoader.hpp"
#include <cstdio>

using namespace psc;

class TestPuzzle : public presenter::game::Puzzle
{
    typedef std::tr1::shared_ptr<int> pDummy;
public:
    typedef std::tr1::shared_ptr<TestPuzzle> pointer_type;
    static pointer_type create() {
        return utils::ObjectPool<TestPuzzle>::create()->init();
    }

    TestPuzzle() {
        printf("TestPuzzle created...\n");
    }
    virtual ~TestPuzzle() {
        printf("TestPuzzle Game destructing ...\n");
    }

    void end(presenter::pMap m) {
        presenter::game::Puzzle::end(m);
        ctrl::EventDispatcher::i().get_timer_dispatcher("game")->subscribe(
            bind(&TestPuzzle::setup_end_button, this), 1000);
    }

    void end_sequence1() {
        printf("This button has no use. Just Alt-F4 directly.\n");
    }

    void setup_end_button()
    {
        using std::tr1::bind;
        std::tr1::function<void(int, int)> clicka = bind(&TestPuzzle::reinit, this);
        std::tr1::function<void(int, int)> clickb = bind(&TestPuzzle::end_sequence1, this);

        btn_reinit_ = pDummy(new int);

        ctrl::Input const* input = ctrl::InputMgr::i().getInputByIndex(0);
        ctrl::EventDispatcher::i().subscribe_btn_event(
            clicka, btn_reinit_, &input->trig1(), ctrl::BTN_PRESS);
        ctrl::EventDispatcher::i().subscribe_btn_event(
            clickb, btn_reinit_, &input->trig2(), ctrl::BTN_PRESS);
    }

    void cycle() { //shadow Puzzle::cycle directly;
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

protected:
    pointer_type init() {
        presenter::game::Puzzle::init("", "", 4);
        reinit();
        ctrl::Input* input = ctrl::InputMgr::i().getInputByIndex(0);
        ctrl::EventDispatcher::i().subscribe_btn_event(
            bind(&TestPuzzle::pause, this), shared_from_this(), &input->pause(), ctrl::BTN_PRESS);

        return std::tr1::static_pointer_cast<TestPuzzle>(shared_from_this());
    }

    void reinit() {
        puzzle_conf_ = Conf::i().config_of("puzzle_config");

        data::pViewSetting s0, s1;

        s0 = data::ViewSetting::create(64);   //must use config
        s0->x_offset(159).y_offset(684);
        s1 = data::ViewSetting::create(64);   //must use config
        s1->x_offset(740).y_offset(684);

        ctrl::Input* input = ctrl::InputMgr::i().getInputByIndex(0);
        player0_ = ctrl::Player::create(input, 0);
        player0_->push_ally(0).player_hit_event(bind(&TestPuzzle::puzzle_started, this));

        // setup map0
        int generate_level = puzzle_conf_.I("generate_level");
        if( generate_level >= 3 && generate_level <= 20 ) {
            map0_ = utils::MapLoader::generate(generate_level);
        } else {
            map0_ = utils::MapLoader::load( puzzle_conf_.S("puzzle_file") );
        }

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

        if( btn_pause_ ) {
            ctrl::EventDispatcher::i().get_timer_dispatcher("game")->start();
            audio::Sound::i().pauseAll(false);
            scene_->allowPicking(true);
        }

        min_ = 0, sec_ = 0 ,last_garbage_1p_ = 0, last_garbage_2p_ = 0;
        win_ = false, puzzle_started_ = false, end_ = false;
        btn_reinit_.reset();
        btn_pause_.reset();
        win_t_.reset();
        blocker_.reset();
        end_text_.reset();
        end_text2_.reset();

        stage_->playBGM();

        //note: bad area
        timer_ui_   = pDummy(new int);
        //note: end of bad area

        ctrl::EventDispatcher::i().get_timer_dispatcher("game")->subscribe(
            bind(&TestPuzzle::update_ui_by_second, this), timer_ui_, 1000, -1);

        ctrl::EventDispatcher::i().subscribe_btn_event(
            bind(&TestPuzzle::pause, this), shared_from_this(), &input->pause(), ctrl::BTN_PRESS);

        player0_->subscribe_player_specific_interactions(false);

        pview1_->setMap( map0_ );
    }

    void update_ui_by_second() {
        presenter::game::Puzzle::update_ui_by_second();
    }

    void pause() {
        if( btn_pause_ ) return;

        ctrl::EventDispatcher::i().get_timer_dispatcher("game")->stop();
        audio::Sound::i().pauseAll(true);
        scene_->allowPicking(false);

        std::tr1::function<void(int, int)> clicka = bind(&TestPuzzle::reinit, this);
        std::tr1::function<void(int, int)> clickb = bind(&TestPuzzle::resume, this);

        btn_pause_ = pDummy(new int);

        ctrl::Input const* controller = ctrl::InputMgr::i().getInputByIndex(0);
        ctrl::EventDispatcher::i().subscribe_btn_event(
            clicka, btn_pause_, &controller->trig1(), ctrl::BTN_PRESS);
        ctrl::EventDispatcher::i().subscribe_btn_event(
            clickb, btn_pause_, &controller->trig2(), ctrl::BTN_PRESS);
        ctrl::EventDispatcher::i().subscribe_btn_event(
            clickb, btn_pause_, &controller->pause(), ctrl::BTN_PRESS);
    }

    void resume() {
        if( !btn_pause_ ) return;

        ctrl::EventDispatcher::i().get_timer_dispatcher("game")->start();
        audio::Sound::i().pauseAll(false);
        scene_->allowPicking(true);

        btn_pause_.reset(); //reset button event subscribed by this handle.

        ctrl::Input const* input = ctrl::InputMgr::i().getInputByIndex(0);
        ctrl::EventDispatcher::i().subscribe_btn_event(
            bind(&TestPuzzle::pause, this), shared_from_this(), &input->pause(), ctrl::BTN_PRESS);

        input->player()->subscribe_player_specific_interactions(false);
    }

    void puzzle_started() {
        presenter::game::Puzzle::puzzle_started();
    }

    utils::map_any puzzle_conf_;
};

typedef TestPuzzle::pointer_type pTestPuzzle;

#include "App.hpp"
#include <cstdlib> // for srand
#include <ctime> // for time, clock

#include <boost/tr1/functional.hpp> // for bind

int main(){
    std::srand(std::time(0)^std::clock()); //  init srand for global rand...
    psc::Conf::i().init("");
    psc::App::i();
    pTestPuzzle tester = TestPuzzle::create();
    return psc::App::i().run(std::tr1::bind(&TestPuzzle::cycle, tester.get()));
}
