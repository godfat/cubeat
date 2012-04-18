
#include "test/test_basic_enet_main.hpp"
#include "presenter/Stage.hpp"
#include "EventDispatcher.hpp"
#include "utils/dictionary.hpp"
#include "net/Manager.hpp"
#include "Conf.hpp"

using namespace psc;
using namespace test;
using namespace utils;

TestBasicENet::TestBasicENet(int type)
    :type_(type)
{
    utils::map_any stage = Conf::i().config_of("test_stage");
    stage_ = presenter::Stage::create( stage.S("test_stage") );
    stage_->playBGM();

    ctrl::EventDispatcher::i().get_timer_dispatcher("global")->subscribe(
        std::tr1::bind(&presenter::Stage::hitGroup, stage_.get(), 1), 3000, -1);

    ctrl::EventDispatcher::i().get_timer_dispatcher("global")->subscribe(
        std::tr1::bind(&presenter::Stage::hitGroup, stage_.get(), 2), 6000, -1);

    net::Manager::i().start_lua(type);
}

TestBasicENet::~TestBasicENet() {
    net::Manager::i().quit_lua();
}

void TestBasicENet::cycle() {
    stage_->cycle();
}

#include "App.hpp"
#include <cstdlib> // for srand
#include <ctime> // for time, clock

#include <boost/tr1/functional.hpp> // for bind

int main(int argc, char* argv[]){
    std::srand(std::time(0)^std::clock()); //  init srand for global rand...
    psc::Conf::i().init("");
    psc::App::i();
    if( argc > 1 ) {
        TestBasicENet tester(1);
        return psc::App::i().run(std::tr1::bind(&TestBasicENet::cycle, &tester));
    }
    else {
        TestBasicENet tester(2);
        return psc::App::i().run(std::tr1::bind(&TestBasicENet::cycle, &tester));
    }
}


