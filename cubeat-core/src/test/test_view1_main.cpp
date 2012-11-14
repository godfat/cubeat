/*
    TODO:

I noticed a glitch in this test case. not sure if old irrlicht/cubeat
has this problem tho. the timer will be interfered by factor outside of
the current process. like some resizing or moving of the widget will
likely break animator effects (especially cyclic ones consist of repetitive
"Equations", they don't reset to the original value after a full period)
*/

/* Testcase */
#include "testcase/ViewTest1.hpp"
#include "Conf.hpp"
#include "App.hpp"
#include <cstdlib> // for srand
#include <ctime> // for time, clock

#include <boost/tr1/functional.hpp> // for bind

int main()
{
    std::srand(std::time(0)^std::clock()); //  init srand for global rand...
    psc::Conf::i().init("");
    psc::App::i().init();

    std::tr1::shared_ptr<ViewTest1> viewtest(new ViewTest1);
    viewtest->init();

    return psc::App::i().run(std::tr1::bind(&ViewTest1::cycle, viewtest));
}
