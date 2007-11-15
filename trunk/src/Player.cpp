
/* 2007.11.9
   Most things remain unchanged.
   But the input button state part needs overhaul.

   These files are going to be heavily modified:
   1. Game.hpp
   2. MC.hpp
   3. Sound.hpp
*/

#include "Player.hpp"
#include "Game.hpp"
#include "MC.hpp"
#include "Debugger.hpp"

#include "Sound.hpp"

void Player::cycle()
{
	process_input();
	
	if( changedelay_ > 0 ) --changedelay_;
	
	if( current_wep_->cycle() ) {
		//do nothing
	}
	else {
		if( weplist_counter_ != 0 ) {
			Sound::i().change_wep();
			current_wep_ = weplist_[0];    //if the weapon is "DEAD" then we change it back to the most basic weapon
			weplist_counter_ = 0;	
		}
	}
}

void Player::eat_item(Item const& item)
{
	weplist_[ item.type()+1 ]->reset();
	Sound::i().item_box();
}

void Player::process_input()
{
    if( SG::Instance().is_paused() && button_select_down() ){
        SG::Instance().graceful_suicide();
        return;
    }

	if( button_start_pressed() && !SS::Instance().is_moving() ){
        SG::Instance().pause();
        return;
    }

	if( is_changing_wep() || SG::Instance().is_paused() ) return;
    
	if( button_b_down() && !button_a_down() ) {        //forward
		do {
            ++weplist_counter_;
		    if( weplist_counter_ >= 5 )
			    weplist_counter_ -= 5;
		} while( !weplist_[ weplist_counter_ ]->cycle() );
		    
		current_wep_ = weplist_[ weplist_counter_ ];
		changedelay_ = changetime_;
		Sound::i().change_wep();
	}

	else if( button_a_down() && !button_b_down() ) {   //backward
		do {
		    --weplist_counter_;
            if( weplist_counter_ < 0 )
                weplist_counter_ += 5;
		} while( !weplist_[ weplist_counter_ ]->cycle() );

		current_wep_ = weplist_[ weplist_counter_ ];
		changedelay_ = changetime_;
		Sound::i().change_wep();
	}
	
	if( button_c_down() || ( button_d_down() && !is_inbound() ) ) 
		current_wep_->reload();
}
