
#include "Conf.hpp"
#include "App.hpp"
#include "presenter/Transitioner.hpp"
#include "presenter/MainMenu.hpp"
#include "presenter/OpeningSequence.hpp"

#include "presenter/game/Multi.hpp"
#include "presenter/game/Puzzle.hpp"

#include "Input.hpp"
#include "IrrDevice.hpp"
#include "EventDispatcher.hpp"

#include <iostream>
#include <tr1/functional>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;

using namespace psc;
using namespace ctrl;
using std::tr1::bind;
using std::tr1::ref;

App::App()
    : framerate_( Conf::i().FRAMERATE ), last_timetick_(0)
{
    std::cout << "App constructed." << std::endl;
    if( !IrrDevice::i().init(true) ) {
        std::cout << "Graphic engine initialization failed. Halting..." << std::endl;
        return;
    }
    timer_ = IrrDevice::i().d()->getTimer();

    input1_ = new Input( Conf::i().CONFIG_INPUT_PATH +"player1.zzml");
    input2_ = new Input( Conf::i().CONFIG_INPUT_PATH +"player2.zzml");

    trans_            = presenter::Transitioner::create();
    master_presenter_ = presenter::OpeningSequence::create();
    temp_presenter_   = presenter::pObject();
}

App::~App()
{
    std::cout << "App destructing, before deleting inputs" << std::endl;
    delete input1_;
    delete input2_;
}

void App::setLoading(int const& cent)
{
    trans_->setLoadingBar(cent);
}

void App::launchMainMenu()
{
    temp_presenter_ = presenter::MainMenu::create();
    std::cout << "MainMenu launched." << std::endl;
}

void App::launchMultiplayer(std::string const& conf1p, std::string const& conf2p,
                            std::string const& stage)
{
    temp_presenter_ = presenter::game::Multi::create(conf1p, conf2p, stage);
    std::cout << "game_Multiplayer launched." << std::endl;
}

void App::launchPuzzle(std::string const& conf1p, std::string const& stage, int puzzle_level)
{
    temp_presenter_ = presenter::game::Puzzle::create(conf1p, stage, puzzle_level);
    std::cout << "game_puzzle launched." << std::endl;
}

bool App::update_block()
{
    u32 now_time = timer_->getTime();
    if( now_time - last_timetick_ <= 1000/framerate_ ) {
        return true;
    }
    last_timetick_ = now_time;
    return false;
}

int App::run(std::tr1::function<void()> tester)
{
    using namespace irr;
    using namespace core;
    using namespace scene;
    using namespace video;

    using namespace ctrl;

    IVideoDriver* driver = IrrDevice::i().d()->getVideoDriver();
    Input::init_graphic_items();
    int lastFPS = -1;

    while( IrrDevice::i().run() ) {
        //if( IrrDevice::i().d()->isWindowActive() )                   //temp for double tasking
        //{                                                            //temp for double tasking
            if( IrrDevice::i().d()->getTimer()->isStopped() )
                IrrDevice::i().d()->getTimer()->start();
            //if( update_block() ) continue;
            Input::update_all();
            EventDispatcher::i().dispatch();

            driver->beginScene(true, true, video::SColor(0,0,0,0));
            if( tester ) tester();
            else master_presenter_->cycle();
            driver->clearZBuffer();
            trans_->cycle();

            Input::redraw_all();

            driver->endScene();

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
        //}                                                      //temp for double tasking
        //else                                                   //temp for double tasking
        //    if( !IrrDevice::i().d()->getTimer()->isStopped() ) //temp for double tasking
        //        IrrDevice::i().d()->getTimer()->stop();        //temp for double tasking
    }

    std::cout << "App main loop has ended." << std::endl;

    return 0;
}
