
//A Singleton pattern for wrapping up IrrlichtDevice pointer.
//See "IrrDevice.hpp"

#include "IrrDevice.hpp"
#include "private/MastEventReceiver.hpp"

using namespace irr;

bool IrrDevice::inited_ = false;

bool IrrDevice::init(bool test)
{
    if( inited_ ) return false;
    inited_ = true;
    if( !test )
        device_ = createDevice( video::EDT_NULL,
                                core::dimension2d<s32>(640,480),
                                32, false, false, false, &MastEventReceiver::i());
    else
        device_ = createDevice( video::EDT_OPENGL,
                                core::dimension2d<s32>(640,480),
                                32, false, false, false, &MastEventReceiver::i());
    return device_ ? true : false;
}
