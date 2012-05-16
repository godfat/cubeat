
#include "net/Manager.hpp"
#include "script/lua_utility.hpp"
#include "utils/Logger.hpp"
#include "Conf.hpp"

using namespace psc;
using namespace net;
using namespace script;
using utils::Logger;

Manager::Manager()
    :lua_quit_(true), state_(State::DEFAULT), L_(0)//, lua_thread_(0)
{
}

void Manager::switch_state(State::ENUM const& e)
{
    state_ = e;
}

void Manager::on(std::string const& key, NetCB const& cb)
{
    events_[key] = cb;
}

void Manager::dispatch(std::string const& key, std::string const& msg)
{
    if( events_[key] )
        events_[key](msg);
    else
        Logger::i().buf("C: bad net callback: ").buf(key).buf(" ").buf(msg).endl();
}

std::string Manager::poll()
{
    //boost::mutex::scoped_lock l(mq_mutex_);
    if( !mqueue_.empty() ) {
        std::string const& front = mqueue_.front();
        mqueue_.pop_front();
        return front;
    }
    return std::string();
}

void Manager::init_lua(int type)
{
    L_ = luaL_newstate();
    luaL_openlibs(L_);
    Lua::run_script(L_, Conf::i().script_path("net/eventloop.lua").c_str());

    bool get_lobby = false;
    if( type == 1 ) {
        get_lobby = Lua::call_R<bool>(L_, "init", 1); //server
    }
    else {
        get_lobby = Lua::call_R<bool>(L_, "init", 2); //client
    }
    if ( !get_lobby ) {
        printf("C: notified by lua that connecting to lobby server failed.\n");
        end_lua();
    }
}

void Manager::process_lua()
{
    Lua::call(L_, "run");
}

void Manager::end_lua()
{
    Lua::call(L_, "dtor");
    printf("C: Lua cleaned up.\n");

    mqueue_.clear();

    state_ = State::DEFAULT;
    lua_close(L_);
    L_ = 0;
    printf("C: Lua State really closed.\n");
}

void Manager::send(std::string const& msg)
{
    if( /*!lua_quit_ &&*/ state_ >= State::LOBBY ) {
        mqueue_.push_back(msg);
    }
}
