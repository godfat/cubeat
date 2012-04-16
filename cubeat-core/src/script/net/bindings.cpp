
#include "test/test_basic_enet_main.hpp"
#include <cstdio>

using namespace psc;
//using namespace ctrl;
using namespace presenter;

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
