
#include "audio/Sound.hpp"
#include "audio/SoundObject.hpp"
#include "audio/detail/OpenAL.hpp"
#include "audio/detail/ALmixer.hpp"

#include <utility>
#include <boost/foreach.hpp>

#include <iostream>

using namespace psc;
using namespace audio;

Sound::Sound()
    :base_path_("rc/sound/"), inited_(false)
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

pSoundObject Sound::playStream(std::string const& path, bool const& loop)
{
    // temp: HACK!!!!!
    return playSample(path, 0, loop);

//    if( !sound_streams_[path] )   //each path name -> stream(file) is unique.
//        loadStream(path);         //normally we should avoid this for big audio files.
//
//    pSoundObject new_sound = SoundObject::create(sound_streams_[path], loop);
//    sound_list_.push_back(new_sound);

    //std::cout << " Sound: we have " << sound_list_.size() << " sounds playing now." << std::endl;

    //return *this;
}

pSoundObject Sound::playSample(std::string const& path, unsigned int const& fade, bool const& loop)
{
    if( !sound_samples_[path] )   //each path name -> sample(file is unique.
        loadSample(path);         //normally we should avoid this for big audio files.

    pSoundObject new_sound = SoundObject::create(sound_samples_[path], fade, loop);
    sound_list_.push_back(new_sound);
    return new_sound;
}

pSoundObject Sound::playABStream(std::string const& path_a, std::string const& path_b)
{
    if( !sound_samples_[path_a] )
        loadSample(path_a);
    if( !sound_samples_[path_b] )
        loadSample(path_b);

    pSoundObject new_sound = SoundObject::create(sound_samples_[path_a], 0, false);
    new_sound->partB_path(path_b);
    sound_list_.push_back(new_sound);

//    if( !sound_streams_[path_a] )
//        loadStream(path_a);
//    if( !sound_streams_[path_b] )
//        loadStream(path_b);

//    pSoundObject new_sound = SoundObject::create(sound_streams_[path_a], false);
//    new_sound->partB_path(path_b);
//    sound_list_.push_back(new_sound);

    return new_sound;
}

pSoundObject Sound::playBuffer(std::string const& path, bool const& loop)
{
    if( !sound_buffers_[path] )   //each path name -> stream(file) is unique.
        loadBuffer(path);         //normally we should avoid this for big audio files.

    pSoundObject new_sound = SoundObject::create(sound_buffers_[path], loop);
    sound_list_.push_back(new_sound);

    //std::cout << " Sound: we have " << sound_list_.size() << " sounds playing now." << std::endl;

    return new_sound;
}

Sound& Sound::stopAll()
{
    BOOST_FOREACH(pSoundObject& p, sound_list_)
        p->stop();

    sound_list_.clear();
    sound_streams_.clear();
    sound_buffers_.clear();
    sound_samples_.clear();
    return *this;
}

Sound& Sound::pauseAll(bool const& f)
{
    if( f )
        BOOST_FOREACH(pSoundObject& p, sound_list_)
            p->pause();
    else
        BOOST_FOREACH(pSoundObject& p, sound_list_)
            p->resume();

    return *this;
}

Sound& Sound::cycle()
{
    //we should not call sleep here.
    //detail::sound_update();
    detail2::sound_update();
    for(SoundList::iterator it = sound_list_.begin(), iend = sound_list_.end(); it != iend; ++it) {
        if( (*it)->finished() ) {
            if( (*it)->has_partB() ) {
                pSoundObject new_sound = SoundObject::create(sound_samples_[ (*it)->partB_path() ], 0, true);
                //pSoundObject new_sound = SoundObject::create(sound_streams_[ (*it)->partB_path() ], true);
                sound_list_.push_back(new_sound);
            }
            sound_to_be_cleared_.push_back(it);
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
