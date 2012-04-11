
#include "model/SimpleCube.hpp"
#include "model/SimpleMap.hpp"

using namespace psc;
using namespace model;

bool SimpleCube::cycle_and_die() { return is_dead_; }

pSimpleCube SimpleCube::dump_data_to_map(wpSimpleMap map) const
{
    pSimpleCube result = SimpleCube::create(map, x(), y(), color_id());
    result->real_y(real_y());
    result->has_grounded_ = has_grounded_;
    result->is_broken_    = is_broken_;
    result->is_garbage_   = is_garbage_;
	result->hp_           = hp_;

    return result;
}
