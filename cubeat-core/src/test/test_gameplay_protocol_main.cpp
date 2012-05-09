
#include "test/test_gameplay_protocol_main.hpp"
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

//


#include "App.hpp"
#include <cstdlib> // for srand
#include <ctime> // for time, clock

#include <boost/tr1/functional.hpp> // for bind

int main(int argc, char* argv[]){
    std::srand(std::time(0)^std::clock()); //  init srand for global rand...
    psc::Conf::i().init("");
    psc::App::i();
    if( argc > 1 ) {
        TestNetGame tester(1);
        return psc::App::i().run(std::tr1::bind(&TestNetGame::cycle, &tester));
    }
    else {
        TestNetGame tester(2);
        return psc::App::i().run(std::tr1::bind(&TestNetGame::cycle, &tester));
    }
}


