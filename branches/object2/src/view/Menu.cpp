
#include "view/Menu.hpp"
#include "IrrDevice.hpp"

using namespace view;

Menu* Menu::clone() const
{
    Menu* obj = new Menu(*this);
    return obj;
}

void Menu::init(Object const* parent)
{
    body_ = smgr_->addEmptySceneNode( parent?parent->body():0 );
}
