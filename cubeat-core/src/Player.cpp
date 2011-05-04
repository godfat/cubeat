
#include "Player.hpp"
#include "Weapon.hpp"
#include "Input.hpp"
#include "EventDispatcher.hpp"
#include "view/Sprite.hpp"
#include "view/SFX.hpp"
#include "audio/Sound.hpp"
#include "utils/Random.hpp"
#include "presenter/Map.hpp"
#include "Accessors.hpp"        //for some basic visual effects
#include "EasingEquations.hpp"  //for some basic visual effects
#include <boost/foreach.hpp>

using namespace psc;
using namespace ctrl;
using std::tr1::bind;
using namespace std::tr1::placeholders;

Player::Player(Input* input, int const& id)
    :id_(id), changetime_(500), changing_wep_(false), weplist_idx_(0), accumulated_heat_(0),
     cooling_speed_(0.06), heat_for_normal_shoot_(0.16), heat_for_haste_(0.03), heat_for_jama_shoot_(0.25),
     overheat_downtime_(2000), overheat_(false), hasting_(false), input_(input), player_hit_event_(0)
{
}

pPlayer Player::init(bool const& can_haste)
{
    weplist_.push_back( new BlockShoot( shared_from_this() ) );
    weplist_.push_back( new PowerShoot( shared_from_this() ) );
    weplist_.push_back( new AreaShoot( shared_from_this() ) );
    current_wep_ = weplist_[0];

    if( input_ )
        input_->player( shared_from_this() );

    subscribe_player_specific_interactions(can_haste);

    EventDispatcher::i().subscribe_timer(
        bind(&Player::heat_cooling, this), shared_from_this(), 100, -1); //check for cooling every 100ms

    return shared_from_this();
}

Player::~Player()
{
    std::cout << "  ctrl::player (related input " << input_ << ") destructing..." << std::endl;
    input_->setRangeShapeVisible(false);
    BOOST_FOREACH(Weapon* wp, weplist_)
        delete wp;
    weplist_.clear();
}

//2011.03.28 remove old commented code here.
void Player::cycle()
{
}

//2011.03.28 This will provide a modifiable speed for ViewSprite
float Player::haste_speedfunc(float orig_speed) const
{
    return hasting_ ? 450.f : orig_speed;
}

void Player::heat_cooling()
{
    if( !overheat_ ) { //2011.03.28 when hasting you shouldn't cool
        if( !hasting_ ) {
            if( accumulated_heat_ > 0 ) {
                accumulated_heat_ -= cooling_speed_;
                if( accumulated_heat_ < 0 )
                    accumulated_heat_ = 0;
            }
        }
        else {
            generate_heat(heat_for_haste_);
        }
    }
}

Player& Player::subscribe_player_specific_interactions(bool const& can_haste)
{
    if( input_ ) {
        //2011.03.25 weapon feature removed temporarily
        //EventDispatcher::i().subscribe_btn_event(
        //    bind(&Player::set_active_weapon, this, 0), shared_from_this(), &input_->wep1(), BTN_PRESS);
        //EventDispatcher::i().subscribe_btn_event(
        //    bind(&Player::set_active_weapon, this, 1), shared_from_this(), &input_->wep2(), BTN_PRESS);
        //EventDispatcher::i().subscribe_btn_event(
        //    bind(&Player::set_active_weapon, this, 2), shared_from_this(), &input_->wep3(), BTN_PRESS);

        EventDispatcher::i().subscribe_btn_event(
            bind(&Player::normal_weapon_fx, this), shared_from_this(), &input_->trig1(), BTN_PRESS);
        if( can_haste ) {
            EventDispatcher::i().subscribe_btn_event(
                bind(&Player::start_haste_effect, this), shared_from_this(), &input_->trig2(), BTN_PRESS);
            EventDispatcher::i().subscribe_btn_event(
                bind(&Player::remove_haste_effect, this), shared_from_this(), &input_->trig2(), BTN_RELEASE);
        }
//note: maybe I should let different callee have parallel calling button and state...
//      do it when have time.

    }
    return *this;
}

Player& Player::set_config(utils::map_any const& config)
{
    weplist_[0]->ammo( config.I("item1_start_ammo") );
    weplist_[1]->ammo( config.I("item2_start_ammo") );
    weplist_[2]->ammo( config.I("item3_start_ammo") );
    cooling_speed_    = config.F("cool_rate");
    heat_for_normal_shoot_ = config.F("heat_rate");
    overheat_downtime_= config.I("downtime");
    heat_for_haste_   = config.F("heat_for_haste");
    heat_for_jama_shoot_ = config.F("heat_for_jama");
    return *this;
}

Player& Player::set_active_weapon(int i)
{
    std::cout << "switch weapon to " << i << std::endl;
    current_wep_ = weplist_[i];
    weplist_idx_ = i;
    if( i == 2 )  //temp: write it dead for now. will expand and refactor it later.
        input_->setRangeShapeVisible(true);
    else
        input_->setRangeShapeVisible(false);
    audio::Sound::i().playBuffer("1/g/09.wav");
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

Player& Player::push_ally(int id)
{
    ally_input_ids_.push_back(id);
    return *this;
}

Player& Player::push_enemy(int id)
{
    enemy_input_ids_.push_back(id);
    return *this;
}

Player& Player::subscribe_shot_event
    (view::pSprite& sv, HitCallback const& ally_cb, HitCallback const& enemy_cb)
{
    BOOST_FOREACH(int const& id, ally_input_ids_) {
        Input*  input = InputMgr::i().getInputByIndex(id);
        wpPlayer ally  = input->player();
        sv->onPress( &input->trig1() ) = bind(&Player::normal_shot_delegate, this, _1, ally_cb);
        //sv->onHit( &input->trig2() ) = bind(&Player::shot_delegate, this, _1, ally_cb, ally); 2011.03.25 weapon remove
    }

    if( enemy_cb ) {
        BOOST_FOREACH(int const& id, enemy_input_ids_) {
            Input*  input = InputMgr::i().getInputByIndex(id);
            wpPlayer enemy = input->player();
            //sv->onHit( &input->trig2() ) = bind(&Player::shot_delegate, this, _1, enemy_cb, enemy); 2011.03.25 weapon remove
            sv->onPress( &input->trig1() ) = bind(&Player::shot_delegate, this, _1, enemy_cb, enemy);
        }
    }
    return *this;
}

void Player::end_overheat()
{
    overheat_ = false;
    if( presenter::pMap m = map_list_[id_].lock() ) {
        m->overheat_event()(false);
    }
}

void Player::generate_heat(double heat)
{
    using std::tr1::ref;
    accumulated_heat_ += heat;
    if( accumulated_heat_ > 1 ) {
        accumulated_heat_ = 1;
        overheat_ = true;
        if( presenter::pMap m = map_list_[id_].lock() ) {
            m->overheat_event()(true);
        }
        remove_haste_effect(); // only call this after you're sure about overheat_ is true
        EventDispatcher::i().subscribe_timer(
            bind(&Player::end_overheat, this), shared_from_this(), overheat_downtime_);
    }
}

void Player::normal_shot_delegate
    (view::pSprite& sv, HitCallback const& hit_cb)
{
    if( !overheat_ ) {
        hit_cb(1); //normal_shot's firepower is always 1.
        if( player_hit_event_ )
            player_hit_event_();
    }
}

//2011.03.28 weapon temporary removal
//void Player::shot_delegate
//    (view::pSprite& sv, HitCallback const& hit_cb, wpPlayer player)
//{
//    if( pPlayer p = player.lock() ) {
//        std::list<int> const& that_allies = p->ally_input_ids();
//        std::list<int> const& self_allies = ally_input_ids_;
//        if( that_allies == self_allies || p->can_crossfire() )
//            hit_cb( p->weapon()->firepower() ); // if the player is ally OR player can crossfire
//    }
//}

void Player::shot_delegate //2011.03.28 new normal-jama shooting integration.
    (view::pSprite& sv, HitCallback const& hit_cb, wpPlayer player)
{
    if( pPlayer p = player.lock() ) {
        std::list<int> const& that_allies = p->ally_input_ids();
        std::list<int> const& self_allies = ally_input_ids_;
        if( that_allies == self_allies )
            hit_cb(1);
        else {
            hit_cb(1);
            p->generate_heat(heat_for_jama_shoot_); //if enemy hit, generate extra heat.
        }
    }
}

// note: no use for now?
//void Player::repeating_shot_delegate
//    (view::pSprite& sv, HitCallback const& hit_cb, wpPlayer player)
//{
//    if( pPlayer p = player.lock() ) {
//        std::list<int> const& that_allies = p->ally_input_ids();
//        std::list<int> const& self_allies = ally_input_ids_;
//        if( that_allies == self_allies || p->can_crossfire() )
    //        hit_cb( p->weapon()->firepower() );
//    }
//}

Input const* Player::input()          const { return input_; }
Weapon* Player::weapon()              const { return current_wep_; }
Weapon* Player::weapon(int id)        const { return weplist_[id]; }
bool Player::is_changing_wep()        const { return changing_wep_; }
bool Player::can_fire()               const { return current_wep_->can_fire(); }
bool Player::can_crossfire()          const { return current_wep_->can_crossfire(); }
bool Player::can_fire_repeatedly()    const { return current_wep_->can_fire_repeatedly(); }
int  Player::wepid()                  const { return weplist_idx_; }
double Player::heat()                 const { return accumulated_heat_; }
bool Player::is_overheat()            const { return overheat_; }
int  Player::overheat_downtime()      const { return overheat_downtime_; }
int  Player::id()                     const { return id_; }
std::list<int> const& Player::ally_input_ids()  const { return ally_input_ids_;  }
std::list<int> const& Player::enemy_input_ids() const { return enemy_input_ids_; }
bool Player::ammo_all_out() const {
    int count = 0;
    BOOST_FOREACH(Weapon* wp, weplist_)
        count += wp->ammo();
    return count == 0;
}

//2011.03.28 make hasting a player effect.
void Player::start_haste_effect()
{
    using namespace accessor;
    using namespace easing;
    if( !overheat_ ) {
        vec3 rot = input_->getCursor()->get<Rotation>();
        rot.Z += 360; //will this overflow eventually?
        input_->getCursor()->tween<Linear, Rotation>(rot, 1000u, -1);
        hasting_ = true;
    }
}

void Player::remove_haste_effect()
{
    using namespace accessor;
    using namespace easing;
    if( hasting_ ) {
        vec3 rot = input_->getCursor()->get<accessor::Rotation>();
        rot.Z += 360; //will this overflow eventually?
        input_->getCursor()->tween<Linear, Rotation>(rot, 3000u, -1);
        hasting_ = false;
    }
}

//temp: not flexible and stupid.
void Player::normal_weapon_fx() {
    if( !overheat_ ) {
        audio::Sound::i().playBuffer("1/a/1a-1.wav");
        view::SFX::i().normal_weapon_vfx(
            InputMgr::i().scene(), vec2(input_->cursor().x(), input_->cursor().y()) );

        generate_heat(heat_for_normal_shoot_);
    }
    else {
        /* special effects of attempting to fire when overheated */
    }
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
    audio::Sound::i().playBuffer("1/e/getitem.wav");
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
