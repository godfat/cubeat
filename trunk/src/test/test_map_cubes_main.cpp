

#define NDEBUG
// compile me by:
// g++ -I /opt/local/include/ -I include/ -I lib/ -DBOOST_HAS_GCC_TR1 -D _SHOOTING_CUBES_TEST_ src/test/test_map_and_cubes.cpp src/model/Map.cpp src/model/Cube.cpp src/presenter/Cube.cpp src/presenter/Map.cpp -W -Wall -std=c++98 -o test

#include "all_fwd.hpp"
#include "presenter/Map.hpp"
#include "EventDispatcher.hpp"
#include "IrrDevice.hpp"
#include <cstdio>

#include <cstdlib> // for srand
#include <ctime> // for time, clock

using namespace psc;

void display(presenter::pMap map){
    presenter::Map::color_map_type p = map->color_map();
    for(int y=10; y>=0; --y){
        for(int x=0; x<6; ++x)
            std::printf("%2d", p[x][y]);
        std::printf("\n");
    }
    std::printf("\n");
}

void echo(){
    std::cout << "XD" << std::endl;
}

void echo2(){
    std::cout << "Orz...." << std::endl;
}

int main(){
    std::srand(std::time(0)^std::clock()); //  init srand for global rand...

    presenter::pMap map = presenter::Map::create(data::pMapSetting(new data::MapSetting));
    std::cerr << "\n\n\nend creation\n\n\n";
    display(map);
    map->cycle();
    std::cerr << "\n\n\nend cycle\n\n\n";
    display(map);
    for(int i=0; i<25; ++i)
        map->cycle();
    display(map);

    for(int i=0; i<25; ++i)
        map->cycle();
    display(map);

    for(int i=0; i<25; ++i)
        map->cycle();
    display(map);

    IrrDevice::i().init(true);
    psc::ctrl::EventDispatcher::i().subscribe_timer(echo, 1000, -1);
    psc::ctrl::EventDispatcher::i().subscribe_timer(echo2, 500, -1);
    while(IrrDevice::i().d()->run()){
#ifdef WIN32
        Sleep(10);
#elif
        usleep(10000);
#endif
        psc::ctrl::EventDispatcher::i().dispatch();
    }
}
