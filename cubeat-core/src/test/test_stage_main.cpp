
#include "presenter/Stage.hpp"
#include "EventDispatcher.hpp"
#include "utils/Random.hpp"
#include "utils/dictionary.hpp"

using namespace psc;

class TestStage{
    typedef std::vector< view::pAnimatedSceneObject > SceneObjList;

public:
    TestStage(){
        //temporary Scene:

        utils::map_any stage = utils::map_any::construct( utils::fetchConfig("config/test_stage.zzml") );
        stage_ = presenter::Stage::create( stage.S("test_stage") );
        stage_->playBGM();

        ctrl::EventDispatcher::i().subscribe_timer(
            std::tr1::bind(&presenter::Stage::hitGroup, stage_.get(), 1), 3000, -1);

        ctrl::EventDispatcher::i().subscribe_timer(
            std::tr1::bind(&presenter::Stage::hitGroup, stage_.get(), 2), 6000, -1);
    }

    void cycle(){
        stage_->cycle();
    }

private:
    presenter::pStage stage_;
};

#include "App.hpp"
#include <cstdlib> // for srand
#include <ctime> // for time, clock

#include <tr1/functional> // for bind

int main(){
    std::srand(std::time(0)^std::clock()); //  init srand for global rand...
    psc::App::i();
    TestStage tester;
    return psc::App::i().run(std::tr1::bind(&TestStage::cycle, &tester));
}
