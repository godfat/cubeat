
#include "testcase/TestMapViews.hpp"

#include "App.hpp"
#include <cstdlib> // for srand
#include <ctime> // for time, clock

#include <tr1/functional> // for bind

int main(){
    std::srand(std::time(0)^std::clock()); //  init srand for global rand...
    psc::App::i();
    TestMapViews tester;
    return psc::App::i().run(std::tr1::bind(&TestMapViews::cycle, &tester));
}
