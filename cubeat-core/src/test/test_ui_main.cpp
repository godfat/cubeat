
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

int main(){
    std::srand(std::time(0)^std::clock()); //  init srand for global rand...
    psc::Conf::i().init("");
    psc::App::i();
    psc::TestUI tester;
    tester.init();
    return psc::App::i().run(std::tr1::bind(&psc::TestUI::cycle, &tester));
}

