
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

void Button::init(pObject const& parent)
{
    Sprite::init(parent);
}

Button* Button::clone() const
{
    Button* obj = new Button(*this);
    return obj;
}

Button::~Button()
{
}
