
#include "Player.hpp"
#include "model/Cube.hpp"
#include "presenter/PlayerAbility.hpp"
#include "presenter/Map.hpp"
#include "EventDispatcher.hpp"
#include "ctrl/TimerDispatcher.hpp"
#include "utils/Random.hpp"

#include <vector>
#include <algorithm>

using namespace psc;
using namespace presenter;
using std::tr1::bind;

///////////////////////////// some internal constant ///////////////////////////

static int restore_percentage_ = 100;
static int restore_percentage2_ = 100;
static int lock_dropping_second_ = 5000;
static double garbage_ratio_ = 0.333;
static int head_crush_layer_ = 5;
static int head_crush_layer_delay_ = 200;
static int magic_transfer_limit_ = 15;
static int broken_cube_limit_ = 15;
static int lock_heat_second_ = 15000;

void PlayerAbility::ability_modify(utils::map_any const& conf)
{
    restore_percentage_     = conf.I("restore_percentage");
    restore_percentage2_    = conf.I("restore_percentage2");
    lock_dropping_second_   = conf.I("lock_dropping_second");
    garbage_ratio_          = conf.F("garbage_ratio");
    head_crush_layer_       = conf.I("head_crush_layer");
    head_crush_layer_delay_ = conf.I("head_crush_layer_delay");
    magic_transfer_limit_   = conf.I("magic_transfer_limit");
    broken_cube_limit_      = conf.I("broken_cube_limit");
    lock_heat_second_       = conf.I("lock_heat_second");
}

///////////////////////////// Character 1 Ability //////////////////////////////

static void check_garbage_and_restore(model::pCube& c, int, int)
{
    if( utils::random(100) > restore_percentage_ ) return;
    if( !c->is_dying() && ( c->is_garbage() || c->is_broken() ) )
        c->restore(99); //99 means damage. originally all these functions can only pass weapon damage.
}

void PlayerAbility::C1(ctrl::wpPlayer const& player, wpMap const& self_map, wpMap const& enemy_map)
{
    if( ctrl::pPlayer p = player.lock() ) {
        if( pMap m0 = self_map.lock() ) {
            m0->foreach(check_garbage_and_restore);
        }
    }
}

///////////////////////////// Character 2 Ability //////////////////////////////

void PlayerAbility::C2(ctrl::wpPlayer const& player, wpMap const& self_map, wpMap const& enemy_map)
{
    pMap m0 = self_map.lock();
    if( !m0 ) return;

    m0->lock_dropping(true);

    ctrl::EventDispatcher::i().get_timer_dispatcher("game")->subscribe(
        bind(&Map::lock_dropping, m0.get(), false), m0, lock_dropping_second_);
}

///////////////////////////// Character 3 Ability //////////////////////////////

void PlayerAbility::C3(ctrl::wpPlayer const& player, wpMap const& self_map, wpMap const& enemy_map)
{
    if( ctrl::pPlayer p = player.lock() ) {
        if ( pMap m0 = self_map.lock() ) {
            m0->set_garbage_amount( static_cast<int>( m0->garbage_left() * garbage_ratio_ )  );
        }
    }
}

///////////////////////////// Character 4 Ability //////////////////////////////

static void check_color_and_restore(model::pCube& c, int, int, int color_2nd, int color_1st)
{
    if( utils::random(100) > restore_percentage2_ ) return;
    if( !c->is_dying() && c->color_id() == color_2nd )
        c->restore_to(color_1st);
}

void PlayerAbility::C4(ctrl::wpPlayer const& player, wpMap const& self_map, wpMap const& enemy_map)
{
    if( ctrl::pPlayer p = player.lock() ) {
        if( pMap m0 = self_map.lock() ) {
            // pair(color_id, count), and it's sorted using count
            std::vector< std::pair<int, int> > const& color_counts = m0->count_colors();
            int max_1st_color = color_counts[0].first;
            int max_2nd_color = color_counts[1].first;
            m0->foreach(bind(&check_color_and_restore, _1, _2, _3, max_2nd_color, max_1st_color));
        }
    }
}

///////////////////////////// Character 5 Ability //////////////////////////////

static void row_do(model::pCube& c, int, int)
{
    if( !c->is_dying() ) c->go_exploding(99); //99 means damage, see above note.
}

void PlayerAbility::C5(ctrl::wpPlayer const& player, wpMap const& self_map, wpMap const& enemy_map)
{
    if( ctrl::pPlayer p = player.lock() ) {
        if( pMap m0 = self_map.lock() ) {
            int delay_time = 1;
            int width = m0->map_setting()->width();
            int height= m0->map_setting()->height();
            for( int y = height - 1 ; y >= height - 1 - head_crush_layer_; --y ) {
                for( int x = 0; x < width; ++x ) {
                    if( m0->exist(x, y) ) {
                        ctrl::EventDispatcher::i().get_timer_dispatcher("game")->subscribe(
                            bind(&Map::for_row, m0.get(), y, row_do), m0, delay_time);
                        delay_time += head_crush_layer_delay_;
                        break; //FUCK, if forgetting breaking here.............
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
    std::random_shuffle(m0_cube_data.begin(), m0_cube_data.end());

    int count = 0;
    int k = 0;

    int width = m1->map_setting()->width();
    int height= m1->map_setting()->height();
    for( int y = 0; y < height - 1 && count < magic_transfer_limit_; ++y ) {
        for( int x = 0; x < width && count < magic_transfer_limit_; ++x ) {
            if( !m1->exist(x, y) && !m1->below_is_dropping(x, y) ) {
                while( k < m0_cube_data.size() && m0_cube_data[k]->color_id() != -1 ) { ++k; }
                if( k >= m0_cube_data.size() ) return;
                m1->new_cube_at(x, y, m0_cube_data[k]->color_id());
                m0->kill_cube_at( m0_cube_data[k]->x(), m0_cube_data[k]->y() );
                ++count;
                ++k;
            }
        }
    }
    for( ; k < m0_cube_data.size() && count < magic_transfer_limit_ ; ++k ) {
        if( m0_cube_data[k]->color_id() == -1 ) {
            m0->kill_cube_at( m0_cube_data[count]->x(), m0_cube_data[count]->y() );
            ++count;
        }
    }
}

///////////////////////////// Character 7 Ability //////////////////////////////

static void cube_broken(model::pCube& c, int, int)
{
    if( !c->is_dying() && !c->is_broken() )
        c->be_broken(99); //99 means damage
}

void PlayerAbility::C7(ctrl::wpPlayer const& player, wpMap const& self_map, wpMap const& enemy_map)
{
    ctrl::pPlayer p = player.lock();
    pMap m1 = enemy_map.lock();
    if( !p ) return;
    if( !m1 ) return;

    std::vector< data::pCube > m1_cube_data = m1->clone_linear_data();
    std::random_shuffle(m1_cube_data.begin(), m1_cube_data.end());

    for( size_t i = 0; i < m1_cube_data.size() && i < static_cast<size_t>(broken_cube_limit_) ; ++i ) {
        m1->apply_func_at(m1_cube_data[i]->x(), m1_cube_data[i]->y(), cube_broken);
    }
}

///////////////////////////// Character 8 Ability //////////////////////////////

void PlayerAbility::C8(ctrl::wpPlayer const& player, wpMap const& self_map, wpMap const& enemy_map)
{
    ctrl::pPlayer p = player.lock();
    if( !p ) return;

    p->lock_heat(true);

    ctrl::EventDispatcher::i().get_timer_dispatcher("game")->subscribe(
        bind(&ctrl::Player::lock_heat, p.get(), false), p, lock_heat_second_);
}
