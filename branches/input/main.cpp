
//Test case

#include "include/Input.hpp"
#include "include/IrrDevice.hpp"
#include "include/EventDispatcher.hpp"
#include "include/InteractiveObject.hpp"

#include <iostream>

using namespace irr;

const int FRAMERATE = 60;
int LAST_TIME = 0;

//控制 FPS 用的
bool update_block()
{
    u32 now_time = IrrDevice::i()->getTimer()->getRealTime();
    if( now_time - LAST_TIME < 1000 / FRAMERATE ) return true;
    LAST_TIME = now_time;
    return false;
}

//暫時充當 InteractiveMapView::pick() 會抓到的那個人
InteractiveObject* test = 0;

//As a button-related callback
void fptr(int x, int y)
{
    std::cout << LAST_TIME << "\n";

    //測試輸出 cursor 位置
    std::cout << x << "," << y << "\n";
    
    std::vector<InteractiveObject*> a; //picked list..
    //假設一定會抓到 test ........
    a.push_back( test );
    a[0]->hit_reaction();
}

int main()
{
    std::cout << IrrDevice::init(true) << std::endl;
    Input input1("config/input_setting_1p.yml"); 
    Input input2("config/input_setting_2p.yml"); 
    InteractiveObject testObj;

    video::IVideoDriver* driver = IrrDevice::i()->getVideoDriver();

    //註冊 button 事件的 callback，還有 picking 
    EventDispatcher::i().subscribe_btn_event(&input1.trig1(), BTN_PRESS, fptr);
    
    //暫時充當 InteractiveMapView::pick() 會抓到的那個人          
    ::test = &testObj;

    while( IrrDevice::i()->run() ) {
        if( update_block() ) continue;

        driver->beginScene(true, true, video::SColor(0,0,0,0));
        Input::update_all();
        EventDispatcher::i().dispatch();

        //explict tests
        if( input1.trig1().pressed() ) std::cout << "1-1 OK.\n";
        if( input1.trig2().pressed() ) std::cout << "1-2 OK.\n";
        if( input1.wep1().pressed() ) std::cout << "1-3 OK.\n";
        if( input1.wep2().pressed() ) std::cout << "1-4 OK.\n";
        if( input1.wep3().pressed() ) std::cout << "1-5 OK.\n";
        if( input1.haste().pressed() ) std::cout << "1-6 OK.\n";
        if( input1.pause().pressed() ) std::cout << "1-7 OK.\n";

        if( input2.trig1().pressed() ) std::cout << "2-1 OK.\n";
        if( input2.trig2().pressed() ) std::cout << "2-2 OK.\n";
        if( input2.wep1().pressed() ) std::cout << "2-3 OK.\n";
        if( input2.wep2().pressed() ) std::cout << "2-4 OK.\n";
        if( input2.wep3().pressed() ) std::cout << "2-5 OK.\n";
        if( input2.haste().pressed() ) std::cout << "2-6 OK.\n";
        if( input2.pause().pressed() ) std::cout << "2-7 OK.\n";

        core::stringw str;
        str = L"cursor1: (";
        str += input1.cursor().x();
        str += L",";
        str += input1.cursor().y();
        str += L") cursor2: (";
        str += input2.cursor().x();
        str += L",";
        str += input2.cursor().y();
        str += L")";

        IrrDevice::i()->setWindowCaption( str.c_str() );
        driver->endScene();
    }

    return 0;
}