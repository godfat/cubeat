
#include "Player.hpp"
#include "model/Cube.hpp"
#include "presenter/PlayerAbility.hpp"
#include "presenter/Map.hpp"
#include "EventDispatcher.hpp"

#include <vector>
#include <algorithm>

using namespace psc;
using namespace presenter;
using std::tr1::bind;
using std::tr1::ref;

///////////////////////////// Character 1 Ability //////////////////////////////

void PlayerAbility::C1(ctrl::wpPlayer const& player, wpMap const& self_map, wpMap const& enemy_map)
{

}

///////////////////////////// Character 2 Ability //////////////////////////////

void check_garbage_and_restore(model::Cube& c)
{
    if( c->is_garbage() || c->is_broken() )
        c->restore();
}

void PlayerAbility::C2(ctrl::wpPlayer const& player, wpMap const& self_map, wpMap const& enemy_map)
{
    if( ctrl::pPlayer p = player.lock() ) {
        if( pMap m0 = self_map.lock() ) {
            m0->foreach_cube(check_garbage_and_restore);
        }
    }
}

///////////////////////////// Character 3 Ability //////////////////////////////

void PlayerAbility::C3(ctrl::wpPlayer const& player, wpMap const& self_map, wpMap const& enemy_map)
{
    if( ctrl::pPlayer p = player.lock() ) {
        if ( pMap m0 = self_map.lock() ) {
            m0->set_garbage_amount( m0->garbage_left() / 3 );
        }
    }
}

///////////////////////////// Character 4 Ability //////////////////////////////

void check_color_and_restore(model::Cube& c, int color_2nd, int color_1st)
{
    if( c->color_id() == color_2nd )
        c->restore_to(color_1st);
}

void PlayerAbility::C4(ctrl::wpPlayer const& player, wpMap const& self_map, wpMap const& enemy_map)
{
    if( ctrl::pPlayer p = player.lock() ) {
        if( pMap m0 = self_map.lock() ) {
            // pair(color_id, count), and it's sorted using count
            std::vector< std::pair<int, int> > const& color_counts = m0->count_colors();
            int max_1st_color = color_counts[0].first;
            int max_2nd_color = color_counts[0].second;
            m0->foreach_cube(bind(&check_color, _1, max_2nd_color, max_1st_color));
        }
    }
}

///////////////////////////// Character 5 Ability //////////////////////////////

void row_do(model::Cube& c)
{
    if( c ) c->go_exploding();
}

void PlayerAbility::C5(ctrl::wpPlayer const& player, wpMap const& self_map, wpMap const& enemy_map)
{
    if( ctrl::pPlayer p = player.lock() ) {
        if( pMap m0 = self_map.lock() ) {
            int delay_time = 1;
            int width = m0->map_setting()->width();
            int height= m0->map_setting()->height();
            for( size_t y = height - 1 ; y >= 5; --y ) {
                for( size_t x = 0; x < width; ++x ) {
                    if( m0->exist(x, y) ) {
                        ctrl::EventDispatcher::i().get_timer_dispatcher("game")->subscribe(
                            bind(&presenter::Map::for_row, m0.get(), y, row_do), m0, delay_time);
                        delay_time += 300;
                    }
                }
            }
        }
    }
}

///////////////////////////// Character 6 Ability //////////////////////////////

void PlayerAbility::C6(ctrl::wpPlayer const& player, wpMap const& self_map, wpMap const& enemy_map)
{
    ctrl::pPlayer p = player.lock();
    pMap m0 = self_map.lock();
    pMap m1 = enemy_map.lock();
    if( !p ) return;
    if( !m0 ) return;
    if( !m1 ) return;

    std::vector< data::pCube > m0_cube_data = m0->clone_linear_data();
    std::random_shuffle(m0_cube_data);

    int count = 0;

    int width = m1->map_setting()->width();
    int height= m1->map_setting()->height();
    for( size_t y = 0; y < height - 1 && count < 15; ++y ) {
        for( size_t x = 0; x < width && count < 15; ++x ) {
            if( !m1->exist(x, y) && !m1->below_is_dropping(x, y) ) {
                m1->make_cube(x, y, m0_cube_data[count]->color_id());
                ++count;
            }
        }
    }
}

///////////////////////////// Character 7 Ability //////////////////////////////

///////////////////////////// Character 8 Ability //////////////////////////////

