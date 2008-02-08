
#include "view/Game.hpp"
#include "view/Scene.hpp"
#include "view/Sprite.hpp"
#include "Input.hpp"
#include "Accessors.hpp"
#include "IrrDevice.hpp"

#include <boost/foreach.hpp>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace accessor;

using namespace psc;
using namespace view;

/* How do I implemnt this clone ?
pGame Game::clone() const
{
    return pointer_type();
}
*/
void Game::init(pScene const& world, pScene const& gui)
{
    Object::init();
    driver_ = smgr_->getVideoDriver();
    world_ = world;
    gui_   = gui;
    for( int i=0; i < ctrl::Input::count(); ++i )
        cursors_.push_back( Sprite::create("title",gui_,100,40) );
}

Game& Game::setWorld(pScene const& world) { world_ = world; return *this;}
Game& Game::setGUI(pScene const& gui)     { gui_ = gui;     return *this;}

void Game::redraw()
{
    int i = 0;
    BOOST_FOREACH(ctrl::Input* it, ctrl::Input::getInputs()) {
        cursors_[i]->moveTo(it->cursor().x(), it->cursor().y());
        ++i;
    }

    gui_->deactivate();
    world_->activate().redraw();

    world_->deactivate();
    gui_->activate().redraw();

    gui_->deactivate();
}
