#ifndef _SHOOTING_CUBES_PRESENTER_PLAYER_ABILITY_
#define _SHOOTING_CUBES_PRESENTER_PLAYER_ABILITY_

#include <boost/tr1/memory.hpp>
#include <boost/tr1/functional.hpp>

namespace psc {

namespace ctrl {
class Player;
typedef std::tr1::weak_ptr<Player> wpPlayer;
}

namespace presenter {

class Map;
typedef std::tr1::weak_ptr<Map> wpMap;

struct PlayerAbility
{
    typedef std::tr1::function<void(ctrl::wpPlayer const& player, wpMap const& self_map, wpMap const& enemy_map)>
        Callback;

    static void C1(ctrl::wpPlayer const& player, wpMap const& self_map, wpMap const& enemy_map);
    static void C2(ctrl::wpPlayer const& player, wpMap const& self_map, wpMap const& enemy_map);
    static void C3(ctrl::wpPlayer const& player, wpMap const& self_map, wpMap const& enemy_map);
    static void C4(ctrl::wpPlayer const& player, wpMap const& self_map, wpMap const& enemy_map);
    static void C5(ctrl::wpPlayer const& player, wpMap const& self_map, wpMap const& enemy_map);
    static void C6(ctrl::wpPlayer const& player, wpMap const& self_map, wpMap const& enemy_map);
};

} //presenter
} //psc

#endif //_SHOOTING_CUBES_PRESENTER_PLAYER_ABILITY_
