
#ifdef _SHOOTING_CUBES_ENABLE_SOUND_

#include "audio/detail/ALmixer.hpp"
#include <cstdio>

#include <ALmixer.h>

namespace psc {
namespace audio {
namespace detail2 {

bool sound_init()
{
    if( !ALmixer_Init(ALMIXER_DEFAULT_FREQUENCY,
                      ALMIXER_DEFAULT_NUM_SOURCES,
                      ALMIXER_DEFAULT_REFRESH) ) {
        fprintf(stderr, "OpenAL (Using ALmixer) initialization failed: %s\n", ALmixer_GetError());
        return false;
    }
    ALmixer_AllocateChannels(32); //default to 32 channels
    fprintf(stdout, "OpenAL (Using ALmixer) initialized.\n");
    return true;
}

void sound_channel_restore()
{
    int n = ALmixer_CountTotalChannels();
    for( int i = 0; i < n; ++i ) {
        if( !ALmixer_IsActiveChannel(i) ) { //restore all non-active channel
            ALmixer_SetVolumeChannel(i, 1);
        }
    }
}

void sound_update()
{
    ALmixer_Update();
}

bool sound_cleanup()
{
    ALmixer_Quit();
    fprintf(stdout, "OpenAL (Using ALmixer) destructed.\n");
    return true;
}

} //detail2
} //audio
} //psc

#endif // _SHOOTING_CUBES_ENABLE_SOUND_

