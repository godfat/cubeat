
#include "Crosshair.hpp"
#include "IrrDevice.hpp"
#include "Conf.hpp"

using namespace psc;
using namespace ctrl;

Crosshair::Crosshair(Input const* input)
    :x_(0), y_(0), lx_(0), ly_(0), leftbound_(0), rightbound_( Conf::i().SCREEN_W() ),
     topbound_(0), bottombound_( Conf::i().SCREEN_H() ), owner_(input)
{
    //DO NOT DEREFERENCE or USE "input", "owner_" in the constructor.
}

void Crosshair::constrain()
{
    if( x_ < leftbound_ )       x_ = leftbound_;
    else if( x_ > rightbound_ ) x_ = rightbound_;
    if( y_ < topbound_ )        y_ = topbound_;
    else if( y_ > bottombound_) y_ = bottombound_;
}
