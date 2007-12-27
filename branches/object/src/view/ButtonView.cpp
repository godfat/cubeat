
#include "IrrDevice.hpp"
#include "view/ButtonView.hpp"
#include "view/GUIView.hpp"

#include <iostream>

using namespace irr;
using namespace core;
using namespace scene;
/*
ButtonView::ButtonView(GUIView const* parent)
    :SpriteView(parent)
{
}
*/

ButtonView* ButtonView::clone() const
{
    ButtonView* obj = new ButtonView(*this);
    return obj;
}

void ButtonView::onPress()
{
    std::cout << "Something Clicked.\n";
}

ButtonView::~ButtonView()
{
}
