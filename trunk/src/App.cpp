
/* TODO:
   Inputs are temporarily declared in this scope,
   will be removed later. (when player class is out) */

#include "App.hpp"
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

App::App()
:framerate_(60), last_timetick_(0)
{
    std::cout << "App constructed.\n";
    if( !IrrDevice::i().init(true) ) {
        std::cout << "Graphic engine initialization failed. Halting...\n";
        return;
    }
    timer_ = IrrDevice::i().d()->getTimer();
    input1_ = new Input("config/input_setting_1p.yml");
    input2_ = new Input("config/input_setting_2p.yml");
    ViewTest1::i();
}

App::~App()
{
    delete input1_;
    delete input2_;
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

    while( IrrDevice::i().run() )
        if( IrrDevice::i().d()->isWindowActive() && !update_block() )
        {
            Input::update_all();
            EventDispatcher::i().dispatch();

            driver->beginScene(true, true, video::SColor(0,64,64,64));
            ViewTest1::i().cycle();
            driver->endScene();
        }

    return 0;
}

