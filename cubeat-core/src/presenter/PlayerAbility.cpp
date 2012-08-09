
#include "Player.hpp"
#include "presenter/PlayerAbility.hpp"
#include "presenter/Map.hpp"

using namespace psc;
using namespace presenter;

///////////////////////////// Character 1 Ability //////////////////////////////

void PlayerAbility::C1(ctrl::wpPlayer const& player, wpMap const& self_map, wpMap const& enemy_map)
{

}

///////////////////////////// Character 2 Ability //////////////////////////////

void PlayerAbility::C2(ctrl::wpPlayer const& player, wpMap const& self_map, wpMap const& enemy_map)
{

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

void PlayerAbility::C4(ctrl::wpPlayer const& player, wpMap const& self_map, wpMap const& enemy_map)
{

}

///////////////////////////// Character 5 Ability //////////////////////////////

void PlayerAbility::C5(ctrl::wpPlayer const& player, wpMap const& self_map, wpMap const& enemy_map)
{

}

///////////////////////////// Character 6 Ability //////////////////////////////

void PlayerAbility::C6(ctrl::wpPlayer const& player, wpMap const& self_map, wpMap const& enemy_map)
{

}


