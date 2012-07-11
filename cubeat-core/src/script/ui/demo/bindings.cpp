#include "view/Scene.hpp"
#include "presenter/game/Demo.hpp"

using namespace psc;
using namespace view;
using namespace presenter;
using namespace game;

extern "C" {
#include "script/ui/demo/bindings.h"
}

pScene* Demo_get_ui_scene(Demo* self) { // not shared_ptr!
    pScene* s = new pScene;
    *s = self->get_ui_scene();
    return s;
}

void Demo_init_vs_ppl(Demo* self, char const* c1p, char const* c2p, char const* scene_name) { // not shared_ptr!
    self->init_vs_ppl(c1p, c2p, scene_name);
}

void Demo_init_vs_cpu(Demo* self, char const* c1p, char const* c2p, char const* scene_name) { // not shared_ptr!
    self->init_vs_cpu(c1p, c2p, scene_name);
}

void Demo_init_cpudemo(Demo* self, char const* c1p, char const* c2p, char const* scene_name) { // not shared_ptr!
    self->init_cpudemo(c1p, c2p, scene_name);
}

void Demo_quit(Demo* self) { // not shared_ptr!
    self->quit();
}

