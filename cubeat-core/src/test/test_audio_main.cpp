
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
        //audio::Sound::i().playBGM_AB("jungle4/bgm_a.ogg", "jungle4/bgm_b.ogg");
        audio::Sound::i().playBGM_AB("jungle5/bgm_a.ogg", "jungle5/bgm_b.ogg");

        playSample_delayed("1/a/1a-1.wav", 2345);
        playSample_delayed("1/a/1a-2.wav", 3632);
        playSample_delayed("1/a/1a-3.wav", 3928);
        playSample_delayed("1/a/1a-4.wav", 4212);
        playSample_delayed("2/2b_1.wav", 3000);
        playSample_delayed("2/2b_1.wav", 3000);
        playSample_delayed("2/2b_1.wav", 3000);
        playSample_delayed("2/2b_2.wav", 3500);
        playSample_delayed("2/2b_3.wav", 4000);
        playSample_delayed("2/2b_4.wav", 4500);
        playSample_delayed("2/2b_4.wav", 4500);
        playSample_delayed("2/2b_5.wav", 5500);
        playSample_delayed("2/2b_6.wav", 6000);
        playSample_delayed("2/2b_7.wav", 6666);

        playSample_delayed("3/3d/alarm.wav", 7000);
        playSample_delayed("3/3d/alarm.wav", 8000);
        playSample_delayed("3/3d/alarm.wav", 9000);
        playSample_delayed("3/3d/alarm.wav", 10000);
        playSample_delayed("3/3d/alarm.wav", 11000);
        playSample_delayed("3/3d/alarm.wav", 12000);
        playSample_delayed("3/3d/alarm.wav", 13000);
        playSample_delayed("3/3d/alarm.wav", 14000);
        playSample_delayed("3/3d/alarm.wav", 15000);
        playSample_delayed("3/3d/alarm.wav", 16000);

        int play_time = 10000;
//
        playBGM_delayed("smb_warning.wav", 400, play_time);
        play_time += 2500;

        //seek_and_playBGM_AB_delayed("jungle4/bgm_quick_a.ogg", "jungle4/bgm_quick_b.ogg", 400, 987, play_time);
        seek_and_playBGM_AB_delayed("jungle5/bgm_quick_a.ogg", "jungle5/bgm_quick_b.ogg", 400, 987, play_time);
        play_time += 10000;

        //seek_and_playBGM_AB_delayed("jungle4/bgm_a.ogg", "jungle4/bgm_b.ogg", 1000, 500, play_time);
        seek_and_playBGM_AB_delayed("jungle5/bgm_a.ogg", "jungle5/bgm_b.ogg", 1000, 500, play_time);
        play_time += 10000;

//        playBGM_delayed("smb_warning.wav", play_time);
//        play_time += 2500;
//
//        playBGM_AB_delayed("jungle4/bgm_quick_a.ogg", "jungle4/bgm_quick_b.ogg", play_time);
//        //seek_and_playBGM_AB_delayed("jungle5/bgm_quick_a.ogg", "jungle5/bgm_quick_b.ogg", 987, play_time);
//        play_time += 10000;
//
//        seek_and_playBGM_AB_delayed("jungle4/bgm_a.ogg", "jungle4/bgm_b.ogg", 9321, play_time);
//        //seek_and_playBGM_AB_delayed("jungle5/bgm_a.ogg", "jungle5/bgm_b.ogg", 26000, play_time);
//        play_time += 30000;
//
//        playBGM_delayed("smb_warning.wav", play_time);
//        play_time += 2500;
//
//        seek_and_playBGM_AB_delayed("jungle4/bgm_quick_a.ogg", "jungle4/bgm_quick_b.ogg", 789, play_time);

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

    void playBGM_delayed(std::string const& path, time_t const& fade_t, time_t const& play_time){
        ctrl::EventDispatcher::i().get_timer_dispatcher("global")->subscribe(
            std::tr1::bind(&audio::Sound::playBGM, &audio::Sound::i(), path, fade_t, 0), play_time);
    }

    void playBGM_AB_delayed(std::string const& path_a, std::string const& path_b, time_t const& fade_t, time_t const& play_time){
        ctrl::EventDispatcher::i().get_timer_dispatcher("global")->subscribe(
            std::tr1::bind(&audio::Sound::playBGM_AB, &audio::Sound::i(), path_a, path_b, fade_t), play_time);
    }

    void playSample_delayed(std::string const& sf, time_t const& play_time){
        ctrl::EventDispatcher::i().get_timer_dispatcher("global")->subscribe(
            std::tr1::bind(&audio::Sound::playSample, &audio::Sound::i(), sf, 0, 0), play_time);
    }

    void seek_and_playBGM_AB_delayed(std::string const& pa, std::string const& pb, time_t const& fade_t, time_t const& seekms, time_t const& play_time){
        ctrl::EventDispatcher::i().get_timer_dispatcher("global")->subscribe(
            std::tr1::bind(&audio::Sound::seek_and_playBGM_AB, &audio::Sound::i(), pa, pb, seekms, fade_t), play_time);
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
