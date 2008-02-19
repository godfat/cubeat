
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

#include <boost/tr1/functional.hpp>
#include <iostream>

using namespace psc;
using namespace ctrl;
using namespace accessor;
using namespace easing;

using std::tr1::bind;
using std::tr1::ref;
using namespace std::tr1::placeholders;

struct ViewTest1
{
    void step4_alt_timer(view::pSprite& sprite)
    {
        std::tr1::function<void()> next_step = bind(&ViewTest1::step1, this, ref(sprite));
        EventDispatcher::i().subscribe_timer( bind( &view::Sprite::moveTween,
            sprite.get(), 0, 0, 3000, next_step, 0), 2000);
    }

    void step4(view::pSprite& sprite) {
        sprite->moveTween( 0, 0, 3000, bind(&ViewTest1::step1, this, ref(sprite)) );
    }
    void step3(view::pSprite& sprite) {
        sprite->moveTween( 0, 420, 4000, bind(&ViewTest1::step4_alt_timer, this, ref(sprite)) );
    }
    void step2(view::pSprite& sprite) {
        sprite->moveTween( 540, 420, 3000, bind(&ViewTest1::step3, this, ref(sprite)), 2500 );
    }
    void step1(view::pSprite& sprite) {
        sprite->moveTween( 540, 0, 4000, bind(&ViewTest1::step2, this, ref(sprite)) );
    }
    void glow(view::pButton& button) {
        button->tween<SineCirc, Alpha>(0, 300, false);
    }
    void test(view::pSprite&) {
        std::cout << "test...." << std::endl;
    }
    void test2(view::pSprite&) {
        std::cout << "another test...." << std::endl;
    }
    void test3(view::pSprite&) {
        std::cout << "yet another test...." << std::endl;
    }
    void cube_owner_hit() {
        std::cout << "HEHEHE.\n";
    }
    void cube_enemy_hit() {
        std::cout << "HAHAHA.\n";
    }

    void cycle();

    static ViewTest1& i() {
        static ViewTest1 test;
        return test;
    }

private:
    ViewTest1();
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
};

ViewTest1::ViewTest1()
{
    //member test function workaround (shortcut)
    std::tr1::function<void(view::pSprite&)> test_ = bind(&ViewTest1::test, this, _1);
    std::tr1::function<void(view::pSprite&)> test2_ = bind(&ViewTest1::test2, this, _1);
    std::tr1::function<void(view::pSprite&)> test3_ = bind(&ViewTest1::test3, this, _1);

    guiv = view::Scene::create(view::pObject(), "ViewTest_GUI");
    guiv->setTo2DView();

    worldv = view::Scene::create(view::pObject(), "ViewTest_WORLD");
    worldv->setTo3DView( PI/3.0f );

    game = view::Game::create( worldv, guiv );

    map1 = view::Map::create(0, worldv );
    map1->moveTo(-20,0,50);

    //added callback setting test
    cube0 = view::Cube::create( map1 );
    cube0->setOwnerHit( bind(&ViewTest1::cube_owner_hit, this) );

    //added callback setting test
    c = view::Cube::create( map1 );
    c->setEnemyHit( bind(&ViewTest1::cube_enemy_hit, this) ).
       moveTo(10,0,0).set<GradientDiffuse>(127);

    testcube2 = view::AnimatedSceneObject::create("ex_move", worldv);
    testcube2->moveTo(5,5,50).set<Scale>(vec3(0.3f, 0.3f, 0.3f))
                             .tween<Linear, Frame>(4801.0f,9600.0f,1500);

    testcube3 = view::AnimatedSceneObject::create("ex_move", worldv);
    testcube3->moveTo(-5,-5,60).set<Scale>(vec3(0.3f, 0.3f, 0.3f))
                               .tween<Linear, Frame>(0.0f,9600.0f,3000);

    something = view::Button::create("title", guiv, 100, 40);
    something->moveTo(100,100).set<GradientDiffuse>(200);

    something->onPress( &(Input::getInputByIndex(1)->trig1()) ) = test_;

    anotherthing = view::Sprite::create("title", guiv, 100, 40);
    anotherthing->moveTo(0,0);

    girl = view::AnimatedSprite::create("test", guiv, 128, 192);
    girl->addAnime("stand",3).addAnime("hit",5).moveTo(400,300);
    girl->playAnime("stand", 1000, true);

    a = view::AnimatedSprite::create( "test", guiv, 128, 192);
    a->addAnime("hit",5).playAnime("hit",876,true).moveTo(150,350);

    //test menu
    menu = view::Menu::create("title", guiv, 100, 40);
    menu->moveTo(520, 50);

    menu->addSprite("item1",test_,100,40,"title").addSprite("item2",test2_,100,40,"title").
          addSprite("item3",test3_,100,40,"title");
    menu->getSprite("item1").set<Green>(0).set<Pos2D>(vec2(0, -50));
    menu->getSprite("item2").set<Blue>(0).set<Pos2D>(vec2(0, -150));
    menu->getSprite("item3").set<Red>(0).set<Pos2D>(vec2(0, -250));

    EventDispatcher::i().subscribe_timer(
        bind(&ViewTest1::step1, this, ref(anotherthing)), 1000 );

    EventDispatcher::i().subscribe_timer(
        bind(&ViewTest1::glow, this, ref(something)), 600, true );
}

void ViewTest1::cycle()
{
    game->redraw();
}

#endif // _SHOOTING_CUBES_VIEWTEST1_
