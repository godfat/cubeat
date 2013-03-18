
#include "model/Chain.hpp"
#include <cstdio>

using namespace psc;
using namespace model;

    /// WTF NOTE TO SELF: Chain::chain_step_ will be a problem when rolling back.
    //  unless the container is stationary or fixed during the life time of this object, (which is rarely likely)
    //  we can't have a persistent, not-rollbackable std container inside a rollbackable object.

Chain::Chain():last_step_x_(0), last_step_y_(0), power_factor_(0), last_power_factor_(0){}

Chain::Chain(pointer_type const& that):
    chain_step_(that->chain_step_),
    last_step_x_(that->last_step_x_),
    last_step_y_(that->last_step_y_),
    power_factor_(that->power_factor_),
    last_power_factor_(that->last_power_factor_)
{}

Chain::~Chain() {
    printf("Chain %x d'tor called.\n", this);
}

int Chain::last_step_amounts() const{ return chain_step_.back(); }

void Chain::add_last_step_amounts(int num){
    chain_step_.back() += num;
    // always add onto the rear element
    // map will be responsible to notify Chain to push new elements to Chains.
}

void Chain::step_one(){
    chain_step_.push_back(0);
}

int Chain::power_factor() const{ return power_factor_; }

void Chain::update_power_factor() {
    last_power_factor_ = power_factor_;
    int new_step_amount = chain_step_.back();
    if( new_step_amount < 3 ) return;
    int stepnum = chain_step_.size();
    if( stepnum == 1 ) {
        // power_factor_ += (new_step_amount - 2);
        if( new_step_amount > 3 ) {
            power_factor_ += (new_step_amount - 1);
        }
    }
    else
        power_factor_ += (new_step_amount - 1)*(stepnum)/2 + (stepnum-1);
        //power_factor_ += (new_step_amount - 1)/2*(stepnum) + (stepnum-2);
        //power_factor_ += (new_step_amount - 1)/2*(stepnum) + (stepnum-2)*2;
        //power_factor_ += (new_step_amount - 2)*(stepnum);
        //power_factor_ += (new_step_amount - 1)/2*(stepnum) + utils::power2(stepnum-2);
        //power_factor_ += (new_step_amount - 1)*(stepnum) + utils::power2(stepnum-2);
}

// this returns the score fragment made by the last combo
int Chain::score_step() const{
    return attack_of_current_step() * 50;
}

int Chain::step() const{
    return chain_step_.size();
}

void Chain::set_last_step_position(int x, int y) { last_step_x_ = x; last_step_y_ = y; }
int Chain::last_step_x() const { return last_step_x_; }
int Chain::last_step_y() const { return last_step_y_; }
int Chain::attack_of_current_step() const {
    return power_factor_ - last_power_factor_;
}

