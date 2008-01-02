
#include "view/Map.hpp"
#include "view/Cube.hpp"
#include "view/Scene.hpp"
#include "EventDispatcher.hpp"
#include "Input.hpp"
#include "IrrDevice.hpp"

#include <boost/tr1/functional.hpp>
#include <iostream>

using namespace irr;
using namespace core;
using namespace scene;
using namespace view;
using std::tr1::bind;

Map* Map::clone() const
{
    Map* obj = new Map(*this);
    return obj;
}

Map& Map::init(Scene const* parent)
{
    //TODO: make these codes here looks better.
    ctrl::Input* input1 = ctrl::Input::getInputByIndex(0);
    ctrl::Input* input2 = ctrl::Input::getInputByIndex(1);

    ctrl::EventDispatcher::i().subscribe_btn_event(
        bind(&Map::ownerHitCallback, this, _1, _2), //(this, x, y)
             (index_ == 0 ) ? &input1->trig1() : &input2->trig1(), ctrl::BTN_PRESS );

    ctrl::EventDispatcher::i().subscribe_btn_event(
        bind(&Map::enemyHitCallback, this, _1, _2), //(this, x, y)
             (index_ == 0 ) ? &input2->trig1() : &input1->trig1(), ctrl::BTN_PRESS );

    body_ = smgr_->addEmptySceneNode( parent?parent->body():0 );
    parent_ = parent;

    return *this;
}

Map& Map::addCube(Cube* cube)
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
    //set to correct camera before ray picking
    ICameraSceneNode* tempcam = smgr_->getActiveCamera();
    smgr_->setActiveCamera( parent_->camera() );

    //Pick
    ISceneCollisionManager* colm = smgr_->getSceneCollisionManager();
    ISceneNode* picked = colm->getSceneNodeFromScreenCoordinatesBB(position2di(x, y), 1, true);

    if( Cube* test = node2view_[picked] ) {
        test->ownerHit();
    }

    smgr_->setActiveCamera( tempcam );
}

void Map::enemyHitCallback(int x, int y)
{
    //set to correct camera before ray picking
    ICameraSceneNode* tempcam = smgr_->getActiveCamera();
    smgr_->setActiveCamera( parent_->camera() );

    //Pick
    ISceneCollisionManager* colm = smgr_->getSceneCollisionManager();
    ISceneNode* picked = colm->getSceneNodeFromScreenCoordinatesBB(position2di(x, y), 1, true);

    if( Cube* test = node2view_[picked] ) {
        test->enemyHit();
    }

    smgr_->setActiveCamera( tempcam );
}
