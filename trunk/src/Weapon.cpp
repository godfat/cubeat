
#include "Weapon.hpp"
#include "EventDispatcher.hpp"
#include "Sound.hpp"
#include <iostream>

using namespace psc;
using namespace ctrl;
using std::tr1::bind;
using namespace std::tr1::placeholders;

void Weapon::reload() {
    if( reloadable_ == true && reloading_ == false ) {
        reloading_ = true;
        //Sound::i().reload();
        //call reset() by timer
    }
}

///////////////// Blocking Shoot ////////////////////

BlockShoot::BlockShoot():Weapon(1, 0, 99, true, false){} //must use config

void BlockShoot::fire() {
    if( ammo_ > 0 && reloading_ == false ) {
        --ammo_;
        Sound::i().play("1/a/1a-2.mp3");
        //use Timer to refresh coolingdown_
    }
}

///////////////// Powerful Shoot ////////////////////

PowerShoot::PowerShoot():Weapon(3, 0, 99, false, false){} //must use config

void PowerShoot::fire() {
    if( ammo_ > 0 && reloading_ == false ) {
        --ammo_;
        Sound::i().play("1/a/1a-3.mp3");
        //use Timer to refresh coolingdown_
    }
}

////////////////// Area Shoot ///////////////////////

AreaShoot::AreaShoot():Weapon(3, 0, 99, true, false){} //must use config

void AreaShoot::fire() {
    if( ammo_ > 0 && reloading_ == false ) {
        --ammo_;
        Sound::i().play("1/a/1a-4.mp3");
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
