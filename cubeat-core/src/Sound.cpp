
#include "Sound.hpp"
#include <irrKlang.h>

using namespace psc;
using namespace irrklang;

Sound::Sound()
    :base_path_("rc/sound/")
{
    engine_ = createIrrKlangDevice();
}

void Sound::init()
{
}

Sound& Sound::play(std::string const& path, bool loop)
{
    engine_->play2D((base_path_ + path).c_str(), loop);
    return *this;
}

Sound& Sound::stopAll()
{
    engine_->stopAllSounds();
    return *this;
}

Sound& Sound::pauseAll(bool f)
{
    engine_->setAllSoundsPaused(f);
    return *this;
}

Sound::~Sound()
{
    engine_->drop();
}
