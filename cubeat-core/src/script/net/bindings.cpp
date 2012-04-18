
#include "net/Manager.hpp"
#include <cstdio>

using namespace psc;

extern "C" {
#include "script/net/bindings.h"
}

void on_connected() {
    printf("Lua->C: farside connected.\n");
    net::Manager::i().dispatch("connected");
//    N_STATE = N_CONNECTED_SERV;
}

void on_matched() {
    printf("Lua->C: farside matched.\n");
    net::Manager::i().dispatch("matched");
//    N_STATE = N_MATCHED;
}

void on_disconnected() {
    printf("Lua->C: farside disconnected.\n");
    net::Manager::i().dispatch("disconnected");
//    N_STATE = N_DEFAULT;
}

void on_received(char const* s) {
    printf("Lua->C: farside received something.\n");
    net::Manager::i().dispatch("received");
}

int poll_from_C() {
    return net::Manager::i().poll();
//    boost::mutex::scoped_lock l(MQ_MUTEX);
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
