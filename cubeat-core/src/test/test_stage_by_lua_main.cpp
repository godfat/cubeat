
#include "view/Scene.hpp"
#include "EventDispatcher.hpp"
#include "ctrl/TimerDispatcher.hpp"
#include "script/lua_utility.hpp"
#include "audio/Sound.hpp"
#include "Conf.hpp"

//ok, is this really cross-platform?
#include <sys/stat.h>

#include <sstream>

using namespace psc;

class TestStageLua
{
public:
    TestStageLua(): last_fs_time_(0) {
        scene_ = view::Scene::create("stage_lua");
        scene_->setTo2DView();
        L_ = luaL_newstate();
        luaL_openlibs(L_);
        stat(Conf::i().script_path("test_stage.lua").c_str(), &fs);
        last_fs_time_ = fs.st_mtime;
        script::Lua::run_script(L_, Conf::i().script_path("test_stage.lua").c_str());
        script::Lua::call(L_, "init", static_cast<void*>(&scene_));
    }

    void cycle() {
        scene_->redraw();

        stat(Conf::i().script_path("test_stage.lua").c_str(), &fs);
        if( fs.st_mtime > last_fs_time_ ) {
            printf("C: reloading Lua stage...\n");
            script::Lua::call(L_, "cleanup");
            script::Lua::run_script(L_, Conf::i().script_path("test_stage.lua").c_str());
            script::Lua::call(L_, "init", static_cast<void*>(&scene_));
            last_fs_time_ = fs.st_mtime;
        }
    }

private:
    view::pScene scene_;
    lua_State* L_;
    struct stat fs;
    int last_fs_time_;
};

#include "App.hpp"
#include <cstdlib> // for srand
#include <ctime> // for time, clock

#include <boost/tr1/functional.hpp> // for bind

int main(){
    std::srand(std::time(0)^std::clock()); //  init srand for global rand...
    psc::Conf::i().init("");
    psc::App::i().init();
    TestStageLua tester;
    return psc::App::i().run(std::tr1::bind(&TestStageLua::cycle, &tester));
}
