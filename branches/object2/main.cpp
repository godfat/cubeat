
//Test case: I WILL NOT USE CHINESE IN COMMENT AGAIN..
//windows utf8 is so troublesome.

#include "Input.hpp"
#include "IrrDevice.hpp"
#include "EventDispatcher.hpp"
#include "view/All.hpp"

#include "Accessors.hpp"
#include "EasingEquations.hpp"
#include "utils/ObjectPool.hpp"

#include <boost/tr1/functional.hpp>
#include <iostream>
#include <map>

using namespace psc;
using namespace irr;
using namespace core;
using namespace video;
using namespace scene;
using namespace easing;
using namespace accessor;
using namespace ctrl;

using std::tr1::bind;
using std::tr1::ref;

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

/*
I changed the method name of overloaded View::moveTo method which has "tween effect"
to View::moveTween, the reason I give up the advantage of function overloading is
it is too troublesome to use boost/tr1 bind with them. The code would have to be like:

using boost::mem_fn;

std::tr1::function<void()> next_step = bind(step1, sprite);

EventDispatcher::i().subscribe_timer( bind( mem_fn(
    static_cast<void(Sprite::*)(int, int, int, std::tr1::function<void()>, int)>
        (&Sprite::moveTo) ), &sprite, 0, 0, 3000, next_step, 0), 2000);
                              ^mem_fn                             ^bind  ^subscribe_timer

I was not sure that mem_fn was necessary or not, anyway it worked.
but NOBODY can stand a cast like that.
*/

void step1(view::Sprite&);  //prototype

void step4_alt_timer(view::Sprite& sprite)
{
    std::tr1::function<void()> next_step = bind(step1, ref(sprite));
    EventDispatcher::i().subscribe_timer( bind( &view::Sprite::moveTween,
        &sprite, 0, 0, 3000, next_step, 0), 2000);
}

void step4(view::Sprite& sprite) {
    sprite.moveTween( 0, 0, 3000, bind(step1, ref(sprite)) );
}
void step3(view::Sprite& sprite) {
    sprite.moveTween( 0, 420, 4000, bind(step4_alt_timer, ref(sprite)) );
}
void step2(view::Sprite& sprite) {
    sprite.moveTween( 540, 420, 3000, bind(step3, ref(sprite)), 2500 );
}
void step1(view::Sprite& sprite) {
    sprite.moveTween( 540, 0, 4000, bind(step2, ref(sprite)) );
}

void glow(view::Button& button) {
    button.tween<SineCirc, Alpha>(0, 500, false);
}

void test(view::Sprite*) {
    std::cout << "test...." << std::endl;
}

int main()
{
    std::cout << IrrDevice::init(true) << std::endl;
    Input input1("config/input_setting_1p.yml");
    Input input2("config/input_setting_2p.yml");

    IVideoDriver* driver = IrrDevice::i()->getVideoDriver();
    ISceneManager* smgr = IrrDevice::i()->getSceneManager();

    view::Scene guiv;
    guiv.init();
    guiv.setTo2DView(640,480);

    view::Scene worldv;
    worldv.init();
    worldv.setTo3DView( PI/3.0f );

    view::Map map1(0);
    map1.init( &worldv );
    map1.moveTo(-20,0,50);

    view::Cube cube0;
    cube0.init( &map1 );

    view::pCube c = view::Cube::create( &map1 );
    c->moveTo(10,0,0).set<RGBDiffuse>(20);

    view::Sprite cursor1; cursor1.init( &guiv );
    view::Sprite cursor2; cursor2.init( &guiv );

    view::Button something; something.init( &guiv );
    something.moveTo(100,100).set<RGBDiffuse>(192);

    //EventDispatcher::i().subscribe_obj_event(test, &input2.trig1(), &something);
    //A new testing interface equivalent to the above line.
    something.onPress( &input2.trig1() ) = test;

    view::Sprite anotherthing; anotherthing.init( &guiv );
    anotherthing.moveTo(0,0);

    //newly added
    view::AnimatedSprite girl; girl.init( "test", &guiv );
    girl.addAnime("stand",3).addAnime("hit",5).moveTo(400,300).set<Size2D>(dimension2df(128,192));
    girl.playAnime("stand", 1000, true);

    view::pAnimatedSprite a = view::AnimatedSprite::create( "test", &guiv );
    a->addAnime("hit",5).playAnime("hit",876,true).moveTo(150,350).set<Size2D>(dimension2df(128,192));

    EventDispatcher::i().subscribe_timer( bind(step1, ref(anotherthing)), 1000 );
    EventDispatcher::i().subscribe_timer( bind(glow, ref(something)), 1500, true );

    ITimer* timer = IrrDevice::i()->getTimer();

    while( IrrDevice::i()->run() )
        if( IrrDevice::i()->isWindowActive() && !update_block( timer->getRealTime() ) )
        {
            Input::update_all();
            EventDispatcher::i().dispatch();

            driver->beginScene(true, true, video::SColor(0,64,64,64));

            //TODO: contain these different phase of drawing into GameView.
            guiv.deactivate();
            worldv.activate();
            smgr->drawAll();

            worldv.deactivate();
            guiv.activate();
            smgr->drawAll();

            cursor1.moveTo(input1.cursor().x(), input1.cursor().y());
            cursor2.moveTo(input2.cursor().x(), input2.cursor().y());

            driver->endScene();
        }

    return 0;
}
