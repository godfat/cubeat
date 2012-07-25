
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
        audio::Sound::i().playBGM_AB("jungle4/bgm_a.ogg", "jungle4/bgm_b.ogg");

        int play_time = 10000;

        ctrl::EventDispatcher::i().get_timer_dispatcher("global")->subscribe(
            std::tr1::bind(&TestAudio::playBGM_delayed, this, "smb_warning.wav"), play_time);

        play_time += 2500;

        ctrl::EventDispatcher::i().get_timer_dispatcher("global")->subscribe(
            std::tr1::bind(&TestAudio::playBGM_AB_delayed, this, "jungle4/bgm_quick_a.ogg", "jungle4/bgm_quick_b.ogg"), play_time);

        play_time += 10000;

        ctrl::EventDispatcher::i().get_timer_dispatcher("global")->subscribe(
            std::tr1::bind(&TestAudio::playBGM_AB_delayed, this, "jungle4/bgm_a.ogg", "jungle4/bgm_b.ogg"), play_time);

        play_time += 10000;

        ctrl::EventDispatcher::i().get_timer_dispatcher("global")->subscribe(
            std::tr1::bind(&TestAudio::playBGM_delayed, this, "smb_warning.wav"), play_time);

        play_time += 2500;

        ctrl::EventDispatcher::i().get_timer_dispatcher("global")->subscribe(
            std::tr1::bind(&TestAudio::playBGM_AB_delayed, this, "jungle4/bgm_quick_a.ogg", "jungle4/bgm_quick_b.ogg"), play_time);

        ctrl::EventDispatcher::i().get_timer_dispatcher("global")->subscribe(
            std::tr1::bind(&presenter::Stage::hitGroup, stage_.get(), 1), 3000, -1);

        ctrl::EventDispatcher::i().get_timer_dispatcher("global")->subscribe(
            std::tr1::bind(&presenter::Stage::hitGroup, stage_.get(), 2), 6000, -1);

        ctrl::EventDispatcher::i().get_timer_dispatcher("global")->subscribe(
            std::tr1::bind(&audio::Sound::check_sound_volumes, &audio::Sound::i()), 1000, -1);
    }

    void cycle(){
        stage_->cycle();
    }

    void playBGM_delayed(std::string const& path){
        audio::Sound::i().playBGM(path, 400);
    }

    void playBGM_AB_delayed(std::string const& path_a, std::string const& path_b){
        audio::Sound::i().playBGM_AB(path_a, path_b, 400);
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
