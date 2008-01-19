
/* Make all constants changable through a outer text file,
   there will be hundreds of them, so had better be faster ...
   need to provide a "constant variable interface." */

#include "App.hpp"

int main()
{
    return psc::App::i().run();
}
