
#include "model/Cube.hpp"

#include "model/state/State.hpp"

#include "all_fwd.hpp"

using namespace psc;
using namespace model;

bool Cube::cycle_and_die(){
    if( cycled_ )
        return false; // i am alive though killed, think of DYING NOW

    state::pState next_state = state_->cycle();
    if(next_state){
        if(next_state != state_){
            // state_->suicide();
            state_ = next_state;
        }
        return false; // i am alive
    }
    else{
        // this->suicide();
        return true; // i am dead
    }
}
