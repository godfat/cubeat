
#ifndef _SHOOTING_CUBES_CTRL_WEAPON_
#define _SHOOTING_CUBES_CTRL_WEAPON_

//#include "Sound.hpp"

namespace psc {
namespace ctrl {

class Weapon
{
public:
	Weapon(int const& firepower = 0, int const& ammo = 0, int const& max_ammo = 0,
	       bool const& reloadable = true, bool const& reloadtime = 500,
           bool const& repeat = false, int const& cooldowntime = 100)
		:ammo_(ammo), max_ammo_(max_ammo), reloadable_(reloadable), reloading_(false),
		 reloadtime_(reloadtime), firepower_(firepower), repeat_(repeat),
		 coolingdown_(false), cooldowntime_(cooldowntime)
	{}

	virtual void fire() = 0;       //trigger fire

	void reload() {
		if( reloadable_ == true && reloading_ == false ) {
			reloading_ = true;
			//Sound::i().reload();
			//call reset() by timer
		}
	}

	void reset() {
		reloading_ = false;
        ammo_ = max_ammo_;
	}

	bool update() {
		if( reloadable_ == false && ammo_ == 0 ) return false;
		else return true;
	}

    int  ammo()           const { return ammo_; }
    int  max_ammo()       const { return max_ammo_; }
	int  firepower()      const { return firepower_; }
	int  reloadtime()     const { return reloadtime_; }
	bool reloadable()     const { return reloadable_; }
    bool is_reloading()   const { return reloading_; }
	bool is_coolingdown() const { return coolingdown_; }
	void reloadable(bool const& in){ reloadable_ = in; }
	void ammo(int const& in)       { ammo_ = in; }

	bool can_fire() { return !is_reloading() && !is_coolingdown() && ammo() > 0; }
	bool can_fire_repeatedly() { return repeat_; }

protected:
	int  ammo_;
	int  max_ammo_;
	bool reloadable_;
	bool reloading_;
	int  reloadtime_;
	int  firepower_;
	bool repeat_;
	bool coolingdown_;
	int  cooldowntime_;
};


//Pistol
class Pistol : public Weapon
{
public:
	Pistol():Weapon(1, 6, 6) {}

	virtual void fire() {
		if( ammo_ > 0 && reloading_ == false ) {
			--ammo_;
			//Sound::i().pistol();
			//use Timer to refresh coolingdown_
		}
	}
};

//HeavyPistol
class HeavyPistol : public Weapon
{
public:
	HeavyPistol():Weapon(3, 0, 6, false, 9999){}

	virtual void fire() {
		if( ammo_ > 0 && reloading_ == false ) {
			--ammo_;
			//Sound::i().heavypistol();
			//use Timer to refresh coolingdown_
		}
	}
};

//MachineGun
class MachineGun : public Weapon
{
public:
	MachineGun():Weapon(1, 0, 15, false, 9999, true, 100) {}

	virtual void fire() {
		if( ammo_ > 0 && reloading_ == false ) {
			--ammo_;
			//Sound::i().machinegun();
			//use Timer to refresh coolingdown_
		}
	}
};

//Heavy Mach
class HeavyMachineGun : public Weapon
{
public:
	HeavyMachineGun():Weapon(1, 0, 30, false, 9999, true, 200) {}

	virtual void fire() {
		if( ammo_ > 0 && reloading_ == false ) {
			--ammo_;
			//Sound::i().heavymachinegun();
			//use Timer to refresh coolingdown_
		}
	}
};

//Rocket
class Rocket : public Weapon
{
public:
	Rocket():Weapon(3, 0, 1, false, 9999) {}

	virtual void fire() {
		if( ammo_ > 0 && reloading_ == false ) {
			--ammo_;
			//Sound::i().rocket();
			//use Timer to refresh coolingdown_
		}
	}
};

} //ctrl
} //psc

#endif //_SHOOTING_CUBES_CTRL_WEAPON_
