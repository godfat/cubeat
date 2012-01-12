#include "Accessors.hpp"
#include "Input.hpp"
#include "view/Scene.hpp"
#include "Accessors.hpp"
#include "view/Scene.hpp"
#include "view/Sprite.hpp"
#include "view/SpriteText.hpp"
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

    ctrl::EventDispatcher::i().get_timer_dispatcher("global")->subscribe(
        std::tr1::bind(&presenter::Stage::hitGroup, stage_.get(), 1), 3000, -1);

    ctrl::EventDispatcher::i().get_timer_dispatcher("global")->subscribe(
        std::tr1::bind(&presenter::Stage::hitGroup, stage_.get(), 2), 6000, -1);

    act_ = view::Sprite::create("char1/test", scene_, 192, 192);
    btn_ = view::Sprite::create("cubes/cube1", scene_, 64, 64);
    text_= view::SpriteText::create("Test word", scene_, "Star Jedi", 24);
    act_->moveTo(620, 50);
    btn_->moveTo(920, 50);
    text_->moveTo(600, 10);
    std::tr1::function<void(view::pSprite&)> clickLeft  = bind(&TestUI::LeftBtnClick, this, _1);
    std::tr1::function<void(view::pSprite&)> clickRight = bind(&TestUI::RightBtnClick, this, _1);
    std::tr1::function<void(view::pSprite&, int, int)> enter = bind(&TestUI::EnterFocus, this, _1, _2, _3);
    std::tr1::function<void(view::pSprite&, int, int)> leave = bind(&TestUI::LeaveFocus, this, _1, _2, _3);
    btn_->onPress( &(InputMgr::i().getInputByIndex(0)->trig1()) ) = clickLeft;
    btn_->onPress( &(InputMgr::i().getInputByIndex(0)->trig2()) ) = clickRight;
    btn_->onEnterFocus( InputMgr::i().getInputByIndex(0) ) = enter;
    btn_->onLeaveFocus( InputMgr::i().getInputByIndex(0) ) = leave;
}

void TestUI::init()
{
    //enable Lua scripting for this class
    L_ = luaL_newstate();
    luaL_openlibs(L_);
    script::Lua::run_script(L_, Conf::i().script_path("ui/test/test1.lua").c_str());
    script::Lua::call(L_, "init", static_cast<void*>(this));
}

TestUI::~TestUI()
{
    script::Lua::call(L_, "destroy", static_cast<void*>(this));
    lua_close(L_);
}

void TestUI::cycle()
{
    script::Lua::call(L_, "cycle", static_cast<void*>(this));
    stage_->cycle();
    scene_->redraw();
}

void TestUI::EnterFocus(view::pSprite& p, int a, int b){
    p->set<Red>(0);
}

void TestUI::LeaveFocus(view::pSprite& p, int a, int b){
    p->set<Red>(255);
}

int main(){
    std::srand(std::time(0)^std::clock()); //  init srand for global rand...
    psc::Conf::i().init("");
    psc::App::i();
    psc::TestUI tester;
    tester.init();
    return psc::App::i().run(std::tr1::bind(&psc::TestUI::cycle, &tester));
}
