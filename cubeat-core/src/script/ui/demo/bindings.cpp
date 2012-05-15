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
