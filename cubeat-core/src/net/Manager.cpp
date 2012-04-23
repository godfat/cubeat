
#include "net/Manager.hpp"
#include "script/lua_utility.hpp"
#include "utils/Logger.hpp"
#include "Conf.hpp"

using namespace psc;
using namespace net;
using namespace script;
using utils::Logger;

Manager::Manager()
    :lua_quit_(true), state_(State::DEFAULT), L_(0), lua_thread_(0)
{
}

void Manager::on(std::string const& key, NetCB const& cb)
{
    events_[key] = cb;
}

void Manager::dispatch(std::string const& key)
{
    if( events_[key] )
        events_[key]();
    else
        Logger::i().buf("C: bad net callback: ").buf(key).endl();
}

int  Manager::poll()
{
    boost::mutex::scoped_lock l(mq_mutex_);
    if( !mqueue_.empty() ) {
        int front = mqueue_.front();
        mqueue_.pop_front();
        return front;
    }
    return 0;
}

void Manager::start_lua(int type)
{
    if( !L_ ) {
        if ( lua_thread_ ) {
            delete lua_thread_;
            lua_thread_ = 0;
        }
        lua_thread_ =
            new boost::thread( std::tr1::bind(&Manager::start_lua_, this, type) );
    }
}

void Manager::start_lua_(int type)
{
    L_ = luaL_newstate();
    luaL_openlibs(L_);
    Lua::run_script(L_, Conf::i().script_path("net/eventloop.lua").c_str());

    lua_quit_ = false; //indicate the event loop in Lua will run

    if( type == 1 )
        Lua::call(L_, "init", 1); //server
    else
        Lua::call(L_, "init", 2); //client
    Lua::call(L_, "run");

    printf("C: Returned back to C.\n");
    {
        boost::mutex::scoped_lock l(mq_mutex_);
        mqueue_.clear();
    }
    state_ = State::DEFAULT;

    lua_close(L_);
    L_ = 0;
    printf("C: Lua State really closed.\n");
}

void Manager::quit_lua()
{
    if( L_ ) {
        lua_quit_ = true;
    }
}

void Manager::send(int msg)
{
    if( !lua_quit_ && state_ >= State::LOBBY ) {
        boost::mutex::scoped_lock l(mq_mutex_);
        mqueue_.push_back(msg);
    }
}
