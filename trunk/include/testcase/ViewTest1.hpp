
#ifndef _SHOOTING_CUBES_VIEWTEST1_
#define _SHOOTING_CUBES_VIEWTEST1_

/* It's quite brute that this file included everything
   and is using a lot of namespaces.

   But that's for test's sake, if you don't want to test,
   simply exclude this file from App.cpp */

#include "Input.hpp"
#include "EventDispatcher.hpp"
#include "Accessors.hpp"
#include "EasingEquations.hpp"
#include "utils/ObjectPool.hpp"
#include "view/All.hpp"

#include <boost/function.hpp>
#include <boost/lambda/lambda.hpp>
#include <tr1/functional>
#include <iostream>

using namespace psc;
using namespace ctrl;
using namespace accessor;
using namespace easing;

using std::tr1::bind;
using std::tr1::ref;
using namespace std::tr1::placeholders;

struct ViewTest1 : public std::tr1::enable_shared_from_this<ViewTest1>
{
    void step4_alt_timer(view::pSprite& sprite)
    {
        std::tr1::function<void()> next_step = bind(&ViewTest1::step1, this, ref(sprite));
        EventDispatcher::i().subscribe_timer(
            bind( &view::Sprite::moveTween, sprite.get(), 0, 0, 3000, next_step, 0), shared_from_this(), 2000);
    }

    void step4(view::pSprite& sprite) {
        sprite->moveTween( 0, 0, 3000, bind(&ViewTest1::step1, this, ref(sprite)) );
    }
    void step3(view::pSprite& sprite) {
        sprite->moveTween( 0, 420, 4000, bind(&ViewTest1::step4_alt_timer, this, ref(sprite)) );
    }
    void step2(view::pSprite& sprite) {
        namespace BLL = boost::lambda;
        boost::function<float()> func = BLL::var(speed);
        data::AnimatorParam<Linear, Pos2D> param;
        param.end(vec2(540,420)).speedfunc(func).cb( bind(&ViewTest1::step3, this, ref(sprite)) ).delay(2500);
        sprite->tween(param);
    }
    void step1(view::pSprite& sprite) {
        sprite->tween<Linear, Pos2D>( vec2(540, 0), 4000, 0, bind(&ViewTest1::step2, this, ref(sprite)) );
    }
    void glow(view::pButton& button) {
        button->tween<SineCirc, Alpha>(0, 300);
    }
    void removeFn(view::pButton& button) {
        button->onUp( &(InputMgr::i().getInputByIndex(1)->trig1()) ) = 0;
        std::cout << "pButton onUp removed.\n";
    }
    void addFn(view::pButton& button) {
        std::cout << "global button input2 trig2 press changed.\n";
        std::tr1::function<void(int,int)> btest2_ = bind(&ViewTest1::btn_press_test2, this, _1, _2);
        EventDispatcher::i().subscribe_btn_event(btest2_, &(InputMgr::i().getInputByIndex(1)->trig2()), BTN_PRESS);
        std::cout << "pButton onUp assigned.\n";
        std::tr1::function<void(view::pSprite&)> test4_ = bind(&ViewTest1::test4, this, _1);
        button->onUp( &(InputMgr::i().getInputByIndex(1)->trig1()) ) = test4_;
    }
    void addFn2(view::pButton& button) {
        std::tr1::function<void(view::pSprite&)> test3_ = bind(&ViewTest1::test3, this, _1);
        button->onUp( &(InputMgr::i().getInputByIndex(1)->trig1()) ) = test3_;
    }
    void test(view::pSprite&) {
        std::cout << "test...." << std::endl;
        speed = 20.f;
    }
    void test2(view::pSprite&) {
        std::cout << "another test...." << std::endl;
        speed = 40.f;
    }
    void test3(view::pSprite&) {
        std::cout << "yet another test...." << std::endl;
        speed = 80.f;
    }
    void test4(view::pSprite& sp) {
        std::cout << "yet yet another test...." << std::endl;
        sp->set<GradientDiffuse>(255);
    }
    void test5(view::pSprite& sp, int, int) {
        sp->set<Scale>(vec3(1.2f,1.2f,1));
    }
    void test6(view::pSprite& sp, int, int) {
        sp->set<Scale>(vec3(1,1,1));
    }
    void cube_owner_hit() {
        std::cout << "HEHEHE.\n";
    }
    void cube_enemy_hit() {
        std::cout << "HAHAHA.\n";
    }


    //AMAZING!!!!!!!! the following 4 functions work very well!

    void btn_press_test(int, int) {
        std::cout << "BLAHHHH.\n";
        std::tr1::function<void(int,int)> btest2_ = bind(&ViewTest1::btn_press_test2, this, _1, _2);
        EventDispatcher::i().subscribe_btn_event(btest2_, &(InputMgr::i().getInputByIndex(1)->trig2()), BTN_PRESS);
    }
    void btn_press_test2(int, int) {
        std::cout << "BLOHHHH.\n";
        std::tr1::function<void(int,int)> btest_ = bind(&ViewTest1::btn_press_test, this, _1, _2);
        EventDispatcher::i().subscribe_btn_event(btest_, &(InputMgr::i().getInputByIndex(1)->trig2()), BTN_PRESS);
    }
    void obj_press_test(view::pSprite& sp) {
        std::cout << "BLEHHHH.\n";
        std::tr1::function<void(view::pSprite&)> otest2_ = bind(&ViewTest1::obj_press_test2, this, _1);
        sp->onPress( &(InputMgr::i().getInputByIndex(1)->haste()) ) = otest2_; //input2's haste is middle button
        sp->setTexture("cubes/cube4");
    }
    void obj_press_test2(view::pSprite& sp) {
        std::cout << "BLIHHHH.\n";
        std::tr1::function<void(view::pSprite&)> otest_ = bind(&ViewTest1::obj_press_test, this, _1);
        sp->onPress( &(InputMgr::i().getInputByIndex(1)->haste()) ) = otest_;  //input2's haste is middle button
        sp->setTexture("title");
    }

    void init();
    void cycle();
    ViewTest1();

private:
    ViewTest1(ViewTest1 const&);

    view::pScene               guiv;
    view::pScene               worldv;
    view::pGame                game;
    view::pMap                 map1;
    view::pCube                cube0;
    view::pCube                c;
    view::pAnimatedSceneObject testcube2;
    view::pAnimatedSceneObject testcube3;
    view::pButton              something;
    view::pSprite              anotherthing;
    view::pAnimatedSprite      girl;
    view::pAnimatedSprite      a;
    view::pMenu                menu;
    view::pAnimatedSceneObject cube4;
    view::pAnimatedSceneObject palm1;
    view::pAnimatedSceneObject palm2;
    view::pAnimatedSceneObject palm3;

    float                      speed;
};

ViewTest1::ViewTest1()
{
    //member test function workaround (shortcut)
    speed = 40.f;
    std::tr1::function<void(view::pSprite&)> test_ = bind(&ViewTest1::test, this, _1);
    std::tr1::function<void(view::pSprite&)> test2_ = bind(&ViewTest1::test2, this, _1);
    std::tr1::function<void(view::pSprite&)> test3_ = bind(&ViewTest1::test3, this, _1);
    std::tr1::function<void(view::pSprite&)> test4_ = bind(&ViewTest1::test4, this, _1);
    std::tr1::function<void(view::pSprite&, int, int)> test5_ = bind(&ViewTest1::test5, this, _1, _2, _3);
    std::tr1::function<void(view::pSprite&, int, int)> test6_ = bind(&ViewTest1::test6, this, _1, _2, _3);
    std::tr1::function<void()> owner_hit_ = bind(&ViewTest1::cube_owner_hit, this);
    std::tr1::function<void()> enemy_hit_ = bind(&ViewTest1::cube_enemy_hit, this);

    guiv = view::Scene::create("ViewTest_GUI");
    guiv->setTo2DView();

    worldv = view::Scene::create("ViewTest_WORLD");
    worldv->setTo3DView(PI/5.f);

    game = view::Game::create( worldv, guiv );

    map1 = view::Map::create(0, worldv );
    map1->moveTo(-20,0,50);

    //added callback setting test
    cube0 = view::Cube::create( map1 );
    cube0->setOwnerHit( owner_hit_ );

    //added callback setting test
    c = view::Cube::create( map1 );
    c->setEnemyHit( enemy_hit_ ).
       moveTo(10,0,0).set<GradientDiffuse>(127);

    data::AnimatorParam<Linear, Frame> param1; param1.start(4801.0f).end(9600.0f).loop(-1);
    data::CirclingParam<Linear, Pos3D> param2;
    param2.center(vec3(5,5,50)).start(vec2(10,0)).end(vec2(10,360)).rotation(vec3(-45,0,0))
          .duration(5000).loop(-1);

    testcube2 = view::AnimatedSceneObject::create("ex_move", worldv);
    testcube2->moveTo(5,5,50).set<Scale>(vec3(0.3f, 0.3f, 0.3f))
                             .tween(param1).tween(param2);

    testcube3 = view::AnimatedSceneObject::create("ex_move", worldv);
    testcube3->moveTo(-5,-5,60).set<Scale>(vec3(0.3f, 0.3f, 0.3f))
                               .tween<Linear, Frame>(0.0f,9600.0f,3000, -1);

    data::CirclingParam<SineCirc, Pos2D> param3;
    param3.center(vec2(100,100)).start(vec2(30,90)).end(vec2(30,450)).duration(5000).loop(-1);

    something = view::Button::create("title", guiv, 100, 40);
    something->moveTo(100,100).set<GradientDiffuse>(200).tween(param3);

    something->onPress( &(InputMgr::i().getInputByIndex(1)->trig1()) ) = test_;
    something->onRelease( &(InputMgr::i().getInputByIndex(1)->trig1()) ) = test2_;
    something->onUp( &(InputMgr::i().getInputByIndex(1)->trig1()) ) = test4_;
    something->onEnterFocus( InputMgr::i().getInputByIndex(1) ) = test5_;
    something->onLeaveFocus( InputMgr::i().getInputByIndex(1) ) = test6_;

    anotherthing = view::Sprite::create("title", guiv, 100, 40);
    anotherthing->moveTo(0,0);

    girl = view::AnimatedSprite::create("test", guiv, 128, 192);
    girl->addAnime("stand",3).addAnime("hit",5).moveTo(400,300);
    girl->playAnime("stand", 1000, -1);

    a = view::AnimatedSprite::create( "test", guiv, 128, 192);
    a->addAnime("hit",5).playAnime("hit",876,-1).moveTo(150,350);

    //test menu
    menu = view::Menu::create("title", guiv, 100, 40);
    menu->moveTo(520, 50);

    menu->addSprite("item1",test_,100,40,false,"title").addSprite("item2",test2_,100,40,false,"title").
          addSprite("item3",test3_,100,40,false,"title");
    menu->getSprite("item1").set<Green>(0).set<Pos2D>(vec2(0, 50));
    menu->getSprite("item2").set<Blue>(0).set<Pos2D>(vec2(0, 150));
    menu->getSprite("item3").set<Red>(0).set<Pos2D>(vec2(0, 250));

    std::vector<vec3> waypoints;
    waypoints.push_back(vec3(0, 10, 60));
    waypoints.push_back(vec3(5, -5, 70));
    waypoints.push_back(vec3(0, 0, 80));

    data::AnimatorParam<SineCirc, Scale> paramA; paramA.end(vec3(.5f,.5f,.5f)).duration(2000).cb(owner_hit_);
    data::AnimatorParam<IOExpo, Pos3D>   paramB; paramB.end(vec3(0,10,60)).duration(3333).cb(enemy_hit_);
    data::WaypointParam<OCirc, Pos3D>    paramC; paramC.waypoints(waypoints).duration(3333).cb(owner_hit_);
    data::CirclingParam<Linear, Pos3D>   paramD;
    paramD.center(vec3(-3,3,83)).start(vec2(3*1.71f,-45)).end(vec2(9,135)).rotation(vec3(-90,45,0))
          .duration(4000).cb(enemy_hit_);

    cube4 = view::AnimatedSceneObject::create("ex_move", worldv);
    cube4->moveTo(0,0,80).set<Scale>(vec3(.3f, .3f, .3f))
          .queue(paramA).queue(paramB).queue(paramC).tween(paramD);

    std::tr1::function<void(int,int)> btest_ = bind(&ViewTest1::btn_press_test, this, _1, _2);
    EventDispatcher::i().subscribe_btn_event(btest_, &(InputMgr::i().getInputByIndex(1)->trig2()), BTN_PRESS);

    std::tr1::function<void(view::pSprite&)> otest_ = bind(&ViewTest1::obj_press_test, this, _1);
    anotherthing->onPress( &(InputMgr::i().getInputByIndex(1)->haste()) ) = otest_;

    data::AnimatorParam<Linear, Frame> paramAni; paramAni.end(90.f).duration(3000).loop(-1);
    palm1 = view::AnimatedSceneObject::create("jungle/palm1", worldv);
    palm1->moveTo(0,-15,48).tween(paramAni);
    palm2 = view::AnimatedSceneObject::create("jungle/palm2", worldv);
    palm2->moveTo(5,-15,48).tween(paramAni);
    palm3 = view::AnimatedSceneObject::create("jungle/palm3", worldv);
    palm3->moveTo(-5,-15,48).tween(paramAni);
}

void ViewTest1::init()
{
    EventDispatcher::i().subscribe_timer(
        bind(&ViewTest1::step1, this, ref(anotherthing)), shared_from_this(), 1000 );

    EventDispatcher::i().subscribe_timer(
        bind(&ViewTest1::glow, this, ref(something)), shared_from_this(), 600, -1 );

    EventDispatcher::i().subscribe_timer(
        bind(&ViewTest1::removeFn, this, ref(something)), shared_from_this(), 2000 );

    EventDispatcher::i().subscribe_timer(
        bind(&ViewTest1::addFn, this, ref(something)), shared_from_this(), 4000 );

    EventDispatcher::i().subscribe_timer(
        bind(&ViewTest1::addFn2, this, ref(something)), shared_from_this(), 6000 );
}

void ViewTest1::cycle()
{
    game->redraw();
}

#endif // _SHOOTING_CUBES_VIEWTEST1_
