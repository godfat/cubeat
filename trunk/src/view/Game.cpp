
/* TODO:
   Inputs are temporarily declared in this scope,
   will be removed later. (when player class is out) */

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

using namespace psc;
using namespace view;
using namespace accessor;

pGame Game::init(pScene const& world, pScene const& gui)
{
    world_ = world;
    gui_   = gui;

    //these are temporarily here.
    for( int i=0; i < ctrl::Input::count(); ++i )
        cursors_.push_back( Sprite::create("title",gui_,100,40) );

    //set them to debug true so we won't stupidly picked the cursor sprite.
    cursors_[0]->setDepth(-500).body()->setIsDebugObject(true);
    cursors_[1]->setDepth(-500).body()->setIsDebugObject(true);

    return shared_from_this();
}

Game& Game::setWorld(pScene const& world) { world_ = world; return *this; }
Game& Game::setGUI(pScene const& gui)     { gui_ = gui;     return *this; }

void Game::redraw()
{
    int i = 0;
    //these are temporarily here.
    BOOST_FOREACH(ctrl::Input* it, ctrl::Input::getInputs()) {
        cursors_[i]->moveTo(it->cursor().x()+1, it->cursor().y()+1);
        ++i;
    }

    world_->redraw();
    gui_->redraw();
}
