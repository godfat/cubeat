
//Test case: I WILL NOT USE CHINESE IN COMMENT AGAIN..
//windows utf8 is so troublesome.

#include "include/Input.hpp"
#include "include/IrrDevice.hpp"
#include "include/EventDispatcher.hpp"
#include "include/GUIView.hpp"
#include "include/SpriteView.hpp"
#include "include/ButtonView.hpp"

#include "include/Accessors.hpp"
#include "include/EasingEquations.hpp"

#include <boost/bind.hpp>
#include <iostream>
#include <map>

using namespace irr;
using namespace core;
using namespace video;
using namespace scene;
using namespace easing;
using namespace accessor;
using boost::bind;

const int FRAMERATE = 60;
u32 LAST_TIME = 0;

//FPS constraint
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

/*
I changed the method name of overloaded View::moveTo method which has "tween effect"
to View::moveTween, the reason I give up the advantage of function overloading is
it is too troublesome to use boost/tr1 bind with them. The code would have to be like:

using boost::mem_fn;

boost::function<void()> next_step = bind(step1, sprite);

EventDispatcher::i().subscribe_timer( bind( mem_fn( 
    static_cast<void(SpriteView::*)(int, int, int, boost::function<void()>, int)>
        (&SpriteView::moveTo) ), &sprite, 0, 0, 3000, next_step, 0), 2000);
                              ^mem_fn                             ^bind  ^subscribe_timer

I was not sure that mem_fn was necessary or not, anyway it worked.
but NOBODY can stand a cast like that.
*/

void step1(SpriteView&);  //prototype

void step4_alt_timer(SpriteView& sprite)
{
    boost::function<void()> next_step = bind(step1, sprite);
    EventDispatcher::i().subscribe_timer( bind( &SpriteView::moveTween,
        &sprite, 0, 0, 3000, next_step, 0), 2000);
}

void step4(SpriteView& sprite) {
    sprite.moveTween( 0, 0, 3000, bind(step1, sprite) );
}
void step3(SpriteView& sprite) {
    sprite.moveTween( 0, 420, 4000, bind(step4_alt_timer, sprite) );
}
void step2(SpriteView& sprite) {
    sprite.moveTween( 540, 420, 3000, bind(step3, sprite), 2500 );
}
void step1(SpriteView& sprite) {
    sprite.moveTween( 540, 0, 4000, bind(step2, sprite) );
}

void glow(ButtonView& button) {
    button.tween<SineCirc, RGBEmissive>(0, 500, false);
}

int main()
{
    std::cout << IrrDevice::init(true) << std::endl;
    Input input1("config/input_setting_1p.yml"); 
    Input input2("config/input_setting_2p.yml"); 

    IVideoDriver* driver = IrrDevice::i()->getVideoDriver();
    ISceneManager* smgr = IrrDevice::i()->getSceneManager();

    //subscribe a button click related callback
    EventDispatcher::i().subscribe_btn_event(fptr, &input2.trig1(), BTN_PRESS);

    GUIView guiv;

    SpriteView cursor1( &guiv );
    SpriteView cursor2( &guiv );

    ButtonView something( &guiv );
    node2view.insert( std::make_pair( something.body(), &something ) );

    SpriteView anotherthing( &guiv );
    anotherthing.moveTo(0,0);

    bool init = false;
    ITimer* timer = IrrDevice::i()->getTimer();

    while( IrrDevice::i()->run() ) 
    if( IrrDevice::i()->isWindowActive() && !update_block( timer->getRealTime() ) )
    {
        driver->beginScene(true, true, video::SColor(0,64,64,64));

        Input::update_all();
        EventDispatcher::i().dispatch();

        smgr->drawAll();

        if( !init ) { 
            EventDispatcher::i().subscribe_timer( bind(step1, anotherthing), 1000 );
            EventDispatcher::i().subscribe_timer( bind(glow, something), 1000, true );
            init = true; 
        }

        cursor1.moveTo(input1.cursor().x(), input1.cursor().y());
        cursor2.moveTo(input2.cursor().x(), input2.cursor().y());

        driver->endScene();
    }

    return 0;
}
