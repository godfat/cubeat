
#include "view/Map.hpp"
#include "view/Cube.hpp"
#include "view/Scene.hpp"
#include "EventDispatcher.hpp"
#include "Input.hpp"
#include "IrrDevice.hpp"

#include <boost/foreach.hpp>
#include <tr1/functional>
#include <iostream>

using namespace irr;
using namespace core;
using namespace scene;

using namespace psc;
using namespace view;
using std::tr1::bind;
using namespace std::tr1::placeholders;

Map* Map::clone() const
{
    Map* obj = new Map(*this);
    return obj;
}

pMap Map::init(pScene const& parent)
{
    /* TODO-FIXME:
       No, you can't put picking and node2view conversion here.
       Too many restrictions. Better move to Player class or Scene class
       when I got one. */
    setupSceneAndManager(parent);

    int i = 0;
    BOOST_FOREACH(ctrl::Input* it, ctrl::Input::getInputs()) {
        if( index_ == i ) {
            ctrl::EventDispatcher::i().subscribe_btn_event(
                bind(&Map::ownerHitCallback, this, _1, _2), //(this, x, y)
                &it->trig1(), ctrl::BTN_PRESS );
        } else {
            ctrl::EventDispatcher::i().subscribe_btn_event(
                bind(&Map::enemyHitCallback, this, _1, _2), //(this, x, y)
                &it->trig1(), ctrl::BTN_PRESS );
        }
        ++i;
    }

    body_ = smgr_->addEmptySceneNode( parent?parent->body():0 );
    parent_ = parent;

    return shared_from_this();
}

Map& Map::addCube(pCube cube)
{
    //Ok, we added cubes here, but when cubes must be reallocated,
    //how to do it? Of course linear-search and then erase will do,
    //but there must be a faster and cleaner way?

    node2view_.insert( std::make_pair( cube->body(), cube ) );
    cubes_.push_back( cube );
    return *this;
}

void Map::ownerHitCallback(int x, int y)
{
    //Pick
    ISceneCollisionManager* colm = smgr_->getSceneCollisionManager();
    ISceneNode* picked = colm->getSceneNodeFromScreenCoordinatesBB(position2di(x, y), 1, true);

    std::cout << "Map Space trace: " << (picked?picked->getName():"0") << " is " << (picked?(picked->isVisible()?"visible":"invisible"):"none") << std::endl;

    if( picked )
        if( pCube test = node2view_[picked] )
            test->ownerHit();
}

void Map::enemyHitCallback(int x, int y)
{
    //Pick
    ISceneCollisionManager* colm = smgr_->getSceneCollisionManager();
    ISceneNode* picked = colm->getSceneNodeFromScreenCoordinatesBB(position2di(x, y), 1, true);

    std::cout << "Map Space trace: " << (picked?picked->getName():"0") << " is " << (picked?(picked->isVisible()?"visible":"invisible"):"none") << std::endl;

    if( picked )
        if( pCube test = node2view_[picked] )
            test->enemyHit();
}
