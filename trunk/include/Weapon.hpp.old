
/* 2007.11.9
  Most things remain unchanged.

  These included file are going to be modified:
  1. Sound.hpp
*/

#ifndef _SHOOTING_CUBE_WEAPON_
#define _SHOOTING_CUBE_WEAPON_

#include "Debugger.hpp"
#include "Sound.hpp"

class Weapon
{
public:
	Weapon() 
	{
		ammo_ = 0;
		max_ammo_ = 0;
		repeat_ = false;
		reloadtime_ = 15;
		reloaddelay_ = reloadtime_;
		cooldowntime_ = 0;
		cooldowndelay_ = 0;
		reloadable_ = true;
		reloading_ = false;
		firepower_ = 0;
	}
	
	virtual void fire() = 0;       //trigger fire

    void set_reloadable(bool reloadable){
        reloadable_ = reloadable;
    }
	
	void reload()     //trigger reload
	{     
		if( reloadable_ == true && reloading_ == false ) {
			reloading_ = true;
			Sound::i().reload();
		}
	}

	void reset()
	{
		reloading_ = false;
        ammo_ = max_ammo_;
		reloaddelay_ = reloadtime_;
	}
	
	bool cycle()      //delay cycle
	{        
		if( reloading_ == true ) {
			if( reloaddelay_ > 0 ) {
				Debugger::i().trace("Weapon is reloading");
				--reloaddelay_;
			}
			else reset();
		}
		
		if( cooldowndelay_ > 0 ) {
			Debugger::i().trace("Weapon is cooling down");
			--cooldowndelay_;
		}
		
		if( reloadable_ == false && ammo_ == 0 ) return false;
		else return true;
	}
	
	int firepower() { return firepower_; }
	bool reloadable() { return reloadable_; }
	void reloadable(bool in) { reloadable_ = in; }
	int delaytime() { return reloaddelay_; }
	bool is_reloading() { return reloading_; }
	bool is_coolingdown() { return cooldowndelay_ > 0; }
	int ammo() { return ammo_; }
	void ammo(int in) { ammo_ = in; }
	int max_ammo() { return max_ammo_; }
	
	bool can_fire() { return !is_reloading() && !is_coolingdown() && ammo() > 0; }
	bool can_fire_repeatedly() { return repeat_; }

	virtual void dummy() {}

	//std::string toString() { return "Something wrong"; }
	
protected:
	int ammo_;
	int max_ammo_;
	bool reloadable_;
	bool reloading_;
	int reloadtime_;
	int reloaddelay_;
	int firepower_;
	bool repeat_;
	int cooldowntime_;
	int cooldowndelay_;
};


//Pistol 
class Pistol : public Weapon
{
public:
	Pistol() {
		ammo_ = 6;
		max_ammo_ = 6;
		repeat_ = false;
		reloadtime_ = 15;
		reloaddelay_ = reloadtime_;
		cooldowntime_ = 0;
		cooldowndelay_ = 0;
		reloadable_ = true;
		reloading_ = false;
		firepower_ = 1;
	}

	virtual void fire()       //trigger fire
    { 
		if( ammo_ > 0 && reloading_ == false ) {
			--ammo_;
			cooldowndelay_ = cooldowntime_;
			Sound::i().pistol();
		}
	}
};

//HeavyPistol
class HeavyPistol : public Weapon
{
public:
	HeavyPistol() {
		ammo_ = 0;
		max_ammo_ = 6;
		repeat_ = false;
		reloadtime_ = 100000;
		reloaddelay_ = reloadtime_;
		cooldowntime_ = 0;
		cooldowndelay_ = 0;
		reloadable_ = false;
		reloading_ = false;
		firepower_ = 3;
	}

	virtual void fire()       //trigger fire
    { 
		if( ammo_ > 0 && reloading_ == false ) {
			--ammo_;
			cooldowndelay_ = cooldowntime_;
			Sound::i().heavypistol();
		}
	}
};

//MachineGun
class MachineGun : public Weapon
{
public:
	MachineGun() {
		ammo_ = 0;
		max_ammo_ = 15;
		repeat_ = true;
		reloadtime_ = 30;
		reloaddelay_ = reloadtime_;
		cooldowntime_ = 6;
		cooldowndelay_ = 0;
		reloadable_ = false;
		reloading_ = false;
		firepower_ = 1;
	}

	virtual void fire()       //trigger fire
    { 
		if( ammo_ > 0 && reloading_ == false ) {
			--ammo_;
			cooldowndelay_ = cooldowntime_;
			Sound::i().machinegun();
		}
	}
};

//Heavy Mach
class HeavyMachineGun : public Weapon
{
public:
	HeavyMachineGun() {
		ammo_ = 0;
		max_ammo_ = 30;
		repeat_ = true;
		reloadtime_ = 100000;
		reloaddelay_ = reloadtime_;
		cooldowntime_ = 6;
		cooldowndelay_ = 0;
		reloadable_ = false;
		reloading_ = false;
		firepower_ = 1;
	}

	virtual void fire()       //trigger fire
    { 
		if( ammo_ > 0 && reloading_ == false ) {
			--ammo_;
			cooldowndelay_ = cooldowntime_;
			Sound::i().heavymachinegun();
		}
	}
};

//Rocket
class Rocket : public Weapon
{
public:
	Rocket() {
		ammo_ = 0;
		max_ammo_ = 1;
		repeat_ = false;
		reloadtime_ = 100000;
		reloaddelay_ = reloadtime_;
		cooldowntime_ = 0;
		cooldowndelay_ = 0;
		reloadable_ = false;
		reloading_ = false;
		firepower_ = 3;
	}

	virtual void fire()       //trigger fire
    { 
		if( ammo_ > 0 && reloading_ == false ) {
			--ammo_;
			cooldowndelay_ = cooldowntime_;
			Sound::i().rocket();
		}
	}
};

#endif