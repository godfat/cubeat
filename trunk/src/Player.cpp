
#include "Player.hpp"
#include "Weapon.hpp"
#include "Input.hpp"
#include "EventDispatcher.hpp"
#include "view/Sprite.hpp"
//#include "Sound.hpp"
#include <boost/foreach.hpp>

using namespace psc;
using namespace ctrl;
using std::tr1::bind;
using namespace std::tr1::placeholders;

Player::Player(Input* input, std::list<int> const& ally_ids, std::list<int> const& enemy_ids)
    :changetime_(500), changing_wep_(false), weplist_idx_(0), input_(input),
     ally_input_ids_(ally_ids), enemy_input_ids_(enemy_ids)
{
    weplist_.push_back( new BlockShoot() );
    weplist_.push_back( new PowerShoot() );
    weplist_.push_back( new AreaShoot() );

    current_wep_ = weplist_[0];  //pistol

    //tempcode
    weplist_[0]->reset();
    weplist_[1]->reset();
    weplist_[2]->reset();
}

pPlayer Player::init() {
    if( input_ ) {
        input_->player( shared_from_this() );
        EventDispatcher::i().subscribe_btn_event(
            bind(&Player::set_active_weapon, this, 0), &input_->wep1(), BTN_PRESS);
        EventDispatcher::i().subscribe_btn_event(
            bind(&Player::set_active_weapon, this, 1), &input_->wep2(), BTN_PRESS);
        EventDispatcher::i().subscribe_btn_event(
            bind(&Player::set_active_weapon, this, 2), &input_->wep3(), BTN_PRESS);

//        EventDispatcher::i().subscribe_btn_event(
//            bind(&Player::special_fire, this), &input_->trig2(), BTN_PRESS);
//      dont subscribe this! it will overwrite scene's picking!
//      maybe I should let different callee have parallel calling button and state...
//      do it when have time.

    }
    return shared_from_this();
}

Player::~Player()
{
    BOOST_FOREACH(Weapon* wp, weplist_)
        delete wp;
    weplist_.clear();
}

//need fix
Player& Player::update()
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
    return *this;
}

Player& Player::set_active_weapon(int i)
{
    std::cout << "switch weapon to " << i << "\n";
    current_wep_ = weplist_[i];
    weplist_idx_ = i;
    return *this;
}

Player& Player::disable_all_wep_reloadability()
{
    BOOST_FOREACH(Weapon* wp, weplist_)
        wp->reloadable(false);
    return *this;
}

// when the cube is RESTORED, consider:
//     player->subscribe_shot_event(body_, go_exploding, be_broken);

// when the cube is BROKEN, consider:
//     player->subscribe_shot_event(body_, restore, go_exploding);

// when the cube is GARBAGE, consider:
//     player->subscribe_shot_event(body_, go_exploding, 0);
//---------------------------------------------------------------------
// now we have 2 kind of firing method:
// as of trig1(), it will always be normal shoot, so actually no delegate hooking is needed:
//     sv->onPress( &input->trig1() ) = ally_cb; //when RESTORED | GARBAGE, this is go_exploding.
                                                 //when BROKEN, this is restore.
// and, enemy's trig1() will never affect you, so there's no subscription at all.

// as of trig2(), it will be a little complicated, we need delegate hooking,
// also, we have to consider enemy's trig2().
//     sv->onPress( &input->trig2() ) = bound_ally_cb; //when RESTORED | GARBAGE, this is go_exploding,
                                                       //when BROKEN, this is restore.
//     sv->onPress( &input->trig2() ) = bound_enemy_cb;
//     when RESTORED, this is be_broken IF AND ONLY IF enemy's weapon can crossfire.
//     when GARBAGE,  this does nothing. (dont bind at all)
//     when BROKEN,   this is go_exploding.


Player& Player::subscribe_shot_event
    (view::pSprite& sv, HitCallback const& ally_cb, HitCallback const& enemy_cb)
{
    BOOST_FOREACH(int& id, ally_input_ids_) {
        Input*  input = Input::getInputByIndex(id);
        pPlayer ally  = input->player();
        sv->onPress( &input->trig1() ) = bind(&Player::normal_shot_delegate, this, _1, ally_cb);
        //sv->onPress( &input->trig2() ) = bind(&Player::shot_delegate, this, _1, ally_cb, ally);

        //Maybe:
        sv->onHit( &input->trig2() ) = bind(&Player::shot_delegate, this, _1, ally_cb, ally);
    }

    if( enemy_cb ) {
        BOOST_FOREACH(int& id, enemy_input_ids_) {
            Input*  input = Input::getInputByIndex(id);
            pPlayer enemy = input->player();
            //sv->onPress( &input->trig2() ) = bind(&Player::shot_delegate, this, _1, enemy_cb, enemy);

            sv->onHit( &input->trig2() ) = bind(&Player::shot_delegate, this, _1, enemy_cb, enemy);
        }
    }
    return *this;
}

void Player::normal_shot_delegate
    (view::pSprite& sv, HitCallback const& hit_cb)
{
    hit_cb(1); //normal_shot's firepower is always 1.
}

void Player::shot_delegate
    (view::pSprite& sv, HitCallback const& hit_cb, pPlayer player)
{
    //if( player->can_fire() && !player->is_changing_wep() )  //now global button event handles this
        if( player->ally_input_ids_ == ally_input_ids_ || player->can_crossfire() )
            hit_cb( player->weapon()->firepower() ); // if the player is ally OR player can crossfire
}

void Player::repeating_shot_delegate
    (view::pSprite& sv, HitCallback const& hit_cb, pPlayer player)
{
    //now global button event handles this
    //if( player->can_fire_repeatedly() && player->can_fire() && !player->is_changing_wep() )
        if( player->ally_input_ids_ == ally_input_ids_ || player->can_crossfire() )
            hit_cb( player->weapon()->firepower() );
}

Weapon* Player::weapon()              const { return current_wep_; }
Weapon* Player::weapon(int id)        const { return weplist_[id]; }
bool Player::is_changing_wep()        const { return changing_wep_; }
bool Player::can_fire()               const { return current_wep_->can_fire(); }
bool Player::can_crossfire()          const { return current_wep_->can_crossfire(); }
bool Player::can_fire_repeatedly()    const { return current_wep_->can_fire_repeatedly(); }
int  Player::wepid()                  const { return weplist_idx_; }
bool Player::ammo_all_out() const {
    int count = 0;
    BOOST_FOREACH(Weapon* wp, weplist_)
        count += wp->ammo();
    return count == 0;
}

void Player::special_fire() { current_wep_->fire(); }

//need fix
//void Player::eat_item(Item const& item)
//{
//    weplist_[ item.type()+1 ]->reset();
//    Sound::i().item_box();
//}

//need fix
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
