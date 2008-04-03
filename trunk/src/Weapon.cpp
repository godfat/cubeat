
#include "Weapon.hpp"
#include "EventDispatcher.hpp"
//#include "Sound.hpp"
#include <iostream>

using namespace psc;
using namespace ctrl;

void Weapon::reload() {
    if( reloadable_ == true && reloading_ == false ) {
        reloading_ = true;
        //Sound::i().reload();
        //call reset() by timer
    }
}

///////////////// Blocking Shoot ////////////////////

void BlockShoot::fire() {
    if( ammo_ >= 0 && reloading_ == false ) {
        --ammo_;
        //Sound::i().blockShoot();
        //use Timer to refresh coolingdown_
    }
}

///////////////// Powerful Shoot ////////////////////

void PowerShoot::fire() {
    if( ammo_ >= 0 && reloading_ == false ) {
        --ammo_;
        //Sound::i().powerShoot();
        //use Timer to refresh coolingdown_
    }
}

////////////////// Area Shoot ///////////////////////

void AreaShoot::fire() {
    if( ammo_ >= 0 && reloading_ == false ) {
        --ammo_;
        //Sound::i().rocket();
        //use Timer to refresh coolingdown_
    }
}
