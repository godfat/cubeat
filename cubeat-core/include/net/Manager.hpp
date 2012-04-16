#ifndef _SHOOTING_CUBES_NET_MANAGER_
#define _SHOOTING_CUBES_NET_MANAGER_

#include <string>
#include <map>
#include <boost/tr1/functional.hpp>

namespace psc {
namespace net {

class Manager
{
    typedef std::tr1::function<void()>     NetCB;
    typedef std::pair<std::string, NetCB>  NetEvent;
public:
    static Manager i() {
        static Manager singleton;
        return singleton;
    }

    //let's keep this simple now, don't use delegate syntax
    void on(std::string const& key, NetCB const& cb);

private:
    Manager();
    Manager(const Manager&);

    std::map<std::string, NetCB> events_;
};

} //net
} //psc

#endif //_SHOOTING_CUBES_NET_MANAGER_


