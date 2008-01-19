
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
        device_ = createDevice( irr::video::EDT_NULL,
                                irr::core::dimension2d<irr::s32>(640,480),
                                32, false, false, false, &MastEventReceiver::i());
    else
        device_ = createDevice( irr::video::EDT_OPENGL,
                                irr::core::dimension2d<irr::s32>(640,480),
                                32, false, false, false, &MastEventReceiver::i());
    return device_ ? true : false;
}
