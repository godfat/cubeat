
#include "model/Map.hpp"
#include "model/Cube.hpp"

#include "utils/ChainChecker.hpp"
#include "utils/for_each_multi_array.hpp"
#include "utils/vector_2d.hpp"

#include <boost/lambda/lambda.hpp>

<%= debug_include %>

using namespace psc;
using namespace model;

Map& <% debug_hook 'Map::init_cubes' do %>(){
<% end %>
    using boost::lambda::_1; // this would be used in for_each for element
    using boost::lambda::_2; // this would be used in for_each for index
    utils::vector_2d cube_colors(boost::extents[ms()->width()][ms()->starting_line()]);

    int const one_color_amounts = std::ceil(
        static_cast<double>(ms()->width())*ms()->starting_line()/ms()->color_amounts()
    );
    utils::for_each(cube_colors, _1 = _2/one_color_amounts);

    utils::ChainChecker::until_no_chained_cubes(cube_colors, ms()->chain_amounts());

    for(int i=0, iend=cube_colors.num_elements(); i!=iend; ++i)
    {
        insert(owner_.lock()->make_cube( i%ms()->width(), // x
                                         ms()->height()-1-i/ms()->width(), // y
                                         cube_colors.origin()[i] )->model());
    }
    return *this;
}

void Map::do_cycle(pCube c, int i){
    if(c){
        if( !c->has_grounded()  ) ++n_of_newcomers_;
        if(  c->cycle_and_die() ) cubes_.origin()[i].reset();
    }
}

Map& Map::insert(pCube cube){
    cubes_[cube->x()][cube->y()] = cube;
    return *this;
}
