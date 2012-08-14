
#include "Conf.hpp"
#include "App.hpp"
#include "presenter/Transitioner.hpp"
#include "presenter/MainMenu.hpp"
#include "presenter/OpeningSequence.hpp"
#include "presenter/Opening2.hpp"

#include "presenter/game/Demo.hpp"
#include "presenter/game/Multi.hpp"
#include "presenter/game/Puzzle.hpp"
#include "view/SFX.hpp"
#include "view/Scene.hpp"

#include "audio/Sound.hpp"
#include "Input.hpp"
#include "IrrDevice.hpp"
#include "EventDispatcher.hpp"
#include "ctrl/TimerDispatcher.hpp"

#include <iostream>
#include <boost/tr1/functional.hpp>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;

using namespace psc;
using namespace ctrl;
using std::tr1::ref;

App::App()
    : framerate_( Conf::i().FRAMERATE() ), last_timetick_(0), quit_(false)
{
}

App::~App()
{
    std::cout << "App destructing, before deleting inputs" << std::endl;
}

//2012.05 apparently, if we want to initialized some App related callbacks to
//presenters or others, it'll have to do outside of App's c'tor!
App& App::init()
{
    std::cout << "App started initializing. (not constructor)" << std::endl;
    if( !IrrDevice::i().init(true) ) {
        std::cout << "Graphic engine initialization failed. Halting..." << std::endl;
        return *this;
    }

    global_timer_ = EventDispatcher::i().get_timer_dispatcher("global");

    InputMgr::i().createInputs();
    audio::Sound::i().init();

    trans_            = presenter::Transitioner::create();
    master_presenter_ = presenter::Object::create();

    view::pScene preload = view::Scene::create("PreLoad Scene");
    view::SFX::i().init_textures(preload);
    std::cout << "App fully initialized." << std::endl;
    return *this;
}

App& App::setLoading(int const& cent)
{
    trans_->setLoadingBar(cent);
    return *this;
}

App& App::launchOpening()
{
    temp_presenter_ = presenter::OpeningSequence::create();
    std::cout << "Opening launched." << std::endl;
    return *this;
}

App& App::launchOpening2()
{
    temp_presenter_ = presenter::Opening2::create();
    std::cout << "Opening2 launched." << std::endl;
    return *this;
}

App& App::launchDemo()
{
    temp_presenter_ = presenter::game::Demo::create();
    std::cout << "Demo launched." << std::endl;
    return *this;
}

App& App::launchMainMenu()
{
    temp_presenter_ = presenter::MainMenu::create();
    std::cout << "MainMenu launched." << std::endl;
    return *this;
}

App& App::launchMultiplayer(std::string const& conf1p, std::string const& conf2p,
                            std::string const& stage, int num_of_cpu, int ai_level)
{
    temp_presenter_ = presenter::game::Multi::create(conf1p, conf2p, stage, num_of_cpu, ai_level);
    std::cout << "game_Multiplayer launched." << std::endl;
    return *this;
}

App& App::launchPuzzle(std::string const& conf1p, std::string const& stage, int puzzle_level)
{
    temp_presenter_ = presenter::game::Puzzle::create(conf1p, stage, puzzle_level);
    std::cout << "game_puzzle launched." << std::endl;
    return *this;
}

bool App::update_block()
{
    u32 now_time = global_timer_.lock()->get_time();
    if( now_time - last_timetick_ <= 1000/framerate_ ) {
        return true;
    }
    last_timetick_ = now_time;
    return false;
}

App& App::pause()
{
    if( !global_timer_.lock()->is_stopped() )
        global_timer_.lock()->stop();
    return *this;
}

App& App::resume()
{
    if( global_timer_.lock()->is_stopped() )
        global_timer_.lock()->start();
    return *this;
}

App& App::quit()
{
    quit_ = true;
    return *this;
}

int App::run(std::tr1::function<void()> tester)
{
    using namespace irr;
    using namespace core;
    using namespace scene;
    using namespace video;

    using namespace ctrl;

    IVideoDriver* driver = IrrDevice::i().d()->getVideoDriver();
    int lastFPS = -1;
    int profile_time_start = 0;
    clock_t t0 = clock(), t1, t2, t3, t4, t5, t6, t7, t8, t9, t10;

    while( IrrDevice::i().run() && !quit_ ) {
        //if( IrrDevice::i().d()->isWindowActive() )                   //comment: temp for double tasking
        //{                                                            //comment: temp for double tasking
        //    if( global_timer_.lock()->isStopped() )        //comment: temp for double tasking
        //        global_timer_.lock()->start();             //comment: temp for double tasking
            //if( update_block() ) continue;

            //t1 = clock();
            InputMgr::i().updateAll();
            t2 = clock();
            EventDispatcher::i().dispatch();
            t3 = clock();
            driver->beginScene(true, true, video::SColor(0,0,0,0));
            t4 = clock();
            printf(" recording App::t4 %ld\n", t4);
            if( tester ) tester();
            else master_presenter_->cycle();
            t5 = clock();
            printf(" recording App::t5 %ld\n", t5);
            driver->clearZBuffer();
            trans_->cycle();
            //t6 = clock();
            InputMgr::i().redrawAll();
            //t7 = clock();
            audio::Sound::i().cycle();
            //t8 = clock();
            view::SFX::i().cleanup(); //newly added, clean up effects pool every cycle.
            //t9 = clock();
            driver->endScene();
            //t10 = clock();
            int elapsed_time = clock() - profile_time_start;

            if( elapsed_time > 30 && elapsed_time < 500 ) {
                std::cout << "frame time spike: " << elapsed_time << "\n";
                printf(" -- App::t2:%ld, t3:%ld, t4:%ld, t5:%ld\n", t2, t3, t4, t5);
            }

            t0 = clock();
            profile_time_start = clock();

            //FPS for debug
            int fps = driver->getFPS();
            if( fps != lastFPS ) {
                core::stringw str(L"FPS: ");
                str += fps;
                IrrDevice::i().d()->setWindowCaption( str.c_str() );
                lastFPS = fps;
            }
            if( temp_presenter_ ) { //hand over master presenter here "safely."
                master_presenter_ = temp_presenter_;
                temp_presenter_.reset();
            }
        //}                                                      //comment: temp for double tasking
        //else                                                   //comment: temp for double tasking
            //if( !timer_->isStopped() ) //comment: temp for double tasking
                //timer_->stop();        //comment: temp for double tasking
    }

    std::cout << "App main loop has ended." << std::endl;
    if( master_presenter_ ) //hack: make the recollection of master_presenter_ faster.
        master_presenter_.reset();
    return 0;
}
