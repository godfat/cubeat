
// compile me by:
// g++ -I /opt/local/include/ -I include/ -I lib/ -DBOOST_HAS_GCC_TR1 -D _SHOOTING_CUBES_TEST_ src/test/test_map_and_cubes.cpp src/model/Map.cpp src/model/Cube.cpp src/presenter/Cube.cpp src/presenter/Map.cpp -W -Wall -std=c++98 -o test

#ifdef _SHOOTING_CUBES_TEST_
#include "all_fwd.hpp"
#include "presenter/Map.hpp"

using namespace psc;

int main(){
    presenter::Map::create(data::pMapSetting(new data::MapSetting));
}

#endif
