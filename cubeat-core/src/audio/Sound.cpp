
#include "audio/Sound.hpp"
#include "audio/SoundObject.hpp"
#include "audio/detail/OpenAL.hpp"

#include <utility>
#include <boost/foreach.hpp>

#include <iostream>

using namespace psc;
using namespace audio;

Sound::Sound()
    :base_path_("rc/sound/"), inited_(false)
{
    detail::sound_init();
}

void audio::Sound::init()
{
    if( inited_ == true ) return;
    loadStream("title.mp3").loadBuffer("attack.wav"); //hack: test music
    inited_ = true;
}

Sound& Sound::loadStream(std::string const& path)
{
    std::string relative_path;// = base_path_ + path;
    relative_path = base_path_ + "test.ogg"; // THIS IS HACK
    pSoundStream new_stream = SoundStream::create(relative_path);
    sound_streams_[path] = new_stream;
    return *this;
}

Sound& Sound::loadBuffer(std::string const& path)
{
    std::string relative_path;// = base_path_ + path;
    relative_path = base_path_ + "attack.wav"; // THIS IS HACK
    pSoundBuffer new_buffer = SoundBuffer::create(relative_path);
    //you cannot use std::make_pair to increase the use_count of shared_ptr. It's probably reference.
    sound_buffers_[path] = new_buffer;
    return *this;
}

Sound& Sound::playStream(std::string const& path, bool const& loop)
{
    if( !sound_streams_[path] )   //each path name -> stream(file) is unique.
        loadStream(path);         //normally we should avoid this for big audio files.

    pSoundObject new_sound = SoundObject::create(sound_streams_[path], loop);
    sound_list_.push_back(new_sound);

    std::cout << " Sound: we have " << sound_list_.size() << " sounds playing now." << std::endl;

    return *this;
}

Sound& Sound::playBuffer(std::string const& path, bool const& loop)
{
    if( !sound_buffers_[path] )   //each path name -> stream(file) is unique.
        loadBuffer(path);         //normally we should avoid this for big audio files.

    pSoundObject new_sound = SoundObject::create(sound_buffers_[path], loop);
    sound_list_.push_back(new_sound);

    std::cout << " Sound: we have " << sound_list_.size() << " sounds playing now." << std::endl;

    return *this;
}

Sound& Sound::stopAll()
{
    BOOST_FOREACH(pSoundObject& p, sound_list_)
        p->stop();

    sound_list_.clear();
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
    detail::sound_update();
    for(SoundList::iterator it = sound_list_.begin(), iend = sound_list_.end(); it != iend; ++it) {
        if( (*it)->finished() )
            sound_to_be_cleared_.push_back(it);
    }

    BOOST_FOREACH(SoundList::iterator it, sound_to_be_cleared_)
        sound_list_.erase(it);

    sound_to_be_cleared_.clear();
    return *this;
}

Sound::~Sound()
{
    stopAll();
    sound_streams_.clear();
    sound_buffers_.clear();
    detail::sound_cleanup();
}
