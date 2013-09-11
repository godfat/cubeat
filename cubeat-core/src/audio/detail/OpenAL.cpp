
#ifdef _SHOOTING_CUBES_ENABLE_SOUND_

#include "audio/detail/OpenAL.hpp"
#include <cstdio>

#if defined(__APPLE__) || defined(__MACOSX__)
#include <OpenAL/alure.h>
#else
#include <AL/alure.h>
#endif

namespace psc {
namespace audio {
namespace detail {

bool sound_init()
{
    if(!alureInitDevice(NULL, NULL)) { // Do we really have hardware accelerated sound here..?
        fprintf(stderr, "OpenAL initialization failed: %s\n", alureGetErrorString());
        return false;
    }
    alureStreamSizeIsMicroSec(AL_TRUE);
    fprintf(stdout, "OpenAL initialized.\n");
    return true;
}

void sound_update()
{
    alureUpdate();
}

bool sound_cleanup()
{
    if(!alureShutdownDevice()) {
        fprintf(stderr, "OpenAL destruction failed: %s\n", alureGetErrorString());
        return false;
    }
    fprintf(stdout, "OpenAL destructed.\n");
    return true;
}

} //detail
} //audio
} //psc

#endif // _SHOOTING_CUBES_ENABLE_SOUND_
