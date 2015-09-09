#include "view/Scene.hpp"
#include "presenter/game/Demo.hpp"

// Is this really cross-platform?
#include <sys/stat.h>

using namespace psc;
using namespace view;
using namespace presenter;
using namespace ctrl;
using namespace game;

extern "C" {
#include "script/demo/bindings.h"
}

long get_ftime(char const* path) {
    struct stat fs;
    stat(path, &fs);
    return fs.st_mtime;
}

pScene* Demo_get_ui_scene(Demo* self) { // not shared_ptr!
    pScene* s = new pScene;
    *s = self->get_ui_scene();
    return s;
}

pScene* Demo_get_game_scene(Demo* self) { // not shared_ptr!
    pScene* s = new pScene;
    *s = self->get_game_scene();
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

void Demo_init_map_with_config(Demo* self, int map_id, char const* filename) { // not shared_ptr!
    self->init_map_with_config(map_id, filename);
}

void Demo_reset_map_record(Demo* self, int map_id) { // not shared_ptr!
    self->reset_map_record(map_id);
}

void Demo_hide_map_warning(Demo* self, int map_id) { // not shared_ptr!
    self->hide_map_warning(map_id);
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

void Demo_play_sound(Demo* self, char const* file) { // not shared_ptr!
    self->play_sound(file);
}

void Demo_hide_character_animations(Demo* self) { // not shared_ptr!
    self->hide_character_animations();
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

int Demo_get_map_broken_num(Demo* self, int map_id) { // not shared_ptr!
    return self->get_map_broken_num(map_id);
}

int Demo_get_map_max_warning_count(Demo* self, int map_id) { // not shared_ptr!
    return self->get_map_max_warning_count(map_id);
}

int const* Demo_get_map_cubes_cleared_data(Demo* self, int map_id) { // not shared_ptr!
    return self->get_map_cubes_cleared_data(map_id);
}

int const* Demo_get_map_cubes_matched_data(Demo* self, int map_id) { // not shared_ptr!
    return self->get_map_cubes_matched_data(map_id);
}

AIPlayer* Demo_get_ai_player(Demo* self) { // not shared_ptr!
    return self->get_ai_player();
}

bool Demo_is_map_dropping(Demo* self, int map_id) { // not shared_ptr!
    return self->is_map_dropping(map_id);
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

void Demo_leave_and_cleanup(Demo* self, bool ending_theme) { // not shared_ptr!
    self->end_sequence1(ending_theme);
}

void Demo_load_stage(Demo* self, int stage_id) { // not shared_ptr!
    self->load_stage(stage_id);
}

void Demo_quit(Demo* self) { // not shared_ptr!
    self->quit();
}

