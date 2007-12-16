
//Test case

#include "include/Input.hpp"
#include "include/IrrDevice.hpp"
#include "include/EventDispatcher.hpp"
#include "include/GUIView.hpp"
#include "include/SpriteView.hpp"
#include "include/ButtonView.hpp"

#include <boost/bind.hpp>
#include <iostream>
#include <map>

using namespace irr;
using namespace core;
using namespace video;
using namespace scene;
using boost::bind;

const int FRAMERATE = 60;
u32 LAST_TIME = 0;

//控制 FPS 用的
bool update_block(u32 now_time)
{
    if( now_time - LAST_TIME <= 1000/FRAMERATE ) {
        return true;
    }
    LAST_TIME = now_time;
    return false;
}

std::map< ISceneNode*, ButtonView* > node2view;

//As a button-related callback
void fptr(int x, int y)
{
    //Tests
    std::cout << LAST_TIME << "\n";
    std::cout << x << "," << y << "\n";

    //Pick
    ISceneManager* smgr = IrrDevice::i()->getSceneManager();
    ISceneCollisionManager* colm = smgr->getSceneCollisionManager();
    ISceneNode* picked = colm->getSceneNodeFromScreenCoordinatesBB(position2di(x, y), 1, true);
    
    if( ButtonView* test = node2view[picked] ) {
        test->onPress();  //just a test, shouldn't be here
    }
}

void step1(SpriteView&);  //prototype
void step4(SpriteView& sprite) {
    sprite.moveTo( 0, 0, 10, 3000, bind(step1, sprite) );
}
void step3(SpriteView& sprite) {
    sprite.moveTo( 0, 420, 10, 4000, bind(step4, sprite) );
}
void step2(SpriteView& sprite) {
    sprite.moveTo( 540, 420, 10, 3000, bind(step3, sprite) );
}
void step1(SpriteView& sprite) {
    sprite.moveTo( 540, 0, 10, 4000, bind(step2, sprite) );
}

int main()
{
    std::cout << IrrDevice::init(true) << std::endl;
    Input input1("config/input_setting_1p.yml"); 
    Input input2("config/input_setting_2p.yml"); 

    IVideoDriver* driver = IrrDevice::i()->getVideoDriver();
    ISceneManager* smgr = IrrDevice::i()->getSceneManager();

    //註冊 button 事件的 callback，還有 picking 
    EventDispatcher::i().subscribe_btn_event(&input2.trig1(), BTN_PRESS, fptr);

    GUIView guiv;

    SpriteView cursor1( &guiv );
    SpriteView cursor2( &guiv );

    ButtonView something( &guiv );
    node2view.insert( std::make_pair( something.body(), &something ) );

    SpriteView anotherthing( &guiv );
    anotherthing.moveTo(0,0,10);

    bool init = false;
    ITimer* timer = IrrDevice::i()->getTimer();

    while( IrrDevice::i()->run() ) 
    if( IrrDevice::i()->isWindowActive() && !update_block( timer->getRealTime() ) )
    {
        driver->beginScene(true, true, video::SColor(0,0,0,0));

        Input::update_all();
        EventDispatcher::i().dispatch();

        smgr->drawAll();

        if( !init ) { step1(anotherthing); init = true; }

        cursor1.moveTo(input1.cursor().x(), input1.cursor().y(), 10);
        cursor2.moveTo(input2.cursor().x(), input2.cursor().y(), 10);

        driver->endScene();
    }

    return 0;
}
