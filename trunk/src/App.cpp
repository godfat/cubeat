
/* TODO:
   Inputs are temporarily declared in this scope,
   will be removed later. (when player class is out) */

#include "Conf.hpp"
#include "App.hpp"
#include "presenter/Transitioner.hpp"
#include "presenter/MainMenu.hpp"
#include "presenter/OpeningSequence.hpp"
#include "Input.hpp"
#include "EventDispatcher.hpp"
#include "IrrDevice.hpp"

/* Testcase */
#include "testcase/ViewTest1.hpp"

#include <iostream>

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
    std::cout << "App constructed.\n";
    if( !IrrDevice::i().init(true) ) {
        std::cout << "Graphic engine initialization failed. Halting...\n";
        return;
    }
    timer_ = IrrDevice::i().d()->getTimer();

    //these are temporarily here.
    input1_ = new Input( Conf::i().CONFIG_INPUT_PATH +"player1.zzml");
    input2_ = new Input( Conf::i().CONFIG_INPUT_PATH +"player2.zzml");

    trans_            = presenter::Transitioner::create();
    master_presenter_ = presenter::OpeningSequence::create();
    temp_presenter_   = presenter::pObject();

    std::cout << view::Map::create(1, view::Scene::create("Test") ).get() << "\n";
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

bool App::update_block()
{
    u32 now_time = timer_->getRealTime();
    if( now_time - last_timetick_ <= 1000/framerate_ ) {
        return true;
    }
    last_timetick_ = now_time;
    return false;
}

int App::run()
{
    IVideoDriver* driver = IrrDevice::i().d()->getVideoDriver();
    int lastFPS = -1;

    std::tr1::shared_ptr<ViewTest1> viewtest(new ViewTest1);
    viewtest->init();

    while( IrrDevice::i().run() )
        if( IrrDevice::i().d()->isWindowActive() /*&& !update_block()*/ )
        {
            Input::update_all();
            EventDispatcher::i().dispatch();

            driver->beginScene(true, true, video::SColor(0,0,0,0));
            master_presenter_->cycle();
            driver->clearZBuffer();  //clear z-buffer to overlay the whole scene
            trans_->cycle();
            driver->clearZBuffer();
            viewtest->cycle();
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

    return 0;
}

