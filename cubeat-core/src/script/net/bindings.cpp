
#include "net/Manager.hpp"
#include <cstdio>

using namespace psc;

extern "C" {
#include "script/net/bindings.h"
}

/* no I can't write like that. these functions will probably be called
   from another thread! (although I can avoid that by making net::Manager
   run in main thread) By doing so, some of the main presenter's methods
   will be called by child thread, and it will just fucking ruin everything. */

//void on_connected() {
//    printf("Lua->C: farside connected.\n");
//    net::Manager::i().dispatch("connected");
////    N_STATE = N_CONNECTED_SERV;
//}
//
//void on_matched() {
//    printf("Lua->C: farside matched.\n");
//    net::Manager::i().dispatch("matched");
////    N_STATE = N_MATCHED;
//}
//
//void on_disconnected() {
//    printf("Lua->C: farside disconnected.\n");
//    net::Manager::i().dispatch("disconnected");
////    N_STATE = N_DEFAULT;
//}
//
//void on_received(int) {
//    printf("Lua->C: farside received something.\n");
//    net::Manager::i().dispatch("received");
//}

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
