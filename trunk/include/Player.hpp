

#ifndef _SHOOTING_CUBES_CTRL_PLAYER_
#define _SHOOTING_CUBES_CTRL_PLAYER_

#include "Weapon.hpp"
#include <vector>

//class Item;

namespace psc {
namespace ctrl {

class Player
{
public:
	Player()
	    :changetime_(500), changing_wep_(false)
	{
		weplist_.push_back( new Pistol() );
		weplist_.push_back( new HeavyPistol() );
		weplist_.push_back( new MachineGun() );
		weplist_.push_back( new HeavyMachineGun() );
		weplist_.push_back( new Rocket() );

		current_wep_ = weplist_[0];  //pistol
		weplist_idx_ = 0;
	}

	~Player()
	{
		for( unsigned int i=0; i < weplist_.size(); i++ )
			delete weplist_[i];
	}

	void update();
	void process_input();

	//I'd better refactor this afterwards.
	//void eat_item(Item const& item);

	Weapon* weapon()              const { return current_wep_; }
	Weapon* weapon(int const& id) const { return weplist_[id]; }
	bool is_changing_wep()        const { return changing_wep_; }
	int  wepid()                  const { return weplist_idx_; }

    void set_active_weapon(int const& i) {
        current_wep_ = weplist_[i];
        weplist_idx_ = i;
    }

	bool weapon_empty() {
		int count = 0;
		for( int i = 0; i < 5; i++ )
			count += weplist_[i]->ammo();

		return count == 0;
	}

    void disable_all_wep_reloadability(){
        for(std::vector<Weapon*>::iterator i=weplist_.begin(), iend=weplist_.end();
            i!=iend; ++i)
        {
            (**i).reloadable(false);
        }
    }

private:
	int const changetime_;
	bool      changing_wep_;
    int       weplist_idx_;

	Weapon*   current_wep_;
	std::vector<Weapon*> weplist_;
};

} //ctrl
} //psc

#endif //_SHOOTING_CUBES_CTRL_PLAYER_
