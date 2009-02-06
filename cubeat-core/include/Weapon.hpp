
#ifndef _SHOOTING_CUBES_CTRL_WEAPON_
#define _SHOOTING_CUBES_CTRL_WEAPON_

#include "data/BasicViewTypes.hpp"
#include <tr1/functional>
#include <tr1/memory>

namespace psc {
namespace ctrl {

class Player;
typedef std::tr1::shared_ptr<Player> pPlayer;
typedef std::tr1::weak_ptr<Player> wpPlayer;

typedef std::tr1::function<bool(vec2 const&, vec2 const&)> AreaPredicate;

class Weapon
{
public:
    Weapon(wpPlayer const& p, int const& firepower = 0, int const& ammo = 0, int const& max_ammo = 0,
           bool const& crossfire = false, bool const& reloadable = true,
           bool const& reloadtime = 500, bool const& repeat = false, int const& cooldowntime = 100)
        :player_(p), firepower_(firepower), ammo_(ammo), max_ammo_(max_ammo), crossfire_(crossfire),
         reloadable_(reloadable), reloading_(false), reloadtime_(reloadtime),
         repeat_(repeat), coolingdown_(false), cooldowntime_(cooldowntime)
    {}

    virtual void fire(vec2 const&) = 0;       //trigger fire

    void reload();
    void reset() {
        reloading_ = false;
        ammo_ = max_ammo_;
    }
    bool update() {
        if( reloadable_ == false && ammo_ == 0 ) return false;
        else return true;
    }
    int  firepower()      const { return firepower_; }
    int  ammo()           const { return ammo_; }
    int  max_ammo()       const { return max_ammo_; }
    bool can_crossfire()  const { return crossfire_; }
    int  reloadtime()     const { return reloadtime_; }
    bool reloadable()     const { return reloadable_; }
    bool is_reloading()   const { return reloading_; }
    bool is_coolingdown() const { return coolingdown_; }
    void reloadable(bool const& in){ reloadable_ = in; }
    void ammo(int const& in) {
        ammo_ = in;
        if( ammo_ > max_ammo_ )
            ammo_ = max_ammo_;
    }

    bool can_fire() { return !is_reloading() && !is_coolingdown() && ammo() > 0; }
    bool can_fire_repeatedly() { return repeat_; }

    virtual AreaPredicate areaPredicate() { return 0; }  //default no predicate. use point picking

protected:
    wpPlayer player_;

    int  firepower_;
    int  ammo_;
    int  max_ammo_;
    bool crossfire_;
    bool reloadable_;
    bool reloading_;
    int  reloadtime_;
    bool repeat_;
    bool coolingdown_;
    int  cooldowntime_;
};

class BlockShoot : public Weapon
{
public:
    BlockShoot(wpPlayer const&);
    virtual void fire(vec2 const&);
};

class PowerShoot : public Weapon
{
public:
    PowerShoot(wpPlayer const&);
    virtual void fire(vec2 const&);
};

class AreaShoot : public Weapon
{
public:
    AreaShoot(wpPlayer const&);
    virtual void fire(vec2 const&);
    virtual AreaPredicate areaPredicate();
protected:
    bool area(vec2 const&, vec2 const&, int, int);
};

} //ctrl
} //psc

#endif //_SHOOTING_CUBES_CTRL_WEAPON_
