#ifndef _SHOOTING_CUBES_NET_MANAGER_
#define _SHOOTING_CUBES_NET_MANAGER_

#include <string>
#include <deque>
#include <map>
#include <boost/tr1/functional.hpp>

#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>

struct lua_State;

namespace psc {
namespace net {

struct State {
    enum ENUM {
        DEFAULT = 0, LOBBY, MATCHED, INGAME
    };
};

class Manager
{
    typedef std::tr1::function<void()>     NetCB;
    typedef std::pair<std::string, NetCB>  NetEvent;
public:
    static Manager& i() {
        static Manager singleton;
        return singleton;
    }

    //let's keep this simple now, don't use delegate syntax
    void on(std::string const& key, NetCB const& cb);
    void dispatch(std::string const& key);
    int  poll();
    void send(int);
    void switch_state(State::ENUM const& s) { state_ = s; }
    void start_lua(int type);

    void quit_lua();
    bool check_quit() const { return lua_quit_; }

private:
    Manager();
    Manager(const Manager&);

    void start_lua_(int type);

    bool           lua_quit_;
    State::ENUM    state_;
    lua_State*     L_;
    boost::thread* lua_thread_;
    boost::mutex   mq_mutex_;

    std::deque<int>              mqueue_;
    std::map<std::string, NetCB> events_;
};

} //net
} //psc

#endif //_SHOOTING_CUBES_NET_MANAGER_


