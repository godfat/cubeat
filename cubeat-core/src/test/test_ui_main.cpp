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

    bg_  = view::Sprite::create("square", scene_, 256, 256);
    bg_->moveTo(600, 50);
    act_ = view::Sprite::create("char1/test", bg_, 192, 192, true);
    act_->moveTo(128, 128);
    text_= view::SpriteText::create("Test Panel", bg_, "Star Jedi", 24, true);
    text_->moveTo(128, -20);
    btn_change_char_    = view::Sprite::create("cubes/cube1", scene_, 64, 64);
    btn_change_char_->moveTo(900, 60);
    btn_visible_panel_  = view::Sprite::create("cubes/cube1", scene_, 64, 64);
    btn_visible_panel_->moveTo(900, 140);
    std::tr1::function<void(view::pSprite&)> clickLeft  = bind(&TestUI::LeftBtnClick, this, _1);
    std::tr1::function<void(view::pSprite&)> clickRight = bind(&TestUI::RightBtnClick, this, _1);
    std::tr1::function<void(view::pSprite&, int, int)> enter = bind(&TestUI::EnterFocus, this, _1, _2, _3);
    std::tr1::function<void(view::pSprite&, int, int)> leave = bind(&TestUI::LeaveFocus, this, _1, _2, _3);
    btn_change_char_->onPress( &(InputMgr::i().getInputByIndex(0)->trig1()) ) = clickLeft;
    btn_change_char_->onPress( &(InputMgr::i().getInputByIndex(0)->trig2()) ) = clickRight;
    btn_change_char_->onEnterFocus( InputMgr::i().getInputByIndex(0) ) = enter;
    btn_change_char_->onLeaveFocus( InputMgr::i().getInputByIndex(0) ) = leave;
    std::tr1::function<void(view::pSprite&)> showPanel = bind(&TestUI::ShowPanel, this, _1);
    std::tr1::function<void(view::pSprite&)> hidePanel = bind(&TestUI::HidePanel, this, _1);
    btn_visible_panel_->onPress( &(InputMgr::i().getInputByIndex(0)->trig1()) ) = showPanel;
    btn_visible_panel_->onPress( &(InputMgr::i().getInputByIndex(0)->trig2()) ) = hidePanel;
    btn_visible_panel_->onEnterFocus( InputMgr::i().getInputByIndex(0) ) = enter;
    btn_visible_panel_->onLeaveFocus( InputMgr::i().getInputByIndex(0) ) = leave;
    btn_visible_char_   = view::Sprite::create("cubes/cube1", scene_, 64, 64);
    btn_visible_char_->moveTo(900, 220);
    std::tr1::function<void(view::pSprite&)> showChar = bind(&TestUI::ShowChar, this, _1);
    std::tr1::function<void(view::pSprite&)> hideChar = bind(&TestUI::HideChar, this, _1);
    btn_visible_char_->onPress( &(InputMgr::i().getInputByIndex(0)->trig1()) ) = showChar;
    btn_visible_char_->onPress( &(InputMgr::i().getInputByIndex(0)->trig2()) ) = hideChar;
    btn_visible_char_->onEnterFocus( InputMgr::i().getInputByIndex(0) ) = enter;
    btn_visible_char_->onLeaveFocus( InputMgr::i().getInputByIndex(0) ) = leave;

    btn1_ = view::Sprite::create("cubes/cube-b-1", scene_, 64, 64);
    btn1_->moveTo(900, 320);
    std::tr1::function<void(view::pSprite&)> showSelCharPanel  = bind(&TestUI::ShowSelCharPanel, this, _1);
    std::tr1::function<void(view::pSprite&)> hideSelCharPanel  = bind(&TestUI::HideSelCharPanel, this, _1);
    btn1_->onPress( &(InputMgr::i().getInputByIndex(0)->trig1()) ) = showSelCharPanel;
    btn1_->onPress( &(InputMgr::i().getInputByIndex(0)->trig2()) ) = hideSelCharPanel;
    btn1_->onEnterFocus( InputMgr::i().getInputByIndex(0) ) = enter;
    btn1_->onLeaveFocus( InputMgr::i().getInputByIndex(0) ) = leave;
}

void TestUI::init()
{
    //enable Lua scripting for this class
    L_ = luaL_newstate();
    luaL_openlibs(L_);
    script::Lua::run_script(L_, Conf::i().script_path("ui/test/test1.lua").c_str());
    script::Lua::call(L_, "init", static_cast<void*>(this));
    script::Lua::call(L_, "init_select_char_panel", static_cast<void*>(this));
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
    p->set<Blue>(0);
}

void TestUI::LeaveFocus(view::pSprite& p, int a, int b){
    p->set<Blue>(255);
}

void TestUI::ShowPanel(view::pSprite& p) {
    bg_->set<Visible>(true);
}

void TestUI::HidePanel(view::pSprite& p){
    bg_->set<Visible>(false);
}

void TestUI::ShowChar(view::pSprite& p) {
    act_->set<Visible>(true);
}

void TestUI::HideChar(view::pSprite& p) {
    act_->set<Visible>(false);
}

void TestUI::ShowSelCharPanel(view::pSprite& p){
    script::Lua::call(L_, "set_select_char_panel_visible", static_cast<void*>(this), true);
}

void TestUI::HideSelCharPanel(view::pSprite& p){
    script::Lua::call(L_, "set_select_char_panel_visible", static_cast<void*>(this), false);
}

int main(){
    std::srand(std::time(0)^std::clock()); //  init srand for global rand...
    psc::Conf::i().init("");
    psc::App::i();
    psc::TestUI tester;
    tester.init();
    return psc::App::i().run(std::tr1::bind(&psc::TestUI::cycle, &tester));
}
