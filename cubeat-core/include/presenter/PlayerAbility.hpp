#ifndef _SHOOTING_CUBES_PRESENTER_PLAYER_ABILITY_
#define _SHOOTING_CUBES_PRESENTER_PLAYER_ABILITY_

#include <boost/tr1/memory.hpp>

namespace psc {

namespace ctrl {
class Player;
typedef std::tr1::weak_ptr<Player> wpPlayer;
}

namespace presenter {

class Map;
typedef std::tr1::weak_ptr<Map> wpMap;

class PlayerAbility
{
public:
    typedef std::tr1::shared_ptr<PlayerAbility> pointer_type;
    virtual void invoke_ability(ctrl::wpPlayer const& player, wpMap const& map) = 0;
    virtual ~PlayerAbility();

protected:
    PlayerAbility();
    PlayerAbility(PlayerAbility const& other);
};
typedef PlayerAbility::pointer_type pPlayerAbility;

class Ability1 : public PlayerAbility
{
public:
    typedef std::tr1::shared_ptr<Ability1> pointer_type;
    static pointer_type create() {
        return pointer_type(new Ability1());
    }
    virtual void invoke_ability(ctrl::wpPlayer const& player, wpMap const& map);
    virtual ~Ability1();

protected:
    Ability1();
    Ability1(Ability1 const& other);
};
typedef Ability1::pointer_type pAbility1;

} //presenter
} //psc

#endif //_SHOOTING_CUBES_PRESENTER_PLAYER_ABILITY_
