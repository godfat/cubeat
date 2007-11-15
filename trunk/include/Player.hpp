
/* 2007.11.9
   Most things remain unchanged.
   But the input button state part needs overhaul.

   These files are going to be modified:
   1. Item.hpp
*/


#ifndef _SHOOTING_CUBE_PLAYER_
#define _SHOOTING_CUBE_PLAYER_

#include "Weapon.hpp"
#include "Item.hpp"
#include <vector>

class Player 
{
public:
	Player( /*Game const& game*/ ) : changetime_(15)
	{
		//game_ = game;
		changedelay_ = 0;
		trigger_ = false;
		
		weplist_.push_back( new Pistol() );
		weplist_.push_back( new HeavyPistol() );
		weplist_.push_back( new MachineGun() );
		weplist_.push_back( new HeavyMachineGun() );
		weplist_.push_back( new Rocket() );
		
		current_wep_ = weplist_[0];  //pistol
		weplist_counter_ = 0;
	}

	~Player()
	{
		for( unsigned int i=0; i < weplist_.size(); i++ )
			delete weplist_[i];
	}

	void cycle();

	void process_input();
	
	//I'd better refactor this afterwards.
	void eat_item(Item const& item);

	bool is_changing_wep() { return changedelay_ > 0; }
	Weapon* weapon()       { return current_wep_; }
	Weapon* weapon(int id) { return weplist_[id]; }
	int wepid()            { return weplist_counter_; }

	void set_trigger( bool in ) { trigger_ = in; }
	bool triggered() { return trigger_; }

	void set_all_button( /* Signature changed */ )
	{
		return 0;
	}

    void set_active_weapon(int i) { current_wep_ = weplist_[ i ]; weplist_counter_ = i; }
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
            (**i).set_reloadable(false);
        }
    }

private:
	const int changetime_;
	int       changedelay_;
    bool      trigger_;

	Weapon*   current_wep_;
	std::vector<Weapon*> weplist_;
	int weplist_counter_;

	bool is_inbound()    { return inbound_;  }

    /*
	bool button_a_down() { return a_; }
	bool button_b_down() { return b_; }
	bool button_c_down() { return c_; }
	bool button_d_down() { return d_; }

	bool button_up_down()    { return up_; }
	bool button_right_down() { return right_; }
	bool button_down_down()  { return down_; }
	bool button_left_down()  { return left_; }

	bool button_start_down() { return start_; }
	bool button_select_down(){ return select_; } */

    bool button_start_pressed(){
        if( start_ ){
            if( start_pressed_ )
                return false;
            else
                return start_pressed_ = true;
        }
        else{
            return start_pressed_ = false;
        }
    }
 
    /*
	bool inbound_;
	bool a_;
	bool b_;
	bool c_;
	bool d_;
	bool up_;
	bool right_;
	bool down_;
	bool left_; */
	bool start_;
	//bool select_;

    bool start_pressed_;
};

#endif