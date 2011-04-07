
#include "audio/Sound.hpp"
#include "audio/SoundObject.hpp"
#include "audio/detail/OpenAL.hpp"

#include <utility>
#include <boost/foreach.hpp>

using namespace psc;
using namespace audio;

Sound::Sound()
    :base_path_("rc/sound/"), inited_(false)
{
}

void audio::Sound::init()
{
    if( inited_ == true ) return;
    load("test.ogg"); //test
}

Sound& Sound::load(std::string const& path)
{
    std::string relative_path = base_path_ + path;
    SoundObject* new_sound = new SoundObject(relative_path);
    sound_base_.insert( std::make_pair(path, new_sound) );
    return *this;
}

Sound& Sound::play(std::string const& path, bool const& loop)
{
    if( !sound_base_[path] ) //each path name is unique.
        load(path); //normally we should avoid this for big audio files.

    sound_base_[path]->play(loop);
    return *this;
}

Sound& Sound::stopAll()
{
    BOOST_FOREACH(SoundMapPair& p, sound_base_) {
        p.second->stop();
    }
    return *this;
}

Sound& Sound::pauseAll(bool const& f)
{
    BOOST_FOREACH(SoundMapPair& p, sound_base_) {
        p.second->pause();
    }
    return *this;
}

Sound& Sound::cycle()
{
    //we should not call sleep here.
    detail::sound_update();
    return *this;
}

Sound::~Sound()
{
    BOOST_FOREACH(SoundMapPair& p, sound_base_) {
        delete p.second;
    }
    detail::sound_cleanup();
}
