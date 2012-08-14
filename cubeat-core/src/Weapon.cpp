
#include "Weapon.hpp"
#include "audio/Sound.hpp"
#include "view/SFX.hpp"
#include "view/Scene.hpp" //warning: very bad!!!!
#include "Input.hpp"
#include "Player.hpp"
#include <iostream>

using namespace psc;
using namespace ctrl;
using std::tr1::bind;
using namespace std::tr1::placeholders;

void Weapon::reload() {
    if( reloadable_ == true && reloading_ == false ) {
        reloading_ = true;
        //audio::Sound::i().reload();
        //call reset() by timer
    }
}

///////////////// Blocking Shoot ////////////////////

BlockShoot::BlockShoot(wpPlayer const& p):Weapon(p, 1, 10, 99, true, false){} //must use config

void BlockShoot::fire(vec2 const& pos) {
    if( ammo_ > 0 && reloading_ == false ) {
        --ammo_;
        audio::Sound::i().playBuffer("1/a/1a-2.wav");
        view::SFX::i().weapon_vfx1( InputMgr::i().scene(), pos );
        //use Timer to refresh coolingdown_
    }
}

///////////////// Powerful Shoot ////////////////////

PowerShoot::PowerShoot(wpPlayer const& p):Weapon(p, 3, 10, 99, false, false){} //must use config

void PowerShoot::fire(vec2 const& pos) {
    if( ammo_ > 0 && reloading_ == false ) {
        --ammo_;
        audio::Sound::i().playBuffer("1/a/1a-3.wav");
        view::SFX::i().weapon_vfx2( InputMgr::i().scene(), pos );
        //use Timer to refresh coolingdown_
    }
}

////////////////// Area Shoot ///////////////////////

AreaShoot::AreaShoot(wpPlayer const& p):Weapon(p, 3, 1, 99, true, false){} //must use config

void AreaShoot::fire(vec2 const& pos) {
    if( ammo_ > 0 && reloading_ == false ) {
        --ammo_;
        audio::Sound::i().playBuffer("1/a/1a-4.wav");
        view::SFX::i().weapon_vfx3( InputMgr::i().scene(), pos );
        //use Timer to refresh coolingdown_
    }
}

AreaPredicate AreaShoot::areaPredicate() {
    return bind(&AreaShoot::area, this, _1, _2, 190, 190);
}

bool AreaShoot::area(vec2 const& p, vec2 const& obj_pos, int w, int h) {
    return obj_pos.X > p.X - w/2 && obj_pos.X < p.X + w/2 &&
           obj_pos.Y > p.Y - h/2 && obj_pos.Y < p.Y + h/2;
}
