
#include "view/MapView.hpp"
#include "view/CubeView.hpp"
#include "EventDispatcher.hpp"
#include "Input.hpp"
#include "IrrDevice.hpp"

#include <boost/tr1/functional.hpp>

using namespace irr;
using namespace core;
using namespace scene;
using std::tr1::bind;

MapView* MapView::clone() const
{
    MapView* obj = new MapView(*this);
    return obj;
}

MapView& MapView::init(ObjectView const* parent)
{
    //TODO
    Input* input1 = Input::getInputByIndex(0);
    Input* input2 = Input::getInputByIndex(1);

    EventDispatcher::i().subscribe_btn_event( 
        bind(&MapView::ownerHitCallback, this, _1, _2), //(this, x, y)
             (index_ == 0 ) ? &input1->trig1() : &input2->trig1(), BTN_PRESS );

    EventDispatcher::i().subscribe_btn_event( 
        bind(&MapView::enemyHitCallback, this, _1, _2), //(this, x, y)
             (index_ == 0 ) ? &input2->trig1() : &input1->trig1(), BTN_PRESS );

    return *this;
}

MapView& MapView::addCube(CubeView* cube)
{
    node2view_.insert( std::make_pair( cube->body(), cube ) );
    cubes_.push_back( cube );
    return *this;
}

void MapView::ownerHitCallback(int x, int y)
{
    //Pick
    ISceneCollisionManager* colm = smgr_->getSceneCollisionManager();
    ISceneNode* picked = colm->getSceneNodeFromScreenCoordinatesBB(position2di(x, y), 1, true);
    
    if( CubeView* test = node2view_[picked] ) {
        test->ownerHit();
    }
}

void MapView::enemyHitCallback(int x, int y)
{
    //Pick
    ISceneCollisionManager* colm = smgr_->getSceneCollisionManager();
    ISceneNode* picked = colm->getSceneNodeFromScreenCoordinatesBB(position2di(x, y), 1, true);
    
    if( CubeView* test = node2view_[picked] ) {
        test->enemyHit();
    }
}