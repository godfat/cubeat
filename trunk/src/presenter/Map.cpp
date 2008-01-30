
#include "presenter/Map.hpp"
#include "model/Map.hpp"
// #include "view/Map.hpp"

#include "presenter/Cube.hpp"

#include "all_fwd.hpp"

using namespace psc;
using namespace presenter;

Map::pointer_type Map::init(){
    model_ = model::Map::create(shared_from_this(), setting_);
    // view_ = view::Map::create(shared_from_this());
    return shared_from_this();
}

pCube Map::make_cube(int x, int y, int color_id){
    pCube cube = Cube::create(shared_from_this(), x, y, color_id);
    cubes_.push_back(cube);
    return cube;
}
