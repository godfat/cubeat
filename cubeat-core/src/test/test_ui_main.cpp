#include "Accessors.hpp"
#include "Input.hpp"
#include "view/Scene.hpp"
#include "Accessors.hpp"
#include "view/Scene.hpp"
#include "view/Sprite.hpp"
#include "presenter/Stage.hpp"
#include "EventDispatcher.hpp"
#include "utils/Random.hpp"
#include "utils/dictionary.hpp"
#include "Conf.hpp"
#include "test/test_ui.hpp"
#include "script/lua_utility.hpp"
#include "App.hpp"

#include <cstdlib> // for srand
#include <ctime> // for time, clock
#include <boost/tr1/functional.hpp> // for bind

using namespace psc;
using namespace ctrl;
using namespace accessor;

TestUI::TestUI()
{
    //temporary Scene:
    utils::map_any stage = Conf::i().config_of("test_stage");
    stage_ = presenter::Stage::create( stage.S("test_stage") );
    stage_->playBGM();

    scene_ = view::Scene::create("ui");
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

    std::tr1::function<void(view::pSprite&)> moveRight      = bind(&TestUI::onMoveRight, this, _1);
    std::tr1::function<void(view::pSprite&)> moveLeft       = bind(&TestUI::onMoveLeft, this, _1);
    std::tr1::function<void(view::pSprite&)> selectChar1    = bind(&TestUI::onSelectChar1, this, _1);
    std::tr1::function<void(view::pSprite&)> selectChar2    = bind(&TestUI::onSelectChar2, this, _1);
    std::tr1::function<void(view::pSprite&)> noRed          = bind(&TestUI::onNoRed, this, _1);
    std::tr1::function<void(view::pSprite&)> fullRed        = bind(&TestUI::onFullRed, this, _1);
    std::tr1::function<void(view::pSprite&)> halfAlpha      = bind(&TestUI::onHalfAlpha, this, _1);
    std::tr1::function<void(view::pSprite&)> fullAlpha      = bind(&TestUI::onFullAlpha, this, _1);
    button_01_->onPress( &(InputMgr::i().getInputByIndex(0)->trig1()) ) = moveRight;
    button_02_->onPress( &(InputMgr::i().getInputByIndex(0)->trig1()) ) = moveLeft;
    button_03_->onPress( &(InputMgr::i().getInputByIndex(0)->trig1()) ) = selectChar1;
    button_04_->onPress( &(InputMgr::i().getInputByIndex(0)->trig1()) ) = selectChar2;
    button_05_->onPress( &(InputMgr::i().getInputByIndex(0)->trig1()) ) = noRed;
    button_06_->onPress( &(InputMgr::i().getInputByIndex(0)->trig1()) ) = fullRed;
    button_07_->onPress( &(InputMgr::i().getInputByIndex(0)->trig1()) ) = halfAlpha;
    button_08_->onPress( &(InputMgr::i().getInputByIndex(0)->trig1()) ) = fullAlpha;

    ctrl::EventDispatcher::i().get_timer_dispatcher("global")->subscribe(
        std::tr1::bind(&presenter::Stage::hitGroup, stage_.get(), 1), 3000, -1);

    ctrl::EventDispatcher::i().get_timer_dispatcher("global")->subscribe(
        std::tr1::bind(&presenter::Stage::hitGroup, stage_.get(), 2), 6000, -1);
}

void TestUI::init()
{
    //enable Lua scripting for this class
    L_ = luaL_newstate();
    luaL_openlibs(L_);
    script::Lua::run_script(L_, Conf::i().script_path("ui/test/test1.lua").c_str());
    script::Lua::call(L_, "test_ui_entry", static_cast<void*>(this));
}

TestUI::~TestUI()
{
    lua_close(L_);
}

void TestUI::cycle()
{
    stage_->cycle();
    scene_->redraw();
}

void TestUI::onMoveRight(view::pSprite& p){
    sprite_->moveTween(300, 200, 1000);
}
void TestUI::onMoveLeft(view::pSprite& p){
    sprite_->moveTween(  0, 200, 1000);
}
void TestUI::onSelectChar1(view::pSprite& p){
    sprite_->setTexture("char1/test");
}
void TestUI::onSelectChar2(view::pSprite& p){
    sprite_->setTexture("char2/test");
}
void TestUI::onNoRed(view::pSprite& p){
    sprite_->set<Red>(0);
}
void TestUI::onFullRed(view::pSprite& p){
    sprite_->set<Red>(255);
}
void TestUI::onHalfAlpha(view::pSprite& p){
    sprite_->set<Alpha>(100);
}
void TestUI::onFullAlpha(view::pSprite& p){
    sprite_->set<Alpha>(255);
    //p->set<Scale>(vec3(1.2f,1.2f,1));
    //p->set<GradientDiffuse>(100);
    //p->set<Visible>(false);
}

int main(){
    std::srand(std::time(0)^std::clock()); //  init srand for global rand...
    psc::Conf::i().init("");
    psc::App::i();
    psc::TestUI tester;
    tester.init();
    return psc::App::i().run(std::tr1::bind(&psc::TestUI::cycle, &tester));
}
