
#include "Player.hpp"
//#include "Game.hpp"
//#include "Sound.hpp"

using namespace psc;
using namespace ctrl;

void Player::update()
{
    process_input();

    if( current_wep_->update() ) {
        //do nothing
    }
    else {
        if( weplist_idx_ != 0 ) {
            //Sound::i().change_wep();
            current_wep_ = weplist_[0];
            //if the weapon is "DEAD" then we change it back to the most basic weapon
            weplist_idx_ = 0;
        }
    }
}

//void Player::eat_item(Item const& item)
//{
//    weplist_[ item.type()+1 ]->reset();
//    Sound::i().item_box();
//}

void Player::process_input()
{
//    changing weapon, using timer call. no more step delay

//    if( SG::Instance().is_paused() && button_select_down() ){
//        SG::Instance().graceful_suicide();
//        return;
//    }
//
//    if( button_start_pressed() && !SS::Instance().is_moving() ){
//        SG::Instance().pause();
//        return;
//    }
//
//    if( is_changing_wep() || SG::Instance().is_paused() ) return;
}
