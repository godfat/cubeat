#include "view/Scene.hpp"
#include "presenter/game/Demo.hpp"

using namespace psc;
using namespace view;
using namespace presenter;
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

void Demo_init_cpudemo(Demo* self, char const* c1p, char const* c2p, char const* scene_name) { // not shared_ptr!
    self->init_cpudemo(c1p, c2p, scene_name);
}

void Demo_init_ai_logging(Demo* self, char const* c1p, char const* c2p, char const* scene_name) { // not shared_ptr!
    self->init_ai_logging(c1p, c2p, scene_name);
}

void Demo_init_puzzle(Demo* self, char const* c1p, char const* scene_name) { // not shared_ptr!
    self->init_puzzle(c1p, scene_name);
}

void Demo_init_tutorial(Demo* self, char const* c1p, char const* c2p, char const* scene_name, bool in_place, int submode) { // not shared_ptr!
    self->init_tutorial(c1p, c2p, scene_name, in_place, submode);
}

void Demo_init_map_starting_line(Demo* self, int map_id, int n) {
    self->init_map_starting_line(map_id, n);
}

void Demo_set_map_garbage_amount(Demo* self, int map_id, int n) {
    self->set_map_garbage_amount(map_id, n);
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

void Demo_quit(Demo* self) { // not shared_ptr!
    self->quit();
}

