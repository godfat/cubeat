
#ifndef _SHOOTING_CUBES_TEST_

/* Make all constants changable through a outer text file,
   there will be hundreds of them, so had better be faster ...
   need to provide a "constant variable interface." */

#include "App.hpp"
#include <cstdlib> // for srand
#include <ctime> // for time, clock

int main()
{
    std::srand(std::time(0)^std::clock()); //  init srand for global rand...
    return psc::App::i().run();
}

#endif
