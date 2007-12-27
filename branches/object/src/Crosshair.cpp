
#include "include/Crosshair.hpp"
#include "include/IrrDevice.hpp"

Crosshair::Crosshair(Input const* input)
    :x_(0), y_(0), lx_(0), ly_(0), leftbound_(0), rightbound_(640),
     topbound_(0), bottombound_(480), owner_(input)
{
    //DO NOT DEREFERENCE or USE "input", "owner_" in the constructor.
    bottombound_ = IrrDevice::i()->getVideoDriver()->getScreenSize().Height;
    rightbound_ = IrrDevice::i()->getVideoDriver()->getScreenSize().Width;
}

void Crosshair::constrain()
{
    if( x_ < leftbound_ )       x_ = leftbound_;
    else if( x_ > rightbound_ ) x_ = rightbound_;
    if( y_ < topbound_ )        y_ = topbound_;
    else if( y_ > bottombound_) y_ = bottombound_;
}
