
#include "presenter/Stage.hpp"
#include "ctrl/AIPlayer.hpp"
#include "Input.hpp"
#include "EventDispatcher.hpp"
#include "ctrl/TimerDispatcher.hpp"
#include "utils/Random.hpp"
#include "utils/dictionary.hpp"
#include "data/ViewSetting.hpp"

#include <boost/tr1/functional.hpp> // for bind
#include <boost/thread/thread.hpp>

using namespace psc;

class TestThread{
    typedef std::vector< view::pAnimatedSceneObject > SceneObjList;

public:
    TestThread(){
        utils::map_any stage = utils::map_any::construct( utils::fetchConfig("config/test_stage.zzml") );
        stage_ = presenter::Stage::create( stage.S("test_stage") );
        stage_->playBGM();

        ctrl::EventDispatcher::i().get_timer_dispatcher("global")->subscribe(
            std::tr1::bind(&presenter::Stage::hitGroup, stage_.get(), 1), 3000, -1);

        ctrl::EventDispatcher::i().get_timer_dispatcher("global")->subscribe(
            std::tr1::bind(&presenter::Stage::hitGroup, stage_.get(), 2), 6000, -1);

        data::pViewSetting s0, s1;
        s0 = data::ViewSetting::create(64);   //must use config
        s0->x_offset(159).y_offset(684).push_ally(0).push_enemy(1);
        s1 = data::ViewSetting::create(64);   //must use config
        s1->x_offset(740).y_offset(684).push_ally(1).push_enemy(0);

        ctrl::Input* cpu_input = ctrl::InputMgr::i().getInputByIndex(1);
        cpu_input->setControlledByAI(true);
        cpu1_ = ctrl::AIPlayer::create(cpu_input, s1);

        if( !cpu1_->startThinking() )
            std::cout << "CPU AI failed the initialization." << std::endl;
    }

    void cycle(){
        stage_->cycle();
        cpu1_->cycle();
    }

private:
    boost::mutex mutex_;
    presenter::pStage stage_;
    ctrl::pAIPlayer cpu1_;
};

#include "App.hpp"
#include <cstdlib> // for srand
#include <ctime> // for time, clock

int main(){
    std::srand(std::time(0)^std::clock()); //  init srand for global rand...
    psc::Conf::i().init("");
    psc::App::i().init();
    TestThread tester;
    return psc::App::i().run(std::tr1::bind(&TestThread::cycle, &tester));
}
