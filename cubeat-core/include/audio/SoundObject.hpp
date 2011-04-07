#ifndef _SHOOTING_CUBES_AUDIO_SOUND_OBJECT_
#define _SHOOTING_CUBES_AUDIO_SOUND_OBJECT_

#include <string>

namespace psc {
namespace audio {

class SoundObject
{
    typedef struct alureStream alureStream; //struct name from OpenAL
public:
    SoundObject(std::string const& path);
    ~SoundObject();

    SoundObject& play(bool const& loop = false);
    SoundObject& pause();
    SoundObject& stop();

private:
    alureStream* stream_;
};

} // audio
} // psc

#endif //_SHOOTING_CUBES_AUDIO_SOUND_OBJECT_


