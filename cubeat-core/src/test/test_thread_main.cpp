
#include "presenter/Stage.hpp"
#include "ctrl/AIPlayer.hpp"
#include "Input.hpp"
#include "EventDispatcher.hpp"
#include "utils/Random.hpp"
#include "utils/dictionary.hpp"

#include <tr1/functional> // for bind
#include <boost/thread/thread.hpp>

using namespace psc;

class TestThread{
    typedef std::vector< view::pAnimatedSceneObject > SceneObjList;

public:
    TestThread(){
        //temporary Scene:

        utils::map_any stage = utils::map_any::construct( utils::fetchConfig("config/test_stage.zzml") );
        stage_ = presenter::Stage::create( stage.S("test_stage") );
        stage_->playBGM();

        ctrl::EventDispatcher::i().subscribe_timer(
            std::tr1::bind(&presenter::Stage::hitGroup, stage_.get(), 1), 3000, -1);

        ctrl::EventDispatcher::i().subscribe_timer(
            std::tr1::bind(&presenter::Stage::hitGroup, stage_.get(), 2), 6000, -1);

        ctrl::Input* cpu_input = ctrl::InputMgr::i().getInputByIndex(1);
        cpu_input->setControlledByAI(true);
        cpu1_ = ctrl::AIPlayer::create(cpu_input);

        if( !cpu1_->startThinking() )
            std::cout << "CPU AI failed the initialization." << std::endl;
    }

    void cycle(){
        stage_->cycle();
        //cpu1_->cycle();
    }

private:
    boost::mutex  mutex_;
    presenter::pStage stage_;
    ctrl::pAIPlayer cpu1_;
};

#include "App.hpp"
#include <cstdlib> // for srand
#include <ctime> // for time, clock

int main(){
    std::srand(std::time(0)^std::clock()); //  init srand for global rand...
    psc::App::i();
    TestThread tester;
    return psc::App::i().run(std::tr1::bind(&TestThread::cycle, &tester));
}
