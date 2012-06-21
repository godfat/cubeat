
#include "audio/Sound.hpp"
#include "audio/SoundObject.hpp"
#include "audio/detail/OpenAL.hpp"
#include "audio/detail/ALmixer.hpp"
#include "EventDispatcher.hpp"

#include <utility>
#include <boost/foreach.hpp>

#include <iostream>

using namespace psc;
using namespace audio;

Sound::Sound()
    :base_path_("rc/sound/"), inited_(false), main_track_(0)
{
    //detail::sound_init();
    detail2::sound_init();
    ALmixer_SetMasterVolume(.7f);
}

void audio::Sound::init()
{
    if( inited_ == true ) return;
    //If we have to preload some file here, then .. just do it.
    //or client code can call loadStream or loadBuffer beforehand as they see fit.
    inited_ = true;
}

Sound& Sound::loadStream(std::string const& path)
{
    std::string absolute_path = base_path_ + path;
    pSoundStream new_stream = SoundStream::create(absolute_path);
    //you cannot use std::make_pair to increase the use_count of shared_ptr. It's probably reference.
    sound_streams_[path] = new_stream;
    return *this;
}

Sound& Sound::loadBuffer(std::string const& path)
{
    std::string absolute_path = base_path_ + path;
    //pSoundBuffer new_buffer = SoundBuffer::create(absolute_path);
    pSoundSample new_buffer = SoundSample::create(absolute_path, false);
    //you cannot use std::make_pair to increase the use_count of shared_ptr. It's probably reference.
    sound_buffers_[path] = new_buffer;
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

Sound& Sound::playSample(std::string const& path, time_t const& fade_t, int const& loop)
{
    if( !sound_samples_[path] )   //each path name -> sample(file is unique.
        loadSample(path);         //normally we should avoid this for big audio files.

    pSoundObject new_sound = SoundObject::create(sound_samples_[path]);
    new_sound->play(fade_t, loop);
    sound_list_.push_back(new_sound);
    return *this;
}

Sound& Sound::loadBGM_AB(std::string const& path_a, std::string const& path_b)
{
    if( !sound_samples_[path_a] )
        loadSample(path_a);
    if( !sound_samples_[path_b] )
        loadSample(path_b);

    if( !bgm_[0] )      internal_so_init(bgm_[0], path_a, path_b);
    else if( !bgm_[1] ) internal_so_init(bgm_[1], path_a, path_b);
    else {
        int track = main_track_ == 0 ? 1 : 0;
        bgm_[track]->stop();
        bgm_[track].reset();
        internal_so_init(bgm_[track], path_a, path_b);
    }
    // not sure if the last one works... choose a not currently playing bgm track to substitute
    return *this;
}

Sound& Sound::playBGM_AB(std::string const& path_a, std::string const& path_b)
{
    //the playBGM API is strictly loading only 1 track and flip (so to resume it) directly.
    //this is just for convenience usages
    loadBGM_AB(path_a, path_b);
    trackFlip(0, 0);
    return *this;
}

void Sound::internal_so_init(pSoundObject& s, std::string const& a, std::string const& b) {
    s = SoundObject::create(sound_samples_[a]);
    if( b.size() ) s->partB(sound_samples_[b]);
}

Sound& Sound::loadBGM(std::string const& path)
{
    if( !sound_samples_[path] )
        loadSample(path);

    if( !bgm_[0] )      internal_so_init(bgm_[0], path);
    else if( !bgm_[1] ) internal_so_init(bgm_[1], path);
    else {
        int track = main_track_ == 0 ? 1 : 0;
        bgm_[track]->stop();
        bgm_[track].reset();
        internal_so_init(bgm_[track], path);
    }
    // not sure if the last one works... choose a not currently playing bgm track to substitute
    return *this;
}

Sound& Sound::playBGM(std::string const& path, int const& loop)
{
    //the playBGM API is strictly loading only 1 track and flip (so to resume it) directly.
    //this is just for convenience usages
    loadBGM(path);
    trackFlip(0, loop);
    return *this;
}

Sound& Sound::trackFlip(time_t const& fade_t, int const& loop)
{
    std::cout << " flipping " << bgm_[0] << " and " << bgm_[1] << std::endl;
    if( bgm_[0] && ( bgm_[0]->is_paused() || !bgm_[0]->is_active() ) ) {
        exchange(bgm_[1], bgm_[0], fade_t, loop);
        main_track_ = 0;
    }
    else if( bgm_[1] && ( bgm_[1]->is_paused() || !bgm_[1]->is_active() ) ) {
        exchange(bgm_[0], bgm_[1], fade_t, loop);
        main_track_ = 1;
    }
    std::cout << " - " << bgm_[0]->is_paused() << " " << bgm_[0]->is_active() << std::endl;
    if (bgm_[1]) std::cout << " | " << bgm_[1]->is_paused() << " " << bgm_[1]->is_active() << std::endl;
    return *this;
}

void Sound::exchange(pSoundObject const& before, pSoundObject const& after, time_t const& t, int const& loop)
{
    if( before && before->is_playing() ) {
        before->volume(1);
        before->fade_volume(0, t);
        ctrl::EventDispatcher::i().get_timer_dispatcher("global")->subscribe(
            std::tr1::bind(&SoundObject::pause, before.get()), t);
    }
    if( after && ( after->is_paused() || !after->is_active() ) ) {
        if( after->is_paused() ) {
            after->rewind();
            after->resume();
        }
        else if ( !after->is_active() ) {
            after->play(t, loop);
        }
        after->volume(0);
        after->fade_volume(1, t);
    }
}

//deprecated API: use playSample instead in the future.
//kept for compatibility
Sound& Sound::playBuffer(std::string const& path, bool const& loop)
{
    if( !sound_buffers_[path] )   //each path name -> stream(file) is unique.
        loadBuffer(path);         //normally we should avoid this for big audio files.

    pSoundObject new_sound = SoundObject::create(sound_buffers_[path]);
    new_sound->play(0, loop?-1:0);
    sound_list_.push_back(new_sound);

    return *this;
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

    detail2::sound_channel_restore();
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
    detail2::sound_update();
    for(SoundList::iterator it = sound_list_.begin(), iend = sound_list_.end(); it != iend; ++it) {
        (*it)->cycle();
        if( !(*it)->is_active() ) {

            //move this part to inside of SoundObject::cycle();

//            if( (*it)->has_partB() ) {
//                pSoundObject new_sound = SoundObject::create(sound_samples_[ (*it)->partB_path() ], 0, true);
//                //pSoundObject new_sound = SoundObject::create(sound_streams_[ (*it)->partB_path() ], true);
//                sound_list_.push_back(new_sound);
//            }
            sound_to_be_cleared_.push_back(it);
        }
    }

    for( int i = 0; i < 2; ++i ) {
        if( bgm_[i] ) {
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

Sound::~Sound()
{
    stopAll();
    //detail::sound_cleanup();
    detail::sound_cleanup();
}
