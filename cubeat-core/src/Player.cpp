
#include "Player.hpp"
#include "Weapon.hpp"
#include "Input.hpp"
#include "EventDispatcher.hpp"
#include "ctrl/TimerDispatcher.hpp"
#include "view/Sprite.hpp"
#include "view/SFX.hpp"
#include "audio/Sound.hpp"
#include "utils/Random.hpp"
#include "presenter/Map.hpp"
#include "presenter/PlayerAbility.hpp"
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
     overheat_downtime_(2000), overheat_(false), hasting_(false), lock_heat_(false), ability_kind_(7),
     input_(input), player_hit_event_(0), player_overheat_event_(0)
{
}

pPlayer Player::init()
{
    weplist_.push_back( new BlockShoot( shared_from_this() ) );
    weplist_.push_back( new PowerShoot( shared_from_this() ) );
    weplist_.push_back( new AreaShoot( shared_from_this() ) );
    current_wep_ = weplist_[0];

    if( input_ )
        input_->player( shared_from_this() );

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

//2012.02.20 When player get to know his map, map has to know when player press haste
void Player::setMapList(std::vector<presenter::wpMap> const& mlist)
{
    map_list_ = mlist;
    if( presenter::pMap m = map_list_[id_].lock() ) {
        m->hasting_cond(bind(&Player::is_hasting, this));
    }
}

//2011.03.28 This will provide a modifiable speed for ViewSprite
//float Player::haste_speedfunc(float orig_speed) const
//{
//    return hasting_ ? 450.f : orig_speed;
//}

void Player::heat_cooling()
{
    if( !overheat_ ) { //2011.03.28 when hasting you shouldn't cool
        if( !hasting_ ) {
            if( accumulated_heat_ > 0 ) {
                delta_heat( - cooling_speed_ );
            }
        }
        else {
            generate_heat(heat_for_haste_);
        }
    }
}

Player& Player::start_heat_timer()
{
    EventDispatcher::i().get_timer_dispatcher("game")->subscribe(
        bind(&Player::heat_cooling, this), shared_from_this(), 100, -1); //check for cooling every 100ms
    return *this;
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

Player& Player::invoke_ability(view::pSprite const&)
{
    if( ability_queue_.size() > 0 ) {
        presenter::PlayerAbility::Callback ab = ability_queue_.front();
        ability_queue_.pop_front();

        if( !map_list_.empty() ) {
            map_list_[id_].lock()->ability_button_event()( ability_left() );
        }
        //NOTE WTF TEMP 2012: let's be lazy for now. It probably will always be only 2 maps anyway.
        ab(shared_from_this(), map_list_[id_], map_list_[enemy_input_ids_.front()] );
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

    std::cout << "Player " << id_ << " ability kind: " << config.I("ability_kind") << std::endl;
    ability_kind_ = config.I("ability_kind");

    push_ability();

    return *this;
}

Player& Player::push_ability(int kind)
{
    if( ability_queue_.size() < 1 /*some config value*/ ) {
        using std::tr1::bind;
        using presenter::PlayerAbility;

        if( kind <= 0 )
            kind = ability_kind_;

        switch( kind ) {
            case 1: ability_queue_.push_back( bind(&PlayerAbility::C1, _1, _2, _3) ); break;
            case 2: ability_queue_.push_back( bind(&PlayerAbility::C2, _1, _2, _3) ); break;
            case 3: ability_queue_.push_back( bind(&PlayerAbility::C3, _1, _2, _3) ); break;
            case 4: ability_queue_.push_back( bind(&PlayerAbility::C4, _1, _2, _3) ); break;
            case 5: ability_queue_.push_back( bind(&PlayerAbility::C5, _1, _2, _3) ); break;
            case 6: ability_queue_.push_back( bind(&PlayerAbility::C6, _1, _2, _3) ); break;
            case 7: ability_queue_.push_back( bind(&PlayerAbility::C7, _1, _2, _3) ); break;
            case 8: ability_queue_.push_back( bind(&PlayerAbility::C8, _1, _2, _3) ); break;
            default:
                ability_queue_.push_back( bind(&PlayerAbility::C7, _1, _2, _3) ); break;
        }

        if( !map_list_.empty() ) {
            map_list_[id_].lock()->ability_button_event()( ability_left() );
        }
    }
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
        if( player_overheat_event_ ) {
            player_overheat_event_(id(), false);
        }
        accumulated_heat_ /= 1.5;
    }
}

int Player::delta_heat(double d)
{
    if( lock_heat_ ) return 0;

    /// Heat Debug: Floating point determinism problem??
    printf(" Player %d previous heat %lf, delta %lf, ", id(), accumulated_heat_, d);

    accumulated_heat_ += d;

    printf("current heat = %lf (unconstrained).\n", accumulated_heat_);

    if( accumulated_heat_ > 1 ) {
        accumulated_heat_ = 1;
        return 1;
    }
    else if( accumulated_heat_ < 0 ) {
        accumulated_heat_ = 0;
        return -1;
    }
    return 0;
}

void Player::generate_heat(double heat)
{
    using std::tr1::ref;
    if( delta_heat( heat ) ) {
        overheat_ = true;
        printf(" -= Player %d OVERHEAT =-\n", id());
        if( presenter::pMap m = map_list_[id_].lock() ) {
            m->overheat_event()(true);
            if( player_overheat_event_ ) {
                player_overheat_event_(id(), true);
            }
        }
        remove_haste_effect(); // only call this after you're sure about overheat_ is true
        EventDispatcher::i().get_timer_dispatcher("game")->subscribe(
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
        if( !p->is_overheat() ) {
            std::list<int> const& that_allies = p->ally_input_ids();
            std::list<int> const& self_allies = ally_input_ids_;
            if( that_allies == self_allies ) {
                hit_cb(1);
            }
            else {
                hit_cb(1);
                p->generate_heat(heat_for_jama_shoot_); //if enemy hit, generate extra heat.
            }
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
bool Player::is_hasting()             const { return hasting_; }
int  Player::overheat_downtime()      const { return overheat_downtime_; }
int  Player::id()                     const { return id_; }
int  Player::ability_kind()           const { return ability_kind_; }
size_t Player::ability_left()         const { return ability_queue_.size(); }
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
        rot.Z -= 360; //will this overflow eventually?
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
        rot.Z -= 360; //will this overflow eventually?
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
    int percent = utils::ya_random(100);
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
