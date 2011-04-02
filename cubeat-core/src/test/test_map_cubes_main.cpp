
// compile me by:
// g++ -I /opt/local/include/ -I include/ -I lib/ -DBOOST_HAS_GCC_TR1 -D _SHOOTING_CUBES_TEST_ src/test/test_map_and_cubes.cpp src/model/Map.cpp src/model/Cube.cpp src/presenter/Cube.cpp src/presenter/Map.cpp -W -Wall -std=c++98 -o test

#include "all_fwd.hpp"
#include "presenter/Map.hpp"
#include "presenter/cube/ViewStdout.hpp"
#include "EventDispatcher.hpp"
#include "IrrDevice.hpp"

#include <cstdio>
#include <cstdlib> // for srand
#include <ctime> // for time, clock

#include <boost/tr1/memory.hpp>
#include <boost/tr1/functional.hpp>
#include <iostream>

using namespace psc;

void echo(){
    std::cout << "XD" << std::endl;
}

void echo2(){
    std::cout << "Orz...." << std::endl;
}

int main(){
    std::srand(std::time(0)^std::clock()); //  init srand for global rand...
    IrrDevice::i().init(true);

    presenter::pMap map = presenter::Map::create();
    map->push_view_slave( presenter::cube::ViewStdoutMaster::create() );

    std::cerr << "\n\n\nend creation\n\n\n";
    // display(map);
    map->cycle();
    std::cerr << "\n\n\nend cycle\n\n\n";
    // display(map);
    // for(int i=0; i<25; ++i)
    //     map->cycle();
    // display(map);
    //
    // for(int i=0; i<25; ++i)
    //     map->cycle();
    // display(map);
    //
    // for(int i=0; i<25; ++i)
    //     map->cycle();
    // display(map);

    IrrDevice::i().init(true);
    // psc::ctrl::EventDispatcher::i().subscribe_timer(echo, 1000, -1);
    // psc::ctrl::EventDispatcher::i().subscribe_timer(echo2, 500, -1);
    // psc::ctrl::EventDispatcher::i().subscribe_timer(std::tr1::bind(&display, std::tr1::ref(map)), 500, -1);
    psc::ctrl::EventDispatcher::i().subscribe_timer(std::tr1::bind(&std::exit, 0), 10000);
    while(IrrDevice::i().d()->run()){
        map->cycle();
        map->redraw();
#ifdef WIN32
        Sleep(10);
#else
        usleep(10000);
#endif
        psc::ctrl::EventDispatcher::i().dispatch();
    }
}
