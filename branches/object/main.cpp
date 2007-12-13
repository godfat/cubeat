
//Test case

#include "include/Input.hpp"
#include "include/IrrDevice.hpp"
#include "include/EventDispatcher.hpp"
#include "include/GUIView.hpp"
#include "include/SpriteView.hpp"
#include "include/ButtonView.hpp"

#include <iostream>
#include <map>

using namespace irr;
using namespace core;
using namespace video;
using namespace scene;

const int FRAMERATE = 60;
int LAST_TIME = 0;

//控制 FPS 用的
bool update_block()
{
    u32 now_time = IrrDevice::i()->getTimer()->getRealTime();
    if( now_time - LAST_TIME < 1000 / FRAMERATE ) {
        //should sleep here
        return true;
    }
    LAST_TIME = now_time;
    return false;
}

std::map< ISceneNode*, ButtonView* > node2view;

//As a button-related callback
void fptr(int x, int y)
{
    std::cout << LAST_TIME << "\n";

    //測試輸出 cursor 位置
    std::cout << x << "," << y << "\n";
  
    std::vector<ButtonView*> a; //picked list..

    //抓看看 test ........
    ISceneManager* smgr = IrrDevice::i()->getSceneManager();
    ISceneCollisionManager* colm = smgr->getSceneCollisionManager();

    ISceneNode* picked = colm->getSceneNodeFromScreenCoordinatesBB(position2d<s32>(x, y), 1, true);
    
    if( ButtonView* test = node2view[picked] ) {
        a.push_back( test );
        a[0]->onPress();  //just a test, shouldn't be here
    }
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
    node2view.insert( make_pair( something.body(), &something ) );

    SpriteView anotherthing( &guiv );
    anotherthing.moveTo(0,0,10);

    bool init = false;

    while( IrrDevice::i()->run() ) {
        if( update_block() ) continue;

        driver->beginScene(true, true, video::SColor(0,0,0,0));
        Input::update_all();
        EventDispatcher::i().dispatch();

        smgr->drawAll();

        if( !init ) { anotherthing.step1(); init = true; }

        cursor1.moveTo(input1.cursor().x(), input1.cursor().y(), 10);
        cursor2.moveTo(input2.cursor().x(), input2.cursor().y(), 10);

        driver->endScene();
    }

    return 0;
}