
#include "audio/Sound.hpp"
#include "audio/SoundObject.hpp"
//#include "audio/detail/OpenAL.hpp"
#include "audio/detail/ALmixer.hpp"
#include "EventDispatcher.hpp"
#include "ctrl/TimerDispatcher.hpp"
#include "Conf.hpp"

#include <utility>
#include <boost/foreach.hpp>

#include <iostream>

using namespace psc;
using namespace audio;

Sound::Sound()
#if defined(_SHOOTING_CUBES_ANDROID_)
    :base_path_(Conf::i().android_ext_path() + "rc/sound/"),
#else
    :base_path_("rc/sound/"),
#endif
     inited_(false), main_track_(0)
{
    //detail::sound_init();
    #ifdef _SHOOTING_CUBES_ENABLE_SOUND_
    detail2::sound_init();
    ALmixer_SetMasterVolume(.7f);
    #endif
}

void audio::Sound::init()
{
    if( inited_ == true ) return;
    //If we have to preload some file here, then .. just do it.
    //or client code can call loadStream or loadBuffer beforehand as they see fit.
    inited_ = true;
}

//Sound& Sound::loadStream(std::string const& path)
//{
//    std::string absolute_path = base_path_ + path;
//    pSoundStream new_stream = SoundStream::create(absolute_path);
//    //you cannot use std::make_pair to increase the use_count of shared_ptr. It's probably reference.
//    sound_streams_[path] = new_stream;
//    return *this;
//}

Sound& Sound::loadBuffer(std::string const& path)
{
    std::string absolute_path = base_path_ + path;
    //pSoundBuffer new_buffer = SoundBuffer::create(absolute_path);
    pSoundSample new_buffer = SoundSample::create(absolute_path, false);
    //you cannot use std::make_pair to increase the use_count of shared_ptr. It's probably reference.
    sound_samples_[path] = new_buffer;
    return *this;
}

Sound& Sound::loadSample(std::string const& path)
{
    std::string absolute_path = base_path_ + path;
    pSoundSample new_sample = SoundSample::create(absolute_path, true);
    //you cannot use std::make_pair to increase the use_count of shared_ptr. It's probably reference.
    sound_samples_[path] = new_sample;
    return *this;
}
//
//pSoundObject Sound::playStream(std::string const& path, bool const& loop)
//{
//    // temp: HACK!!!!!
//    return playSample(path, 0, loop);
//
////    if( !sound_streams_[path] )   //each path name -> stream(file) is unique.
////        loadStream(path);         //normally we should avoid this for big audio files.
////
////    pSoundObject new_sound = SoundObject::create(sound_streams_[path], loop);
////    sound_list_.push_back(new_sound);
//
//    //std::cout << " Sound: we have " << sound_list_.size() << " sounds playing now." << std::endl;
//
//    //return *this;
//}

//deprecated API: use playSample instead in the future.
//kept for compatibility
Sound& Sound::playBuffer(std::string const& path, bool const& loop)
{
//    if( !sound_buffers_[path] )   //each path name -> stream(file) is unique.
//        loadBuffer(path);         //normally we should avoid this for big audio files.
//
//    pSoundObject new_sound = SoundObject::create(sound_buffers_[path]);
//    new_sound->play(0, loop?-1:0);
//    sound_list_.push_back(new_sound);
//
//    return *this;
    playSample(path, 0, loop);
    return *this;
}

Sound& Sound::playSample(std::string const& path, time_t const& fade_t, int const& loop)
{
    if( !sound_samples_[path] )   //each path name -> sample(file is unique.
        loadBuffer(path);         //normally we should avoid this for big audio files.

    pSoundObject new_sound = SoundObject::create(sound_samples_[path]);
    new_sound->play(fade_t, loop);
    sound_list_.push_back(new_sound);
    return *this;
}

void Sound::internal_so_init(pSoundObject& s, std::string const& a, std::string const& b) {
    s = SoundObject::create(sound_samples_[a]);
    if( b.size() ) s->partB(sound_samples_[b]);
}

int Sound::loadBGM_AB(std::string const& path_a, std::string const& path_b)
{
    if( !sound_samples_[path_a] )
        loadSample(path_a);
    if( !sound_samples_[path_b] )
        loadSample(path_b);

    if( !bgm_[0] ) {
        internal_so_init(bgm_[0], path_a, path_b);
        return 0;
    }
    else if( !bgm_[1] ) {
        internal_so_init(bgm_[1], path_a, path_b);
        return 1;
    }
    else {
        int track = main_track_ == 0 ? 1 : 0;
        bgm_[track]->stop();
        bgm_[track].reset();
        internal_so_init(bgm_[track], path_a, path_b);
        return track;
    }
}

int Sound::loadBGM(std::string const& path)
{
    if( !sound_samples_[path] )
        loadSample(path);

    if( !bgm_[0] ) {
        internal_so_init(bgm_[0], path);
        return 0;
    }
    else if( !bgm_[1] ) {
        internal_so_init(bgm_[1], path);
        return 1;
    }
    else {
        int track = main_track_ == 0 ? 1 : 0;
        bgm_[track]->stop();
        bgm_[track].reset();
        internal_so_init(bgm_[track], path);
        return track;
    }
}

Sound& Sound::playBGM_AB(std::string const& path_a, std::string const& path_b, time_t const& fade_t)
{
    //the playBGM API is strictly loading only 1 track and flip (so to resume it) directly.
    //this is just for convenience usages
    loadBGM_AB(path_a, path_b);
    trackFlip(fade_t, 0);
    return *this;
}

Sound& Sound::playBGM(std::string const& path, time_t const& fade_t, int const& loop)
{
    //the playBGM API is strictly loading only 1 track and flip (so to resume it) directly.
    //this is just for convenience usages
    loadBGM(path);
    trackFlip(fade_t, loop);
    return *this;
}

Sound& Sound::seek_and_playBGM_AB(
    std::string const& pa, std::string const& pb, time_t const& seekms, time_t const& fade_t)
{
    loadBGM_AB(pa, pb);
    trackFlip(fade_t, 0, seekms);
    return *this;
}

Sound& Sound::seek_and_playBGM(
    std::string const& p, time_t const& seekms, time_t const& fade_t, int const& loop)
{
    loadBGM(p);
    trackFlip(fade_t, loop, seekms);
    return *this;
}

// actually, you should not use trackFlip outside.
// this is considered an implementation detail.
void Sound::trackFlip(time_t const& fade_t, int const& loop, int const& seek)
{
    std::cout << " flipping " << bgm_[0] << " and " << bgm_[1] << std::endl;
    if( bgm_[0] && ( bgm_[0]->is_paused() || !bgm_[0]->is_active() ) ) {
        exchange(bgm_[1], bgm_[0], fade_t, loop, seek);
        main_track_ = 0;
    }
    else if( bgm_[1] && ( bgm_[1]->is_paused() || !bgm_[1]->is_active() ) ) {
        exchange(bgm_[0], bgm_[1], fade_t, loop, seek);
        main_track_ = 1;
    }
    std::cout << " - " << bgm_[0]->is_paused() << " " << bgm_[0]->is_active() << std::endl;
    if (bgm_[1]) std::cout << " | " << bgm_[1]->is_paused() << " " << bgm_[1]->is_active() << std::endl;
}

void Sound::exchange(pSoundObject const& before, pSoundObject const& after, time_t const& t, int const& loop, int const& seek)
{
    if( before ) {
        if( before->is_playing() ) {
            before->volume(1);
            before->fade_volume(0, t);
            ctrl::EventDispatcher::i().get_timer_dispatcher("global")->subscribe(
                std::tr1::bind(&SoundObject::pause, before.get()), before, t);
        }
    }
    if( after ) {
        if( after->is_paused() ) {
            //after->rewind(); unneeded feature, pick out now
            after->resume();
            // well, you can't seek in this situation. (like if you want to resume, it's should only resume at where you paused.)
        }
        else if ( !after->is_loaded() ) {
            after->play(t, loop);
            if( seek > 0 ) after->seek(seek); //only seek if it's not zero.
        }
        after->volume(0);
        after->fade_volume(1, t);
    }
}

Sound& Sound::stopAll()
{
    BOOST_FOREACH(pSoundObject& p, sound_list_)
        p->stop();

    if( bgm_[0] ) bgm_[0]->stop();
    if( bgm_[1] ) bgm_[1]->stop();
    bgm_[0].reset();
    bgm_[1].reset();
    main_track_ = 0;

    sound_list_.clear();
    sound_streams_.clear();
    sound_buffers_.clear();
    sound_samples_.clear();

    #ifdef _SHOOTING_CUBES_ENABLE_SOUND_
    detail2::sound_channel_restore();
    #endif
    std::cout << " Sound: all stopped, and all channels' volumn setting go back to 1." << std::endl;
    return *this;
}

Sound& Sound::pauseAll(bool const& f)
{
    if( f ) {
        bgm_[main_track_]->pause();
        BOOST_FOREACH(pSoundObject& p, sound_list_)
            p->pause();
    }
    else {
        bgm_[main_track_]->resume();
        BOOST_FOREACH(pSoundObject& p, sound_list_)
            p->resume();
    }
    return *this;
}

Sound& Sound::cycle()
{
    //we should not call sleep here.
    //detail::sound_update();
    #ifdef _SHOOTING_CUBES_ENABLE_SOUND_
    detail2::sound_update();
    #endif
    for(SoundList::iterator it = sound_list_.begin(), iend = sound_list_.end(); it != iend; ++it) {
        (*it)->cycle();
        if( !(*it)->is_active() ) {
            sound_to_be_cleared_.push_back(it);
        }
    }

    for( int i = 0; i < 2; ++i ) {
        if( bgm_[i] && bgm_[i]->is_loaded() ) {
            bgm_[i]->cycle();
            if( !bgm_[i]->is_active() ) {
                bgm_[i].reset();
            }
        }
    }

    BOOST_FOREACH(SoundList::iterator it, sound_to_be_cleared_)
        sound_list_.erase(it);

    sound_to_be_cleared_.clear();
    return *this;
}

void Sound::check_sound_volumes()
{
    #ifdef _SHOOTING_CUBES_ENABLE_SOUND_
    for( int i = 0; i < 16; ++i ) {
        std::cout << ALmixer_GetVolumeChannel(i) << " ";
    }
    #endif
    std::cout << std::endl;
}

Sound::~Sound()
{
    stopAll();
    #ifdef _SHOOTING_CUBES_ENABLE_SOUND_
    //detail::sound_cleanup();
    detail2::sound_cleanup();
    #endif
}
