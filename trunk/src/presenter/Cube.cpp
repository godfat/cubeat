
#include "presenter/Cube.hpp"
#include "presenter/Map.hpp"
#include "model/Cube.hpp"

#include "all_fwd.hpp"

using namespace psc;
using namespace presenter;

Cube::Cube(wpMap map, int x, int y, int color_id):
    map_(map),
    model_(model::Cube::create(map.lock()->model(), x, y, color_id)),
    view_()// view::Cube::create(map.lock()->view()), x, y, color_id)
{}
