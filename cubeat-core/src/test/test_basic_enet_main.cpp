
#include "test/test_basic_enet_main.hpp"
#include "presenter/Stage.hpp"
#include "EventDispatcher.hpp"
#include "utils/dictionary.hpp"
#include "Conf.hpp"

using namespace psc;
using namespace test;
using namespace utils;

TestBasicENet::TestBasicENet() {
    utils::map_any stage = Conf::i().config_of("test_stage");
    stage_ = presenter::Stage::create( stage.S("test_stage") );
    stage_->playBGM();

    ctrl::EventDispatcher::i().get_timer_dispatcher("global")->subscribe(
        std::tr1::bind(&presenter::Stage::hitGroup, stage_.get(), 1), 3000, -1);

    ctrl::EventDispatcher::i().get_timer_dispatcher("global")->subscribe(
        std::tr1::bind(&presenter::Stage::hitGroup, stage_.get(), 2), 6000, -1);
}

void TestBasicENet::cycle() {
    stage_->cycle();
}

#include "App.hpp"
#include <cstdlib> // for srand
#include <ctime> // for time, clock

#include <boost/tr1/functional.hpp> // for bind

int main(){
    std::srand(std::time(0)^std::clock()); //  init srand for global rand...
    psc::Conf::i().init("");
    psc::App::i();
    TestBasicENet tester;
    return psc::App::i().run(std::tr1::bind(&TestBasicENet::cycle, &tester));
}


