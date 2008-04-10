
#include "Sound.hpp"
#include <irrKlang.h>

using namespace psc;
using namespace irr;
using namespace audio;

Sound::Sound()
    :base_path_("rc/sound/")
{
    engine_ = createIrrKlangDevice();
}

void Sound::init()
{
}

void Sound::play(std::string const& path, bool loop)
{
    engine_->play2D((base_path_ + path).c_str(), loop);
}

Sound::~Sound()
{
    engine_->drop();
}
