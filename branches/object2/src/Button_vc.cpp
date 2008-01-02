
#include "Button.hpp"
#include "Input.hpp"

using namespace ctrl;

Button::Button(Input const* input)
    :now_(false), last_(false), state_(BTN_UP), owner_(input)
{
    //DO NOT DEREFERENCE or USE "input", "owner_" in the constructor.
}
