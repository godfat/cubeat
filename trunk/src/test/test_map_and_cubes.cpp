
// compile me by:
// g++ -I /opt/local/include/ -I include/ -I lib/ -DBOOST_HAS_GCC_TR1 -D _SHOOTING_CUBES_TEST_ src/test/test_map_and_cubes.cpp src/model/Map.cpp src/model/Cube.cpp src/presenter/Cube.cpp src/presenter/Map.cpp -W -Wall -std=c++98 -o test

#ifdef _SHOOTING_CUBES_TEST_
#include "all_fwd.hpp"
#include "presenter/Map.hpp"
#include <cstdio>

#include <cstdlib> // for srand
#include <ctime> // for time, clock

using namespace psc;

void preview(presenter::pMap map){
    presenter::Map::preview_type p = map->preview();
    for(int y=10; y>=0; --y){
        for(int x=0; x<6; ++x)
            std::printf("%2d", p[x][y]);
        std::printf("\n");
    }
    std::printf("\n");
}

int main(){
    std::srand(std::time(0)^std::clock()); //  init srand for global rand...

    presenter::pMap map = presenter::Map::create(data::pMapSetting(new data::MapSetting));
    std::cerr << "\n\n\nend creation\n\n\n";
    preview(map);
    map->cycle();
    std::cerr << "\n\n\nend cycle\n\n\n";
    preview(map);
    map->cycle();
    preview(map);
    map->cycle();
    preview(map);
    map->cycle();
    preview(map);
}

#endif
