
#include "IrrDevice.hpp"
#include "view/Button.hpp"
#include "EventDispatcher.hpp"
#include "Button.hpp"

#include <iostream>

/* This class is nullified. Functionality moved to Sprite class */

using namespace irr;
using namespace core;
using namespace scene;

using namespace psc;
using namespace view;

pButton Button::init(pObject const& parent, int const& w, int const& h)
{
    Sprite::init(parent, w, h);
    return std::tr1::static_pointer_cast<Button>(shared_from_this());
}

Button* Button::clone() const
{
    Button* obj = new Button(*this);
    return obj;
}

Button::~Button()
{
}
