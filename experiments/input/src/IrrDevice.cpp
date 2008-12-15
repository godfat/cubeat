
//A Singleton pattern for wrapping up IrrlichtDevice pointer.
//See "IrrDevice.hpp"

#include "IrrDevice.hpp"
#include "private/MastEventReceiver.hpp"

using namespace irr;
IrrlichtDevice* IrrDevice::device_ = 0;

IrrDevice::IrrDevice(bool test)
{
    if( !test )
        device_ = irr::createDevice( irr::video::EDT_NULL, 
                                     irr::core::dimension2d<irr::s32>(640,480), 
                                     16, false, false, false, &MastEventReceiver::i());
    else
        device_ = irr::createDevice( irr::video::EDT_OPENGL, 
                                     irr::core::dimension2d<irr::s32>(640,480), 
                                     16, false, false, false, &MastEventReceiver::i());
}
