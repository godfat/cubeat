
/* TODO:
   Inputs are temporarily declared in this scope,
   will be removed later. (when player class is out) */

#include "Conf.hpp"
#include "App.hpp"
#include "presenter/Transitioner.hpp"
#include "presenter/MainMenu.hpp"
#include "presenter/OpeningSequence.hpp"

#include "presenter/game/Multi.hpp"

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

    //these are temporarily here.
    input1_ = new Input( Conf::i().CONFIG_INPUT_PATH +"player1.zzml");
    input2_ = new Input( Conf::i().CONFIG_INPUT_PATH +"player2.zzml");

    trans_            = presenter::Transitioner::create();
    master_presenter_ = presenter::OpeningSequence::create();
    temp_presenter_   = presenter::pObject();
}

App::~App()
{
    //these are temporarily here.
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
    std::cout << "MainMenu launched.\n";
}

void App::launchMultiplayer()
{
    temp_presenter_ = presenter::game::Multi::create();
    std::cout << "game_Multiplayer launched.\n";
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
        if( IrrDevice::i().d()->isWindowActive() )
        {
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
        }
        else
            if( !IrrDevice::i().d()->getTimer()->isStopped() )
                IrrDevice::i().d()->getTimer()->stop();
    }

    return 0;
}
