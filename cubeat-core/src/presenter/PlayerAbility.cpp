
#include "Player.hpp"
#include "presenter/PlayerAbility.hpp"
#include "presenter/Map.hpp"

using namespace psc;
using namespace presenter;

PlayerAbility::PlayerAbility() {}
PlayerAbility::~PlayerAbility() {}

///////////////////////////// Character 1 Ability //////////////////////////////

Ability1::Ability1() : PlayerAbility() {}
Ability1::~Ability1() {}

void Ability1::invoke_ability(ctrl::wpPlayer const& player, wpMap const& map)
{
    if( ctrl::pPlayer p = player.lock() ) {
        if ( pMap m = map.lock() ) {
            m->set_garbage_amount( m->garbage_left() / 3 );
        }
    }
}


