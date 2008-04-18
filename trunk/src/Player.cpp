
#include "Player.hpp"
#include "Weapon.hpp"
#include "Input.hpp"
#include "EventDispatcher.hpp"
#include "view/Sprite.hpp"
#include "view/SFX.hpp"
#include "Sound.hpp"
#include "utils/Random.hpp"
#include <boost/foreach.hpp>

using namespace psc;
using namespace ctrl;
using std::tr1::bind;
using namespace std::tr1::placeholders;

Player::Player(Input* input, std::list<int> const& ally_ids, std::list<int> const& enemy_ids)
    :changetime_(500), changing_wep_(false), weplist_idx_(0), input_(input),
     ally_input_ids_(ally_ids), enemy_input_ids_(enemy_ids)
{
}

pPlayer Player::init()
{
    weplist_.push_back( new BlockShoot( shared_from_this() ) );
    weplist_.push_back( new PowerShoot( shared_from_this() ) );
    weplist_.push_back( new AreaShoot( shared_from_this() ) );

    current_wep_ = weplist_[0];

    if( input_ ) {
        input_->player( shared_from_this() );
        EventDispatcher::i().subscribe_btn_event(
            bind(&Player::set_active_weapon, this, 0), shared_from_this(), &input_->wep1(), BTN_PRESS);
        EventDispatcher::i().subscribe_btn_event(
            bind(&Player::set_active_weapon, this, 1), shared_from_this(), &input_->wep2(), BTN_PRESS);
        EventDispatcher::i().subscribe_btn_event(
            bind(&Player::set_active_weapon, this, 2), shared_from_this(), &input_->wep3(), BTN_PRESS);

        EventDispatcher::i().subscribe_btn_event(
            bind(&Player::normal_weapon_fx, this), shared_from_this(), &input_->trig1(), BTN_PRESS);

//note: maybe I should let different callee have parallel calling button and state...
//      do it when have time.

    }
    return shared_from_this();
}

Player::~Player()
{
    input_->setRangeShapeVisible(false);
    BOOST_FOREACH(Weapon* wp, weplist_)
        delete wp;
    weplist_.clear();
}

//note: need fix
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
    if( i == 2 )  //temp: write it dead for now. will expand and refactor it later.
        input_->setRangeShapeVisible(true);
    else
        input_->setRangeShapeVisible(false);
    Sound::i().play("1/g/09.mp3");
    return *this;
}

Player& Player::debug_reset_all_weapon()
{
    BOOST_FOREACH(Weapon* wp, weplist_)
        wp->reset();
    return *this;
}

Player& Player::disable_all_wep_reloadability()
{
    BOOST_FOREACH(Weapon* wp, weplist_)
        wp->reloadable(false);
    return *this;
}

Player& Player::subscribe_shot_event
    (view::pSprite& sv, HitCallback const& ally_cb, HitCallback const& enemy_cb)
{
    BOOST_FOREACH(int& id, ally_input_ids_) {
        Input*  input = Input::getInputByIndex(id);
        pPlayer ally  = input->player();
        sv->onPress( &input->trig1() ) = bind(&Player::normal_shot_delegate, this, _1, ally_cb);
        sv->onHit( &input->trig2() ) = bind(&Player::shot_delegate, this, _1, ally_cb, ally);
    }

    if( enemy_cb ) {
        BOOST_FOREACH(int& id, enemy_input_ids_) {
            Input*  input = Input::getInputByIndex(id);
            pPlayer enemy = input->player();
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
    if( player->ally_input_ids_ == ally_input_ids_ || player->can_crossfire() )
        hit_cb( player->weapon()->firepower() ); // if the player is ally OR player can crossfire
}

// note: no use for now?
//void Player::repeating_shot_delegate
//    (view::pSprite& sv, HitCallback const& hit_cb, pPlayer player)
//{
//    if( player->ally_input_ids_ == ally_input_ids_ || player->can_crossfire() )
//        hit_cb( player->weapon()->firepower() );
//}

Input const* Player::input()          const { return input_; }
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

//temp: not flexible and stupid.
void Player::normal_weapon_fx() {
    Sound::i().play("1/a/1a-1.mp3");
    view::SFX::i().normal_weapon_vfx( Input::scene(), vec2(input_->cursor().x(), input_->cursor().y()) );
}

//note: need fix
void Player::eat_item()
{
    int percent = utils::random(100);
    if( percent < 45 ) {
        weplist_[0]->ammo( weplist_[0]->ammo()+10 );
    }
    else if( percent < 90 ) {
        weplist_[1]->ammo( weplist_[1]->ammo()+10 );
    }
    else {
        weplist_[2]->ammo( weplist_[2]->ammo()+1 );
    }
    Sound::i().play("1/e/getitem.mp3");
}

//note: need fix
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
