
#include "view/Scene.hpp"
#include "presenter/Stage.hpp"
#include "EventDispatcher.hpp"
#include "utils/Random.hpp"
#include "utils/dictionary.hpp"
#include "audio/Sound.hpp"
#include "Conf.hpp"
#include <sstream>

using namespace psc;

class TestAudio{
    typedef std::vector< view::pAnimatedSceneObject > SceneObjList;

public:
    TestAudio(){
        //temporary Scene:
        utils::map_any stage = Conf::i().config_of("test_stage");
        stage_ = presenter::Stage::create( stage.S("test_stage") );
        //stage_->playBGM();
        audio::Sound::i().playBGM("jungle1/bgm_b.ogg");

        ctrl::EventDispatcher::i().get_timer_dispatcher("global")->subscribe(
            std::tr1::bind(&TestAudio::playBGM_delayed, this, 2), 3000);

        ctrl::EventDispatcher::i().get_timer_dispatcher("global")->subscribe(
            std::tr1::bind(&TestAudio::playBGM_delayed, this, 3), 6000);

        ctrl::EventDispatcher::i().get_timer_dispatcher("global")->subscribe(
            std::tr1::bind(&TestAudio::playBGM_delayed, this, 4), 9000);

        ctrl::EventDispatcher::i().get_timer_dispatcher("global")->subscribe(
            std::tr1::bind(&TestAudio::playBGM_delayed, this, 5), 12000);

        ctrl::EventDispatcher::i().get_timer_dispatcher("global")->subscribe(
            std::tr1::bind(&TestAudio::playBGM_delayed, this, 6), 15000);

        ctrl::EventDispatcher::i().get_timer_dispatcher("global")->subscribe(
            std::tr1::bind(&presenter::Stage::hitGroup, stage_.get(), 1), 3000, -1);

        ctrl::EventDispatcher::i().get_timer_dispatcher("global")->subscribe(
            std::tr1::bind(&presenter::Stage::hitGroup, stage_.get(), 2), 6000, -1);
    }

    void cycle(){
        stage_->cycle();
    }

    void playBGM_delayed(int number){
        std::ostringstream oss;
        oss << "jungle" << number << "/bgm_b.ogg";
        audio::Sound::i().playBGM(oss.str());
    }

private:
    presenter::pStage stage_;
};

#include "App.hpp"
#include <cstdlib> // for srand
#include <ctime> // for time, clock

#include <boost/tr1/functional.hpp> // for bind

int main(){
    std::srand(std::time(0)^std::clock()); //  init srand for global rand...
    psc::Conf::i().init("");
    psc::App::i().init();
    TestAudio tester;
    return psc::App::i().run(std::tr1::bind(&TestAudio::cycle, &tester));
}
