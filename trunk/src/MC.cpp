
/* 2007.11.9
   Too many things infected by Virtools SDK things,
   needs total overhual, but the structure might possibly remain
   the same, so I'll just keep most of the things unchanged for future references.

   Basically this file is deprecated and cannot compile.
*/

#include "MC.hpp"

void SceneState::change_owner_state(SceneState* state){
    assert(this->scene_);
    this->scene_->state_ = state;
}

void SceneState::translate(double value){
    double const new_value = this->scene_->camera_.z()+value;
    scene_->camera_.z(new_value);
}
