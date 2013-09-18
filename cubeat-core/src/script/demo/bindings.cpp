#include "view/Scene.hpp"
#include "presenter/game/Demo.hpp"

using namespace psc;
using namespace view;
using namespace presenter;
using namespace ctrl;
using namespace game;

extern "C" {
#include "script/demo/bindings.h"
}

pScene* Demo_get_ui_scene(Demo* self) { // not shared_ptr!
    pScene* s = new pScene;
    *s = self->get_ui_scene();
    return s;
}

void Demo_init_vs_ppl(Demo* self, char const* c1p, char const* c2p, char const* scene_name) { // not shared_ptr!
    self->init_vs_ppl(c1p, c2p, scene_name);
}

void Demo_init_vs_cpu(Demo* self, char const* c1p, char const* c2p, char const* scene_name, int ai_level) { // not shared_ptr!
    self->init_vs_cpu(c1p, c2p, scene_name, ai_level);
}

void Demo_init_story(Demo* self, char const* c1p, char const* c2p, char const* scene_name, int ai_level) { // not shared_ptr!
    self->init_story(c1p, c2p, scene_name, ai_level);
}

void Demo_init_tutorial(Demo* self, char const* c1p, char const* c2p, char const* scene_name) { // not shared_ptr!
    self->init_tutorial(c1p, c2p, scene_name);
}

void Demo_init_cpudemo(Demo* self, char const* c1p, char const* c2p, char const* scene_name) { // not shared_ptr!
    self->init_cpudemo(c1p, c2p, scene_name);
}

void Demo_init_ai_logging(Demo* self, char const* c1p, char const* c2p, char const* scene_name) { // not shared_ptr!
    self->init_ai_logging(c1p, c2p, scene_name);
}

void Demo_init_single(Demo* self, int submode, int level, char const* c1p, char const* scene_name, bool in_place) { // not shared_ptr!
    self->init_single(submode, level, c1p, scene_name, in_place);
}

void Demo_init_map_starting_line(Demo* self, int map_id, int n) { // not shared_ptr!
    self->init_map_starting_line(map_id, n);
}

void Demo_set_only_one_shot_for_puzzle(Demo* self) { // not shared_ptr!
    self->set_only_one_shot_for_puzzle();
}

void Demo_set_map_garbage_amount(Demo* self, int map_id, int n) { // not shared_ptr!
    self->set_map_garbage_amount(map_id, n);
}

void Demo_set_map_dropping(Demo* self, int map_id, bool flag) { // not shared_ptr!
    self->set_map_dropping(map_id, flag);
}

void Demo_set_stage_name(Demo* self, char const* str) { // not shared_ptr!
    self->set_stage_name(str);
}

void Demo_set_countdown(Demo* self, bool flag) { // not shared_ptr!
    self->set_countdown(flag);
}

void Demo_set_time(Demo* self, int time) { // not shared_ptr!
    self->set_time(time);
}

int Demo_get_time(Demo* self) { // not shared_ptr!
    return self->get_time();
}

int Demo_get_map_score(Demo* self, int map_id) { // not shared_ptr!
    return self->get_map_score(map_id);
}

int Demo_get_map_highest_chain(Demo* self, int map_id) { // not shared_ptr!
    return self->get_map_highest_chain(map_id);
}

int Demo_get_map_garbage_left(Demo* self, int map_id) { // not shared_ptr!
    return self->get_map_garbage_left(map_id);
}

int Demo_get_map_warning_level(Demo* self, int map_id) { // not shared_ptr!
    return self->get_map_warning_level(map_id);
}

int const* Demo_get_map_cubes_cleared_data(Demo* self, int map_id) { // not shared_ptr!
    return self->get_map_cubes_cleared_data(map_id);
}

AIPlayer* Demo_get_ai_player(Demo* self) { // not shared_ptr!
    return self->get_ai_player();
}

bool Demo_is_map_all_waiting(Demo* self, int map_id) { // not shared_ptr!
    return self->is_map_all_waiting(map_id);
}

bool Demo_is_map_empty(Demo* self, int map_id) { // not shared_ptr!
    return self->is_map_empty(map_id);
}

bool Demo_is_puzzle_started(Demo* self) { // not shared_ptr!
    return self->is_puzzle_started();
}

void Demo_reinit(Demo* self) { // not shared_ptr!
    self->reinit();
}

void Demo_endgame(Demo* self, int map_num) { // not shared_ptr!
    self->endgame(map_num);
}

void Demo_eventual_pause(Demo* self) { // not shared_ptr!
    self->eventual_pause();
}

void Demo_eventual_resume(Demo* self) { // not shared_ptr!
    self->eventual_resume();
}

void Demo_leave_and_cleanup(Demo* self) { // not shared_ptr!
    self->end_sequence1();
}

void Demo_quit(Demo* self) { // not shared_ptr!
    self->quit();
}

