
#include "App.hpp"

#include <cstdlib> // for srand
#include <ctime> // for time, clock

int main(){
    std::srand(std::time(0)^std::clock()); //  init srand for global rand...
    return psc::App::i().run();
}
