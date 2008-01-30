
// compile me by:
// g++ -I /opt/local/include/ -I include/ -D _SHOOTING_CUBES_TEST_ src/test/test_map_and_cubes.cpp -o test

#ifdef _SHOOTING_CUBES_TEST_
#include "all_fwd.hpp"
#include "presenter/Map.hpp"

using namespace psc;

int main(){
    presenter::Map::create(data::pMapSetting(new data::MapSetting));
}

#endif