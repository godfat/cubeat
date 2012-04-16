
#include "net/Manager.hpp"
#include <cstdio>

using namespace psc;
using namespace net;

extern "C" {
#include "script/net/bindings.h"
}

void on_connected() {
    printf("Lua->C: farside connected.\n");
//    N_STATE = N_CONNECTED_SERV;
}

void on_matched() {
    printf("Lua->C: farside matched.\n");
//    N_STATE = N_MATCHED;
}

void on_disconnected() {
    printf("Lua->C: farside disconnected.\n");
//    N_STATE = N_DEFAULT;
}

void on_received(char const* s) {
    printf("Lua->C: farside received something.\n");
}

int poll_from_C() {
//    boost::mutex::scoped_lock l(MQ_MUTEX);
//    if( !MQ.empty() ) {
//        int front = MQ.front();
//        MQ.pop_front();
//        return front;
//    }
    return 0;
}

bool check_quit() {
//    return LUA_QUIT;
    return false;
}
