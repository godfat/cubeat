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

void Demo_init_vs_ppl(Demo* self) { // not shared_ptr!
    self->init_vs_ppl();
}

void Demo_init_vs_cpu(Demo* self) { // not shared_ptr!
    self->init_vs_cpu();
}

void Demo_init_cpudemo(Demo* self) { // not shared_ptr!
    self->init_cpudemo();
}
