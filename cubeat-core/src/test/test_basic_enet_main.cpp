
#include "test/test_basic_enet_main.hpp"
#include "presenter/Stage.hpp"
#include "EventDispatcher.hpp"
#include "utils/dictionary.hpp"
#include "net/Manager.hpp"
#include "Input.hpp"
#include "Conf.hpp"

#include <boost/lexical_cast.hpp>
#include <cstdio>

using namespace psc;
using namespace test;
using namespace utils;

TestBasicENet::TestBasicENet(int type)
    :type_(type)
{
    utils::map_any stage = Conf::i().config_of("test_stage");
    stage_ = presenter::Stage::create( stage.S("test_stage") );
    stage_->playBGM();

    ctrl::EventDispatcher::i().get_timer_dispatcher("global")->subscribe(
        std::tr1::bind(&presenter::Stage::hitGroup, stage_.get(), 1), 3000, -1);

    ctrl::EventDispatcher::i().get_timer_dispatcher("global")->subscribe(
        std::tr1::bind(&presenter::Stage::hitGroup, stage_.get(), 2), 6000, -1);

    using std::tr1::bind;
    using namespace std::tr1::placeholders;
    net::Manager::i().on("connected",   bind(&TestBasicENet::on_connected, this, _1));
    net::Manager::i().on("matched",     bind(&TestBasicENet::on_matched, this, _1));
    net::Manager::i().on("received",    bind(&TestBasicENet::on_received, this, _1));
    net::Manager::i().on("disconnected",bind(&TestBasicENet::on_disconnected, this, _1));

    ctrl::EventDispatcher::i().get_timer_dispatcher("global")->subscribe(
        bind(&TestBasicENet::update_cursor_pos, this), 10, -1);

    ctrl::EventDispatcher::i().get_timer_dispatcher("global")->subscribe(
        bind(&TestBasicENet::initiate_matching, this), 5000); // mockup connection initiator

    net::Manager::i().init_lua(type);
}

void TestBasicENet::update_cursor_pos(){
    ctrl::Input* input = ctrl::InputMgr::i().getInputByIndex( (type_ == 1 ? 0 : 1) );
    int x = input->cursor().x();
    int y = input->cursor().y();
    char buf[32] = {0};
    sprintf(buf, "return {T='MOV', x=%d, y=%d}", x, y);
    net::Manager::i().send(buf);
}

void TestBasicENet::initiate_matching(){
    net::Manager::i().send("1"); //mockup message
}

void TestBasicENet::on_connected(std::string const& msg) {
}

void TestBasicENet::on_matched(std::string const& msg) {
}

template<typename T>
T get_next(std::string const& msg, int& from) {
    int to  = msg.find_first_of(" ", from);
    T   ret = boost::lexical_cast<T>(msg.substr(from, to-from));
    from = to + 1;
    return ret;
}

void TestBasicENet::on_received(std::string const& msg) {
    int to = 0; //to will be changed
    std::string cmd = get_next<std::string>(msg, to); //to will be changed

    if( cmd == "mov" ) {
        int tm= get_next<int>(msg, to); //to will be changed
        int x = get_next<int>(msg, to);
        int y = get_next<int>(msg, to);
        on_mov(x, y);
    }
    else {
        printf("unknown command: %s\n", msg.c_str());
    }
}

void TestBasicENet::on_disconnected(std::string const& msg) {
}

void TestBasicENet::on_mov(int x, int y) {
    ctrl::InputMgr::i().getInputByIndex( (type_ == 1 ? 1 : 0) )->cursor().x( x );
    ctrl::InputMgr::i().getInputByIndex( (type_ == 1 ? 1 : 0) )->cursor().y( y );
}

TestBasicENet::~TestBasicENet() {
    net::Manager::i().end_lua();
}

void TestBasicENet::cycle() {
    net::Manager::i().process_lua();
    stage_->cycle();
}

#include "App.hpp"
#include <cstdlib> // for srand
#include <ctime> // for time, clock

#include <boost/tr1/functional.hpp> // for bind

int main(int argc, char* argv[]){
    std::srand(std::time(0)^std::clock()); //  init srand for global rand...
    psc::Conf::i().init("");
    psc::App::i();
    if( argc > 1 ) {
        TestBasicENet tester(1);
        return psc::App::i().run(std::tr1::bind(&TestBasicENet::cycle, &tester));
    }
    else {
        TestBasicENet tester(2);
        return psc::App::i().run(std::tr1::bind(&TestBasicENet::cycle, &tester));
    }
}


