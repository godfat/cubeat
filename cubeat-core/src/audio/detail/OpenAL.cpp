
#include "audio/detail/OpenAL.hpp"
#include <cstdio>
#include <AL/alure.h>

namespace psc {
namespace audio {
namespace detail {

bool sound_init()
{
    if(!alureInitDevice(NULL, NULL)) { // Do we really have hardware accelerated sound here..?
        fprintf(stderr, "OpenAL initialization failed: %s\n", alureGetErrorString());
        return false;
    }
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
