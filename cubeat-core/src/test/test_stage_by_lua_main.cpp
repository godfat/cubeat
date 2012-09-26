
#include "EventDispatcher.hpp"
#include "ctrl/TimerDispatcher.hpp"
#include "presenter/Stage2.hpp"
#include "audio/Sound.hpp"
#include "Conf.hpp"

#include <sstream>

using namespace psc;

class TestStageLua
{
public:
    TestStageLua() {
        stage_ = presenter::Stage2::create("stage/jungle6");
    }

    void cycle() {
        stage_->cycle();
    }

private:
    presenter::pStage2 stage_;
};

#include "App.hpp"
#include <cstdlib> // for srand
#include <ctime> // for time, clock

#include <boost/tr1/functional.hpp> // for bind

int main(){
    std::srand(std::time(0)^std::clock()); //  init srand for global rand...
    psc::Conf::i().init("");
    psc::App::i().init();
    TestStageLua tester;
    return psc::App::i().run(std::tr1::bind(&TestStageLua::cycle, &tester));
}
