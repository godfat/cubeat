
#include "net/Manager.hpp"
#include <cstdio>

using namespace psc;

extern "C" {
#include "script/net/bindings.h"
}

void on_connected(char const* str) {
    printf("Lua->C: farside connected.\n");
    net::Manager::i().dispatch("connected", str);
    net::Manager::i().switch_state(net::State::LOBBY);
//    N_STATE = N_CONNECTED_SERV;
}

void on_matched(char const* str) {
    printf("Lua->C: farside matched.\n");
    net::Manager::i().dispatch("matched", str);
    net::Manager::i().switch_state(net::State::MATCHED);
//    N_STATE = N_MATCHED;
}

void on_disconnected(char const* str) {
    printf("Lua->C: farside disconnected.\n");
    net::Manager::i().dispatch("disconnected", str);
//    N_STATE = N_DEFAULT;
}

void on_received(char const* str) {
    printf("Lua->C: farside received something.\n");
    net::Manager::i().dispatch("received", str);
}

char const* poll_from_C() {
    return net::Manager::i().poll().c_str();
//    //boost::mutex::scoped_lock l(MQ_MUTEX);
//    if( !MQ.empty() ) {
//        int front = MQ.front();
//        MQ.pop_front();
//        return front;
//    }
}

bool check_quit() {
//    return LUA_QUIT;
    return net::Manager::i().check_quit();
}
