
#include "audio/SoundObject.hpp"
#include "Conf.hpp"

#include <AL/alure.h>

using namespace psc;
using namespace audio;

SoundObject::SoundObject(std::string const& path)
{
    std::string actual_path = Conf::i().expand(path);
}

SoundObject& SoundObject::play(bool const& loop)
{
    return *this;
}

SoundObject& SoundObject::pause()
{
    return *this;
}

SoundObject& SoundObject::stop()
{
    return *this;
}

SoundObject::~SoundObject()
{
}


