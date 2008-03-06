
/* TODO:
   Inputs are temporarily declared in this scope,
   will be removed later. (when player class is out) */

#include "Conf.hpp"
#include "App.hpp"
#include "presenter/Transitioner.hpp"
#include "presenter/MainMenu.hpp"
#include "presenter/OpeningSequence.hpp"
#include "Input.hpp"
#include "IrrDevice.hpp"

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

bool App::update_block()
{
    u32 now_time = timer_->getTime();
    if( now_time - last_timetick_ <= 1000/framerate_ ) {
        return true;
    }
    last_timetick_ = now_time;
    return false;
}
