#include "Accessors.hpp"
#include "Input.hpp"
#include "view/Scene.hpp"
#include "view/Sprite.hpp"
#include "presenter/Stage.hpp"
#include "EventDispatcher.hpp"
#include "utils/Random.hpp"
#include "utils/dictionary.hpp"
#include "Conf.hpp"

using namespace psc;
using namespace ctrl;
using namespace accessor;

class TestStage{
    typedef std::vector< view::pAnimatedSceneObject > SceneObjList;

public:
    TestStage(){
        //temporary Scene:

        utils::map_any stage = Conf::i().config_of("test_stage");
        stage_ = presenter::Stage::create( stage.S("test_stage") );
        stage_->playBGM();

        scene_ = view::Scene::create("test scene");
        scene_->setTo2DView();

        sprite_= view::Sprite::create("char1/test", scene_, 384, 384);
        sprite_->moveTo(0, 200);

        button_01_ = view::Sprite::create("cubes/cube1", scene_, 64, 64);
        button_01_->moveTo(800, 100);
        button_02_ = view::Sprite::create("cubes/cube1", scene_, 64, 64);
        button_02_->moveTo(800, 180);
        button_03_ = view::Sprite::create("cubes/cube1", scene_, 64, 64);
        button_03_->moveTo(800, 260);
        button_04_ = view::Sprite::create("cubes/cube1", scene_, 64, 64);
        button_04_->moveTo(800, 340);
        button_05_ = view::Sprite::create("cubes/cube1", scene_, 64, 64);
        button_05_->moveTo(800, 420);
        button_06_ = view::Sprite::create("cubes/cube1", scene_, 64, 64);
        button_06_->moveTo(800, 500);
        button_07_ = view::Sprite::create("cubes/cube-b-1", scene_, 64, 64);
        button_07_->moveTo(880, 100);
        button_08_ = view::Sprite::create("cubes/cube-b-1", scene_, 64, 64);
        button_08_->moveTo(880, 180);
        button_09_ = view::Sprite::create("cubes/cube-b-1", scene_, 64, 64);
        button_09_->moveTo(880, 260);
        button_10_ = view::Sprite::create("cubes/cube-b-1", scene_, 64, 64);
        button_10_->moveTo(880, 340);
        button_11_ = view::Sprite::create("cubes/cube-b-1", scene_, 64, 64);
        button_11_->moveTo(880, 420);
        button_12_ = view::Sprite::create("cubes/cube-b-1", scene_, 64, 64);
        button_12_->moveTo(880, 500);

        std::tr1::function<void(view::pSprite&)> moveRight      = bind(&TestStage::onMoveRight, this, _1);
        std::tr1::function<void(view::pSprite&)> moveLeft       = bind(&TestStage::onMoveLeft, this, _1);
        std::tr1::function<void(view::pSprite&)> selectChar1    = bind(&TestStage::onSelectChar1, this, _1);
        std::tr1::function<void(view::pSprite&)> selectChar2    = bind(&TestStage::onSelectChar2, this, _1);
        std::tr1::function<void(view::pSprite&)> noRed          = bind(&TestStage::onNoRed, this, _1);
        std::tr1::function<void(view::pSprite&)> fullRed        = bind(&TestStage::onFullRed, this, _1);
        std::tr1::function<void(view::pSprite&)> halfAlpha      = bind(&TestStage::onHalfAlpha, this, _1);
        std::tr1::function<void(view::pSprite&)> fullAlpha      = bind(&TestStage::onFullAlpha, this, _1);
        button_01_->onPress( &(InputMgr::i().getInputByIndex(0)->trig1()) ) = moveRight;
        button_02_->onPress( &(InputMgr::i().getInputByIndex(0)->trig1()) ) = moveLeft;
        button_03_->onPress( &(InputMgr::i().getInputByIndex(0)->trig1()) ) = selectChar1;
        button_04_->onPress( &(InputMgr::i().getInputByIndex(0)->trig1()) ) = selectChar2;
        button_05_->onPress( &(InputMgr::i().getInputByIndex(0)->trig1()) ) = noRed;
        button_06_->onPress( &(InputMgr::i().getInputByIndex(0)->trig1()) ) = fullRed;
        button_07_->onPress( &(InputMgr::i().getInputByIndex(0)->trig1()) ) = halfAlpha;
        button_08_->onPress( &(InputMgr::i().getInputByIndex(0)->trig1()) ) = fullAlpha;

        std::tr1::function<void(view::pSprite&)> pressSprite    = bind(&TestStage::onPressSprite, this, _1);

        ctrl::EventDispatcher::i().get_timer_dispatcher("global")->subscribe(
            std::tr1::bind(&presenter::Stage::hitGroup, stage_.get(), 1), 3000, -1);

        ctrl::EventDispatcher::i().get_timer_dispatcher("global")->subscribe(
            std::tr1::bind(&presenter::Stage::hitGroup, stage_.get(), 2), 6000, -1);
    }

    void onMoveRight(view::pSprite& p){
        sprite_->moveTween(300, 200, 1000);
    }
    void onMoveLeft(view::pSprite& p){
        sprite_->moveTween(  0, 200, 1000);
    }
    void onSelectChar1(view::pSprite& p){
        sprite_->setTexture("char1/test");
    }
    void onSelectChar2(view::pSprite& p){
        sprite_->setTexture("char2/test");
    }
    void onNoRed(view::pSprite& p){
        sprite_->set<Red>(0);
    }
    void onFullRed(view::pSprite& p){
        sprite_->set<Red>(255);
    }
    void onHalfAlpha(view::pSprite& p){
        sprite_->set<Alpha>(100);
    }
    void onFullAlpha(view::pSprite& p){
        sprite_->set<Alpha>(255);
    }

    void onPressSprite(view::pSprite& p){
        //p->set<Red>(100);
        //p->set<Alpha>(100);
        //p->set<Scale>(vec3(1.2f,1.2f,1));
        //p->set<GradientDiffuse>(100);
        //p->set<Visible>(false);
    }

    void cycle(){
        stage_->cycle();
        scene_->redraw();
    }

private:
    presenter::pStage stage_;
    view::pScene      scene_;
    view::pSprite     sprite_;

    view::pSprite   button_01_;
    view::pSprite   button_02_;
    view::pSprite   button_03_;
    view::pSprite   button_04_;
    view::pSprite   button_05_;
    view::pSprite   button_06_;
    view::pSprite   button_07_;
    view::pSprite   button_08_;
    view::pSprite   button_09_;
    view::pSprite   button_10_;
    view::pSprite   button_11_;
    view::pSprite   button_12_;
};

#include "App.hpp"
#include <cstdlib> // for srand
#include <ctime> // for time, clock

#include <boost/tr1/functional.hpp> // for bind

int main(){
    std::srand(std::time(0)^std::clock()); //  init srand for global rand...
    psc::Conf::i().init("");
    psc::App::i();
    TestStage tester;
    return psc::App::i().run(std::tr1::bind(&TestStage::cycle, &tester));
}

