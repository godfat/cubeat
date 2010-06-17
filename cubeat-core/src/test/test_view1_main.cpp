
/* Testcase */
#include "testcase/ViewTest1.hpp"
#include "App.hpp"
#include <cstdlib> // for srand
#include <ctime> // for time, clock

#include <tr1/functional> // for bind

int main()
{
    std::srand(std::time(0)^std::clock()); //  init srand for global rand...

    psc::App::i(); // init irr

    std::tr1::shared_ptr<ViewTest1> viewtest(new ViewTest1);
    viewtest->init();

    return psc::App::i().run(std::tr1::bind(&ViewTest1::cycle, viewtest));
}
