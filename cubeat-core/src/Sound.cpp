
#include "Sound.hpp"
#include "Conf.hpp"
#include "audio/SoundObject.hpp"

#include <boost/foreach.hpp>

using namespace psc;
using namespace audio;

Sound::Sound()
    :base_path_( Conf::i().expand("rc/sound/") ), inited_(false)
{
    //engine_ = createIrrKlangDevice();
}

void Sound::init()
{
    if( inited_ == true ) return;
}

Sound& Sound::play(std::string const& path, bool loop)
{
    //engine_->play2D((base_path_ + path).c_str(), loop);
    return *this;
}

Sound& Sound::stopAll()
{

    return *this;
}

Sound& Sound::pauseAll(bool f)
{

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
