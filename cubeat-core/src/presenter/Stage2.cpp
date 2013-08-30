
#include "presenter/Stage2.hpp"
#include "view/Scene.hpp"
#include "audio/Sound.hpp"
#include "audio/SoundObject.hpp"
#include "script/lua_utility.hpp"
#include "Conf.hpp"

using namespace irr;
using namespace video;
using namespace scene;

using namespace psc;
using namespace presenter;

Stage2::Stage2() : need_release_(false), last_fs_time_(0)
{
    audio::Sound::i().stopAll();
}

Stage2::~Stage2()
{
    if( need_release_ ) {
        script::Lua::call(L_, "set_release", true);
    }
    script::Lua::call(L_, "cleanup");
    lua_close(L_);
}

//tell engine to release some of the cached resources.
Stage2& Stage2::releaseResource()
{
    need_release_ = true;
    return *this;
}

pStage2 Stage2::init( std::string const& path )
{
    lua_path_ = path+".lua";

    conf_ = Conf::i().config_of(path);
    printf("C: 1\n");
    scene_ = view::Scene::create( conf_.S("name") );
    scene_->setTo2DView();
    if( path == "stage/jungle6" ) { // WTF: hard code for now
        scene_->enablePostProc();
    }
    printf("C: 2\n");
    L_ = luaL_newstate();
    luaL_openlibs(L_);

    // check for file time
    stat(Conf::i().script_path(lua_path_).c_str(), &fs);
    last_fs_time_ = fs.st_mtime;

    script::Lua::run_script(L_, Conf::i().script_path(lua_path_).c_str() );
    printf("C: 3\n");
    script::Lua::call(L_, "init", static_cast<void*>(&scene_));
    printf("C: 4\n");
    music_path_a_ = conf_.S("music");
    music_path_b_ = conf_.S("music2");
    fmusic_path_a_= conf_.S("fast_music");
    fmusic_path_b_= conf_.S("fast_music2");

    return shared_from_this();
}

Stage2& Stage2::playBGM(time_t const& fade_t)
{
    audio::Sound::i().seek_and_playBGM_AB(music_path_a_, music_path_b_, 1000, fade_t);
    return *this;
}

Stage2& Stage2::playFastBGM(time_t const& fade_t)
{
    audio::Sound::i().playBGM_AB(fmusic_path_a_, fmusic_path_b_, fade_t);
    return *this;
}

void Stage2::cycle()
{
    //script::Lua::call(L_, "cycle");
    scene_->redraw();

    stat(Conf::i().script_path(lua_path_).c_str(), &fs);
    if( fs.st_mtime > last_fs_time_ ) {
        printf("C: reloading Lua stage...\n");
        script::Lua::call(L_, "cleanup");
        script::Lua::run_script(L_, Conf::i().script_path(lua_path_).c_str());
        script::Lua::call(L_, "init", static_cast<void*>(&scene_));
        last_fs_time_ = fs.st_mtime;
    }
}

