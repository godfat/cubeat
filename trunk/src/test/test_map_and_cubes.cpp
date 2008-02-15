
// compile me by:
// g++ -I /opt/local/include/ -I include/ -I lib/ -DBOOST_HAS_GCC_TR1 -D _SHOOTING_CUBES_TEST_ src/test/test_map_and_cubes.cpp src/model/Map.cpp src/model/Cube.cpp src/presenter/Cube.cpp src/presenter/Map.cpp -W -Wall -std=c++98 -o test

#ifdef _SHOOTING_CUBES_TEST_
#include "all_fwd.hpp"
#include "presenter/Map.hpp"
#include <iostream>

using namespace psc;

void preview(presenter::pMap map){
    using std::cout; using std::endl;
    presenter::Map::preview_type p = map->preview();
    for(int y=0; y<11; ++y)
        for(int x=0; x<6; ++x)
            cout << p[x][y] << ' ';
    cout << endl;
}

int main(){
    presenter::pMap map = presenter::Map::create(data::pMapSetting(new data::MapSetting));
    preview(map);
    map->cycle();
    preview(map);
}

#endif
